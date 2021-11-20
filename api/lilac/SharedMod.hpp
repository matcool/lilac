#pragma once

#include "Mod.hpp"

namespace lilac {
    /**
     * An SharedMod is a special type of Mod 
     * that is meant to provide shared utilities 
     * / features for a lot of child mods, 
     * which may be selected individually 
     * by the end-user.   
     * 
     * SharedMods can do anything normal 
     * mods can, but in addition will receive 
     * events for when new mods that have 
     * listed the SharedMod as a dependency 
     * are loaded. This enables them to expand 
     * lilac's mod API to fit more niche and 
     * specialized functions.
     * 
     * SharedMods may also reject the loading
     * of a mod, if it doesn't fit the
     * SharedMod's requirements, for example
     * the Mod doesn't inherit from the
     * SharedMod's extended Mod interface.
     * 
     * @class SharedMod
     */
    class LILAC_DLL SharedMod : public Mod {
        protected:
            std::string_view m_customExtension = "";

            /**
             * Called when a new mod that lists
             * this SharedMod as a dependency
             * is loaded.
             * @param mod The loaded mod's handle
             * @returns A truthy result if the mod's fine,
             * and an errorful result with error information
             * explaining why the mod was rejected
             */
            virtual Result<> loadMod(Mod* mod)      = 0;
            /**
             * Called when a mod that lists
             * this SharedMod as a dependency
             * is enabled.
             * @param mod The mod's handle
             */
            virtual void enableMod(Mod* mod)    = 0;
            /**
             * Called when a mod that lists
             * this SharedMod as a dependency
             * is disabled.
             * @param mod The mod's handle
             */
            virtual void disableMod(Mod* mod)   = 0;
            /**
             * Called when a mod that lists
             * this SharedMod as a dependency
             * is unloaded.
             * @param mod The mod's handle
             */
            virtual void unloadMod(Mod* mod)    = 0;
    };
}
