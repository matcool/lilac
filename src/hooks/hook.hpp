#pragma once

#include <lilac.hpp>
#include "InternalMod.hpp"

USE_LILAC_NAMESPACE();

template<auto Func, auto Detour>
struct CreateHook {
    CreateHook() {
        auto res = InternalMod::get()->addHook(as<void*>(Func), as<void*>(Detour));
        if (!res) {
            InternalMod::get()->throwError(res.error(), Severity::Critical);
        }
    }
};

template<auto Detour>
struct CreateHookAddr {
    CreateHookAddr(uintptr_t addr) {
        auto res = InternalMod::get()->addHook(as<void*>(addr), as<void*>(Detour));
        if (!res) {
            InternalMod::get()->throwError(res.error(), Severity::Critical);
        } else {
            InternalMod::get()->log() << "Succesfully hooked " << addr << lilac::endl;
        }
    }
};

template<auto Detour>
struct CreateHookMod {
    static inline std::unordered_map<std::string, HMODULE> m_mods = {};

    CreateHookMod(const char* module, const char* symbol) {
        HMODULE mod;
        if (m_mods.count(module)) {
            mod = m_mods[module];
        } else {
            mod = GetModuleHandleA(module);
            m_mods[module] = mod;
        }
        if (!mod) {
            InternalMod::get()->throwError(
                "Unable to find module \""_s + module + "\"",
                Severity::Critical
            );
        }
        auto addr = reinterpret_cast<uintptr_t>(GetProcAddress(mod, symbol));
        if (!addr) {
            InternalMod::get()->throwError(
                "Unable to find symbol \""_s + symbol + "\" in module \""_s + module + "\"",
                Severity::Critical
            );
        }
        auto _ = CreateHookAddr<Detour>(addr);
    }
};
