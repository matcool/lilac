#pragma once

#include <Loader.hpp>
#include <Mod.hpp>
#include <CApiMod.hpp>
#include <InternalMod.hpp>
#include <Log.hpp>

#ifdef LILAC_IS_WINDOWS

#include <Windows.h>
#include <lilac/windows.hpp>
#include <unzipper.h>
#include <json.hpp>

using namespace zipper;

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

Result<bool> Loader::checkDependencies(std::string const& path) {
    auto unzip = Unzipper(path);
    lilac::byte_array bytes {};
    if (!unzip.extractEntryToMemory("mod.json", bytes)) {
        return Err<>(
            "\"" + path + "\" is missing mod.json "
            "(or is not a zip file at all)"
        );
    }
    auto json = nlohmann::json(bytes);
    if (!json.is_object()) {
        return Err<>(
            "\"" + path + "/mod.json\" does not have an "
            "object at root despite expected"
        );
    }
    return Ok<>(true);
}

Result<> Loader::loadModFromFile(std::string const& path) {
    auto check = this->checkDependencies(path);
    if (!check) {
        return check;
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
            mod->m_enabled = true;
            mod->m_platformInfo = new PlatformInfo { load };
            mod->m_path = path.c_str();
            this->m_mods.push_back(mod);
            return Ok<>();
        } else {
            return Err<>("Unable to find load functions within " + path);
        }
    }
    return Err<>("Unable to load the DLL from \"" + path + "\"");
}

#endif
