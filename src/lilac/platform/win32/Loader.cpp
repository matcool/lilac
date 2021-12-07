#pragma once

#include <Loader.hpp>
#include <Mod.hpp>
#include <CApiMod.hpp>
#include <InternalMod.hpp>
#include <Log.hpp>

#ifdef LILAC_IS_WINDOWS

#include <Windows.h>
#include <lilac/windows.hpp>

USE_LILAC_NAMESPACE();

#define TRY_C_AND_MANGLED(_var_, _to_, _c_, _mangled_)                      \
    auto _var_ = reinterpret_cast<_to_>(GetProcAddress(load, _c_));         \
    if (!_var_) {                                                           \
        _var_ = reinterpret_cast<_to_>(GetProcAddress(load, _mangled_));    \
    }

Mod* loadWithCApi(HMODULE load) {
    TRY_C_AND_MANGLED(loadFunc, lilac_c_load, "lilac_c_load", "_lilac_c_load@4");

    if (loadFunc) {
        auto info = new CModInfo;
        auto err = loadFunc(info);
        if (err) {
            // todo: log error in internal plugin
            delete info;
            return nullptr;
        }
        auto mod = new CApiMod(info);

        TRY_C_AND_MANGLED(unloadFunc, lilac_c_unload, "lilac_c_unload", "_lilac_c_unload@0");
        TRY_C_AND_MANGLED(enableFunc, lilac_c_enable, "lilac_c_enable", "_lilac_c_enable@0");
        TRY_C_AND_MANGLED(disableFunc,lilac_c_disable,"lilac_c_disable","_lilac_c_disable@0");

        mod->m_loadFunc     = loadFunc;
        mod->m_unloadFunc   = unloadFunc;
        mod->m_enableFunc   = enableFunc;
        mod->m_disableFunc  = disableFunc;

        delete info;
        return mod;
    }
    return nullptr;
}

Loader::PlatformInfoCheckResult Loader::checkPlatformInfo(std::string const& path) {
    // https://stackoverflow.com/questions/46024914/
    // how-to-parse-exe-file-and-get-data-from-image-
    // dos-header-structure-using-c-and

    auto hFile = CreateFileA(
        path.c_str(),
        GENERIC_READ, FILE_SHARE_READ,
        nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        this->logStream() << Severity::Error << InternalMod::get()
            << "Unable to get handle for \"" << path << "\""
            << lilac::endl;
        return PlatformInfoCheckResult::UnableToLoad;
    }

    auto hFileMapping = CreateFileMapping(
        hFile, nullptr, PAGE_READONLY, 0, 0, nullptr
    );

    if (hFileMapping == 0) {
        CloseHandle(hFile);
        this->logStream() << Severity::Error << InternalMod::get()
            << "Unable to create file mappings for \"" << path << "\""
            << lilac::endl;
        return PlatformInfoCheckResult::UnableToLoad;
    }

    auto lpFileBase = MapViewOfFile(
        hFileMapping, FILE_MAP_READ, 0, 0, 0
    );

    if (lpFileBase == 0) {
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        this->logStream() << Severity::Error << InternalMod::get()
            << "Unable to do MapViewOfFile for \"" << path << "\""
            << lilac::endl;
        return PlatformInfoCheckResult::UnableToLoad;
    }

    auto dosHeader = (PIMAGE_DOS_HEADER) lpFileBase;  //pointer to dos headers

    if (dosHeader->e_magic == IMAGE_DOS_SIGNATURE) {
        auto peHeader = (PIMAGE_NT_HEADERS) ((u_char*)dosHeader+dosHeader->e_lfanew);

        if (peHeader->Signature == IMAGE_NT_SIGNATURE) {
            auto sectionCount = peHeader->FileHeader.NumberOfSections;
            if (sectionCount == 0) {
                this->logStream() << Severity::Error << InternalMod::get()
                    << "Found no sections in \"" << path << "\""
                    << lilac::endl;
                UnmapViewOfFile(lpFileBase);
                CloseHandle(hFileMapping);
                CloseHandle(hFile);
                return PlatformInfoCheckResult::UnableToLoad;
            }
            uintptr_t peSize = peHeader->OptionalHeader.Magic == 0x10b ? 
                sizeof IMAGE_OPTIONAL_HEADER32 :
                sizeof IMAGE_OPTIONAL_HEADER64;
            auto sectionStart = (reinterpret_cast<uintptr_t>(&peHeader->OptionalHeader) + peSize);
            std::cout << sectionStart << "\n";
            auto sections = reinterpret_cast<IMAGE_SECTION_HEADER*>(sectionStart);
            for (auto i = 0; i < sectionCount; i++) {
                auto section = sections[i];
                this->logStream() << "section: " << reinterpret_cast<const char*>(section.Name) << lilac::endl;
                if (strcmp(reinterpret_cast<const char*>(section.Name), ".lilac") == 0) {
                    std::cout << reinterpret_cast<uintptr_t>(lpFileBase) + section.VirtualAddress << "\n";
                    auto data = *reinterpret_cast<const char**>(
                        reinterpret_cast<uintptr_t>(lpFileBase) + section.VirtualAddress
                    );
                    std::cout << "yeah\n";
                    this->logStream() << Severity::Debug << InternalMod::get()
                        << "Data: " << data << lilac::endl;
                    UnmapViewOfFile(lpFileBase);
                    CloseHandle(hFileMapping);
                    CloseHandle(hFile);
                    return PlatformInfoCheckResult::WontLoad;
                }
            }
        }

        this->logStream() << Severity::Error << InternalMod::get()
            << "No .lilac section found, loading DLL"
            << lilac::endl;

        UnmapViewOfFile(lpFileBase);
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return PlatformInfoCheckResult::WillLoad;
    } else {
        this->logStream() << Severity::Error << InternalMod::get()
            << "Unable to match DOS signature for \"" << path << "\""
            << lilac::endl;
        UnmapViewOfFile(lpFileBase);
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return PlatformInfoCheckResult::UnableToLoad;
    }
}

bool Loader::loadModFromFile(std::string const& path) {
    auto check = this->checkPlatformInfo(path);
    if (check == PlatformInfoCheckResult::UnableToLoad) {
        return false;
    }
    if (check == PlatformInfoCheckResult::WontLoad) {
        return true;
    }
    auto load = LoadLibraryA(path.c_str());
    if (load) {
        Mod* mod = nullptr;

        TRY_C_AND_MANGLED(loadFunc, lilac_load, "lilac_load", "_lilac_load@0");

        if (loadFunc) {
            mod = loadFunc();
        } else {
            mod = loadWithCApi(load);
        }
        if (mod) {
            mod->setup();
            // todo: fix dependencies (this does not work.)
            if (this->checkDependencies(mod)) {
                mod->m_enabled = true;
            } else {
                mod->disableBase();
            }
            mod->m_platformInfo = new PlatformInfo { load };
            mod->m_path = path.c_str();
            this->m_mods.push_back(mod);
            return true;
        }
    }
    return false;
}

#endif
