#ifndef __INCLUDE_LILAC_HOOK_H__
#define __INCLUDE_LILAC_HOOK_H__

#include <string>
#include "Types.h"

namespace lilac {

    class Loader;
    class Mod;

    class Patch {
    protected:
        Mod* m_owner;
        l_addr m_address;
        l_byte_array m_patched_bytes;
        l_byte_array m_original_bytes;
    
        friend class Mod;
        friend class Loader;

    public:
        /**
        * Get the address of the patch
        * @returns Address of the patch
        */
        l_addr get_addr();

        /**
        * Enable the patch
        */
        void commit();

        /**
        * Disable the patch
        */
        void unpatch();
    };

    class Hook {
    protected:
        Mod* m_owner;
        l_addr m_address;
        bool m_enabled;

        friend class Mod;
        friend class Loader;

        Hook() = delete;
        Hook(Mod* owner, l_addr address);
        ~Hook() = default;

    public:
        /**
        * Get the address being hooked
        * @returns Address of the function in GD being hooked
        */
        l_addr get_addr();

        /**
        * Check if the hook is enabled
        * @returns True if the hook is enabled, false if not
        */
        bool is_enabled();
    };

}

#endif
