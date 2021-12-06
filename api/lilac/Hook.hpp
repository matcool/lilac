#pragma once

#include "macros.hpp"
#include <inttypes.h>

namespace lilac {
    class Mod;
    class ModBase;
    class Loader;

    class LILAC_DLL Hook {
        protected:
            Mod*  m_owner;
            void* m_address;
            void* m_detour;
            void* m_handle = nullptr;
            bool  m_enabled;

            friend class Mod;
            friend class Loader;

            // Only allow friend classes to create
            // hooks. Whatever method created the
            // hook should take care of populating
            // m_owner, m_address, m_detour and 
            // m_handle.
            Hook() : m_enabled(false) {}

            // no copying
            Hook(Hook const&) = delete;
            Hook operator=(Hook const&) = delete;

            friend class Mod;
            friend class ModBase;

        public:
            /**
             * Get the address of the function hooked.
             * @returns Address
             * @author HJfod
             */
            uintptr_t getAddress() const { return reinterpret_cast<uintptr_t>(m_address); }

            /**
             * Get whether the hook is enabled or not.
             * @returns True if enabled, false if not.
             * @author HJfod
             */
            bool isEnabled() const { return m_enabled; }

            /**
             * Get the owner of this hook.
             * @returns Pointer to the owner's Mod handle.
             * @author HJfod
             */
            Mod* getOwner() const { return m_owner; }
    };
}
