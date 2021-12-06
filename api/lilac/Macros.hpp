#pragma once

#include <utils/include.hpp>

#define LILAC_C_API \
    extern "C" __declspec(dllexport)

// Macros for making default functions for mods
#define LILAC_LOAD(_modName_) \
    extern "C" __declspec(dllexport) lilac::Mod* LILAC_CALL lilac_load() { return _modName_::get(); }

#define LILAC_MOD_GET(_modName_)            \
    public:                                 \
    static inline _modName_* get() {        \
        static auto g_mod = new _modName_;  \
        return g_mod;                       \
    }
