#pragma once

#include <lilac.hpp>
#include "InternalMod.hpp"
#include "address.hpp"

USE_LILAC_NAMESPACE();
using namespace std::literals::string_literals;

#define CREATE_HOOK(_class_, _func_) \
    static CreateHookAddr<&_class_##_##_func_>$##_class_##_##_func_(addressOf<&_class_##::##_func_>());

template<auto Func, auto Detour>
struct CreateHook {
    constexpr CreateHook() {
        auto res = InternalMod::get()->addHook(as<void*>(Func), as<void*>(Detour));
        if (!res) {
            InternalMod::get()->throwError(res.error(), Severity::Critical);
        }
    }
};

template<auto Detour>
struct CreateHookAddr {
    constexpr CreateHookAddr(void* addr) {
        auto res = InternalMod::get()->addHook(addr, as<void*>(Detour));
        if (!res) {
            InternalMod::get()->throwError(res.error(), Severity::Critical);
        } else {
            InternalMod::get()->log() << "Succesfully hooked " << addr << lilac::endl;
        }
    }
    constexpr CreateHookAddr(uintptr_t addr) {
        auto _ = CreateHookAddr<Detour>(as<void*>(addr));
    }
};

template<auto Detour>
struct CreateHookMod {
    static inline std::unordered_map<std::string, HMODULE> m_mods = {};

    constexpr CreateHookMod(const char* module, const char* symbol) {
        HMODULE mod;
        if (m_mods.count(module)) {
            mod = m_mods[module];
        } else {
            mod = GetModuleHandleA(module);
            m_mods[module] = mod;
        }
        if (!mod) {
            InternalMod::get()->throwError(
                "Unable to find module \""s + module + "\"",
                Severity::Critical
            );
        }
        auto addr = reinterpret_cast<uintptr_t>(GetProcAddress(mod, symbol));
        if (!addr) {
            InternalMod::get()->throwError(
                "Unable to find symbol \""s + symbol + "\" in module \""s + module + "\"",
                Severity::Critical
            );
        }
        auto _ = CreateHookAddr<Detour>(addr);
    }
};
