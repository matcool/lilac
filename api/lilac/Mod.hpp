#pragma once

#include "Macros.hpp"
#include "Result.hpp"
#include <string_view>
#include <vector>

namespace lilac {
    #pragma warning(disable: 4251) // I will use unordered_map and
                                   // no amount of compiler warnings
                                   // can stop me >:)
                                   
    struct PlatformInfo;

    class Hook;

    /**
     * @class ModBase
     * Base for the Mod class.
     * Contains internal members that
     * are managed by lilac. Do not
     * try to call / access properties
     * on here that don't have public
     * getters / setters in the Mod
     * class.
     */
    class LILAC_DLL ModBase {
        protected:
            std::string_view    m_path;         // Path of the loaded file on a given platform
            PlatformInfo*       m_platformInfo; // Platform-specific info
            std::vector<Hook*>  m_hooks;        // Hooks owned by this mod
            bool                m_enabled;      // Whether the mod is enabled or not

            /**
             * Cleanup platform-related info
             */
            void platformCleanup();
            
            /**
             * Low-level add hook
             */
            Result<Hook*> addHookBase(
                void* addr,
                void* detour,
                Hook* hook = nullptr
            );
            Result<Hook*> addHookBase(Hook* hook);
    };

    /**
     * @class Mod
     * Represents a Mod ingame. Inherit
     * from this class to create your own
     * mod interfaces.
     * @abstract
     */
    class LILAC_DLL Mod : ModBase {
        protected:
            /**
             * Human-readable ID of the Mod.
             * Recommended to be in the format
             * "com.developer.mod". Not
             * guaranteed to be either case-
             * nor space-sensitive. Should
             * be restricted to the ASCII
             * character set.
             */
            std::string_view m_id;
            /**
             * Name of the mod. May contain
             * spaces & punctuation, but should
             * be restricted to the ASCII
             * character set.
             */
            std::string_view m_name;
            /**
             * The name of the head developer.
             * Should be a single name, like
             * "HJfod" or "The Lilac Team".
             * If the mod has multiple
             * developers, this field should
             * be one of their name or a team
             * name, and the rest of the credits
             * should be named in `m_credits`
             * instead.
             */
            std::string_view m_developer;
            /**
             * Short description between 1 and
             * 60 characters.
             */
            std::string_view m_description  = "";
            /**
             * Free-form detailed description
             * of the mod. Do not write credits
             * here; use `m_credits` instead.
             */
            std::string_view m_details      = "";
            /**
             * Free-form list of credits.
             */
            std::string_view m_credits      = "";

            /**
             * Mod-specific setup function.
             * Initialize `m_id`, `m_name`,
             * `m_developer` and other members
             * here. Do not call this yourself,
             * lilac will call it for you.
             */
            virtual void setup() = 0;
            
            /**
             * Override to provide mod-specific
             * enabling code, such as re-initializing
             * managers. Do not manually re-create
             * hooks, patches, keybinds nor
             * anything else created through the
             * Mod handle; lilac will handle those.
             * Do not call this yourself,
             * lilac will call it for you.
             */
            virtual void enable();
            
            /**
             * Override to provide mod-specific
             * disabling code, such as de-initializing
             * managers. Do not manually disable
             * hooks, patches, keybinds nor
             * anything else created through the
             * Mod handle; lilac will handle those.
             * Do not call this yourself,
             * lilac will call it for you.
             */
            virtual void disable();
            virtual void saveData();
            virtual void loadData();

            Result<Hook*> addHookInternal(
                void* addr,
                void* detour,
                void** trampoline
            );

            // no copying
            Mod(Mod const&)           = delete;
            Mod operator=(Mod const&) = delete;
            
            Mod();
            virtual ~Mod();

        public:
            /* @region getters */
            std::string_view getID()         const;
            std::string_view getName()       const;
            std::string_view getDeveloper()  const;
            std::string_view getCredits()    const;
            std::string_view getPath()       const;
            std::string_view getDescription()const;
            bool             isEnabled()     const;

            /**
             * Get all hooks owned by this Mod
             * @returns Vector of hooks
             */
            std::vector<Hook*> getHooks() const;

            /**
             * Enable a hook owned by this Mod
             * @returns Successful result on success, errorful result with info on error
             */
            Result<> enableHook(Hook* hook);

            /**
             * Disable a hook owned by this Mod
             * @returns Successful result on success, errorful result with info on error
             */
            Result<> disableHook(Hook* hook);

            /**
             * Remove a hook owned by this Mod
             * @returns Successful result on success, errorful result with info on error
             */
            Result<> removeHook(Hook* hook);
    };

    #pragma warning(restore: 4251)
}
