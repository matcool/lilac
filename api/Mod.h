#ifndef __INCLUDE_LILAC_MOD_H__
#define __INCLUDE_LILAC_MOD_H__

#include <string>
#include <optional>
#include "Hook.h"

namespace lilac {

    class Hook;
    class Loader;

    class Mod {
	protected:
		struct Metadata {
			struct Credit {
				struct Link {
					std::string platform_name;
					std::string user_tag;
				};
				
				std::string name;
				// ! Maybe this should be stored as the UID instead?
				std::string gd_name;
				std::string role_description;
				std::vector<Link> links;
			};

			std::string description;
			std::string version_string;
			int version_num;
			std::vector<Credit> credits;
			std::vector<std::string> special_thanks;
			ModLoadTime preferred_load_time;
		};

		/**
		* Name of the mod
		*/
		std::string m_name;
		/**
		* The human-readable unique ID of the mod
		* Like com.example.mod-name
		*/
		std::string m_id;
		/**
		* Metadata about the mod
		*/
		Metadata m_metadata;
		/**
		* Whether the mod has been loaded or not;
		* This should only be set by the Loader
		*/
		bool m_loaded;
		/**
		* Hooks owned by this mod
		*/
		std::vector<Hook*> m_hooks;
		/**
		* Patches owned by this mod
		*/
		std::vector<Patch*> m_patches;
		/**
		* Containers handled owned by this mod
		*/
		std::vector<TargetContainer> m_handlers;

		/**
		* Private constructors
		*/
		Mod();
		~Mod();
		/**
		* Delete the copy constructor
		*/
		Mod(const Mod&) = delete;

		friend class Loader;

		// PROTECTED MEMBER FUNCS
		// Only used by internal systems, such as the loader

		/**
		* Check if this mod wants to handle a certain
		* target container
		* @param container The container to handle
		*/
		bool will_handle(TargetContainer container);
		/**
		* Call the mod's position handler clearer.
		* @param container The container to clear.
		*/
		void handle_clear(TargetContainer container);
		/**
		* Call the mod's container getter.
		* @param container The container to get.
		* @returns The container.
		*/
		cocos2d::CCNode* handle_get(TargetContainer container);
		/**
		* Call the mod's free-position-in-container getter.
		* @param container The target container.
		* @param position The requested position in the container.
		* @param size The free space requested. Will be a size of
		* 0,0 if no space was specified, meaning the mod should
		* use the default size for that container.
		* @returns A free position in the container.
		*/
		cocos2d::CCPoint handle_pos(
			TargetContainer container,
			TargetPosition position,
			cocos2d::CCSize size
		);
	
	public:
		/**
		* Create a hook at a location
		* @param address Address to hook
		* @param detour Pointer to the detour function to be called whenever the hook is hit
		* @returns Pointer to the Hook that was created, or nullptr on error
		*/
		template<typename Hook_T>
		Hook* add_hook(l_addr address, Hook_T detour);

		/**
		* Get hook at a location
		* @param address Address of the hook
		* @returns Reference to the hook, or nullptr if none was found
		*/
		Hook* get_hook_at_addr(l_addr address);

		/**
		* Enable all of the hooks associated with this mod
		*/
		void enable_all_hooks();

		/**
		* Write bytes at an address
		* @param address Address to overwrite
		* @param bytes Bytes to write
		* @returns Pointer to Patch if succesful, nullptr on error
		*/
		Patch* patch_addr(l_addr address, l_byte_array bytes);

		/**
		* Get the name of the mod
		* @returns Name of the mod
		*/
		const std::string& get_name();

		/**
		* Get the human-readable ID of the mod
		* @returns ID of the mod
		*/
		const std::string& get_id();
    };

}

#endif
