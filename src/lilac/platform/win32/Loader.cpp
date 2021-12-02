#pragma once

#include <Loader.hpp>
#include <Mod.hpp>
#include <CApiMod.hpp>

#ifdef LILAC_IS_WINDOWS

#include <Windows.h>
#include <Win32.hpp>

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

bool Loader::loadModFromFile(std::string const& path) {
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
