#pragma once

#include "Macros.hpp"
#include <inttypes.h>

namespace lilac {
    using address_t = uintptr_t;

    class LILAC_DLL Hook {
        private:
            void* m_address;
            void* m_detour;
            void* m_handle = nullptr;
            bool m_enabled;

        public:
            inline address_t getAddress() const {
                return reinterpret_cast<address_t>(m_address);
            }
            inline bool isEnabled() const {
                return m_enabled;
            }

            Hook(address_t addr) :
                m_address(reinterpret_cast<void*>(addr)), m_enabled(false) {}
            Hook(void* addr) :
                m_address(addr), m_enabled(false) {}
    };
}
