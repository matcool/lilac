#include "../api/Lilac.h"

template<typename Hook_T>
lilac::Hook* lilac::Mod::add_hook(lilac::l_addr address, Hook_T detour) {
    auto hook = new lilac::Hook(this, address);

    if (hook && lilac::Loader::shared().add_hook(hook, detour)) {
        this->m_hooks.push_back(hook);

        return hook;
    }

    while (hook) delete hook;

    return nullptr;
}
