#ifndef __INCLUDE_LILAC_LOADER_H__
#define __INCLUDE_LILAC_LOADER_H__

#include <string>
#include <vector>
#include "Mod.h"

namespace lilac {

    class Mod;

    class Loader {
    public:
        using mod_list = std::vector<Mod*>;

    protected:
        mod_list m_mods;
        
        /**
        * Load mods for a specified load time
        */
        void enable_mods(ModLoadTime time);

    public:
        /**
        * Get shared Loader instance
        * @returns Loader instance
        */
        static Loader& shared();
        
        /**
        * Get list of installed mods
        * @returns List of mods
        */
        mod_list get_mods();

        /**
        * Check if another mod of a given name is loaded
        * @param id The human-readable ID string of the mod
        * @returns True if the mod is loaded, false if not
        */
        bool is_mod_loaded(const std::string& id);
    };

}

#endif
