#include <Hook.hpp>
#include <vector>
#include <Mod.hpp>
#include <Loader.hpp>
#include <utils/other/casts.hpp>
#include <utils/other/vector.hpp>
#include <core/hook/hook.hpp>
#include "Internal.hpp"

USE_LILAC_NAMESPACE();

struct hook_info {
    Hook* hook;
    Mod* mod;
};

static std::vector<hook_info> g_hooks;
static bool g_readyToHook = false;

Result<Hook*> ModBase::addHookBase(void* addr, void* detour, Hook* hook) {
    if (!hook) {
        hook = new Hook();
        hook->m_address = addr;
    }
    if ((hook->m_handle = const_cast<void*>(lilac::core::hook::add(addr, detour)))) {
        this->m_hooks.push_back(hook);
        hook->m_enabled = true;
        return Ok<Hook*>(hook);
    } else {
        delete hook;
        return Err<>(
            "Unable to create hook at " + std::to_string(as<uintptr_t>(addr))
        );
    }
}

Result<Hook*> ModBase::addHookBase(Hook* hook) {
    return this->addHookBase(
        hook->m_address,
        hook->m_detour,
        hook
    );
}

Result<> Mod::enableHook(Hook* hook) {
    if (!hook->isEnabled()) {
        if (!hook->m_handle) {
            if ((hook->m_handle = const_cast<void*>(lilac::core::hook::add(hook->m_address, hook->m_detour)))) {
                hook->m_enabled = true;
                return Ok<>();
            }
            return Err<>("Unable to create hook");
        }
        return Err<>("Hook already has a handle");
    }
    return Ok<>();
}

Result<> Mod::disableHook(Hook* hook) {
    if (hook->isEnabled()) {
        if (hook->m_handle) {
            if (lilac::core::hook::remove(hook->m_handle)) {
                hook->m_enabled = false;
                hook->m_handle = nullptr;
                return Ok<>();
            }
            return Err<>("Unable to remove hook");
        }
        return Err<>("Hook lacks a handle");
    }
    return Ok<>();
}

Result<> Mod::removeHook(Hook* hook) {
    auto res = this->disableHook(hook);
    if (res) {
        vector_erase<Hook*>(this->m_hooks, hook);
        delete hook;
    }
    return res;
}

Result<Hook*> Mod::addHook(void* addr, void* detour) {
    if (g_readyToHook) {
        return this->addHookBase(addr, detour);
    } else {
        auto hook = new Hook();
        hook->m_address = addr;
        hook->m_detour = detour;
        g_hooks.push_back({ hook, this });
        return Ok<Hook*>(hook);
    }
}

Result<Hook*> Mod::addHook(void* addr, void* detour, void** trampoline) {
    *trampoline = addr;
    return this->addHook(addr, detour);
}

bool Lilac::loadHooks() {
    auto thereWereErrors = false;
    for (auto const& hook : g_hooks) {
        auto res = hook.mod->addHookBase(hook.hook);
        if (!res) {
            hook.mod->throwError(
                res.error(),
                Severity::Error
            );
            thereWereErrors = true;
        }
    }
    g_readyToHook = true;
    return thereWereErrors;
}
