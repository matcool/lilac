#pragma once

#include <lilac.hpp>

#ifdef LILAC_IS_WIN32

#include <Windows.h>

USE_LILAC_NAMESPACE();

bool Loader::loadModFromFile(std::string const& path) {
    auto load = LoadLibraryA(path.c_str());
    if (load) {
        auto loadFunc = reinterpret_cast<lilac_load_type>(GetProcAddress(load, "lilac_load"));
        if (!loadFunc) {
            loadFunc  = reinterpret_cast<lilac_load_type>(GetProcAddress(load, "_lilac_load@0"));
        }
        if (loadFunc) {
            auto mod = loadFunc();
            if (mod) {
                mod->setup();
                mod->m_platformInfo = new PlatformInfo { load };
                mod->m_path = path.c_str();
                this->m_loadedMods.push_back(mod);
                return true;
            }
        }
    }
    return false;
}

#endif
