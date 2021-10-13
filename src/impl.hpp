#pragma once
#include <lilac.hpp>
#include <memory>
#include <unordered_map>

namespace lilac::impl {
	class Mod {
	public:
		std::string name;
		std::unordered_map<lilac::CallbackEvent, std::function<void()>> callbacks;
	};

	class Loader {
	public:
		std::unordered_map<const void*, lilac::Mod*> mods;
	public:
		static Loader& instance();
		lilac::Mod* register_mod(const void*);
		void on_finish_launching();
		void on_load();
	};
}