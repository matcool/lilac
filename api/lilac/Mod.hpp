#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include "Result.hpp"
#include "VersionInfo.hpp"
#include <string_view>
#include <vector>
#include <unordered_map>

class Lilac;

namespace lilac {
    #pragma warning(disable: 4251) // I will use unordered_map and
                                   // no amount of compiler warnings
                                   // can stop me >:)
                                   
    struct PlatformInfo;

    class Hook;
    class Loader;
    class LogStream;

    enum class DependencyType {
        Optional,
        Required,
        CustomLoader,
    };

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
            using Dependency = std::tuple<DependencyType, bool>;

            /**
             * Path of the loaded file on a
             * given platform
             */
            std::string_view m_path;
            /**
             * Platform-specific info
             */
            PlatformInfo* m_platformInfo;
            /**
             * Hooks owned by this mod
             */
            std::vector<Hook*> m_hooks;
            /**
             * Whether the mod is enabled or not
             */
            bool m_enabled;
            /**
             * Dependencies
             */
            std::unordered_map<std::string, Dependency> m_dependencies;

            /**
             * Cleanup platform-related info
             */
            void platformCleanup();

            /**
             * Check whether or not this Mod
             * depends on another mod
             */
            bool depends(std::string_view const& id) const;
            
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
        private:
            void disableBase();
            void enableBase();

        protected:
            /**
             * Mod Version. Should follow semver.
             */
            VersionInfo m_version;
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
            std::string_view m_description = "";
            /**
             * Free-form detailed description
             * of the mod. Do not write credits
             * here; use `m_credits` instead.
             */
            std::string_view m_details = "";
            /**
             * Free-form list of credits.
             */
            std::string_view m_credits = "";

            /**
             * Set the dependencies for this mod.
             * The value is a map of keys containing
             * the ID of the mod and values of
             * type `DependencyType` specifying
             * whether the dependency is required,
             * an Expnasion or optional.
             */
            void setDependencies(std::unordered_map<std::string, DependencyType> const& dependencies);

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

            // no copying
            Mod(Mod const&)           = delete;
            Mod operator=(Mod const&) = delete;
            
            /**
             * Protected constructor/destructor
             */
            Mod();
            virtual ~Mod();

            friend class Loader;
            friend class Lilac;

        public:
            /* @region getters */
            std::string_view getID()         const;
            std::string_view getName()       const;
            std::string_view getDeveloper()  const;
            std::string_view getDescription()const;
            std::string_view getDetails()    const;
            std::string_view getCredits()    const;
            std::string_view getPath()       const;
            VersionInfo      getVersion()    const;
            bool             isEnabled()     const;

            /**
             * Log to lilac's integrated console / 
             * the platform debug console.
             * @returns Reference to log stream. Make sure 
             * to end your logging with lilac::endl.
             */
            LogStream& log();

            /**
             * Throw an error. Equivalent to 
             * ```
             * Mod::log() << Severity::severity << info << lilac::endl.
             * ```
             * @param info Error infomration
             * @param severity Error severity
             */
            void throwError(
                std::string_view const& info,
                Severity severity
            );

            /**
             * Get all hooks owned by this Mod
             * @returns Vector of hooks
             */
            std::vector<Hook*> getHooks() const;

            /**
             * Create a hook at an address. Call the original 
             * function by calling the original function – 
             * no trampoline needed
             * @param address The absolute address of 
             * the function to hook, i.e. gd_base + 0xXXXX
             * @param detour Pointer to your detour function
             * @returns Successful result containing the 
             * Hook handle, errorful result with info on 
             * error
             */
            Result<Hook*> addHook(void* address, void* detour);

            /**
             * Create a hook at an address with a detour
             * and trampoline
             * @param address The absolute address of 
             * the function to hook, i.e. gd_base + 0xXXXX
             * @param detour Pointer to your detour function
             * @param trampoline Pointer to a function pointer 
             * used to call the original
             * @returns Successful result containing the 
             * Hook handle, errorful result with info on 
             * error
             */
            Result<Hook*> addHook(void* address, void* detour, void** trampoline);

            /**
             * Enable a hook owned by this Mod
             * @returns Successful result on success, 
             * errorful result with info on error
             */
            Result<> enableHook(Hook* hook);

            /**
             * Disable a hook owned by this Mod
             * @returns Successful result on success, 
             * errorful result with info on error
             */
            Result<> disableHook(Hook* hook);

            /**
             * Remove a hook owned by this Mod
             * @returns Successful result on success, 
             * errorful result with info on error
             */
            Result<> removeHook(Hook* hook);
    };

    #pragma warning(default: 4251)
}
