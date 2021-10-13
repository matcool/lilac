#include <lilac.hpp>
#include <Windows.h>
#include <filesystem>
#include "impl.hpp"
#include <lilac_hook.hpp>
#include <iostream>

namespace lilac {
	Mod::~Mod() { delete this->impl; }

	void Mod::on(CallbackEvent event, std::function<void()> callback) {
		impl->callbacks[event] = callback;
	}

	void Mod::add_hook(const void* address, const void* hook) {
		// TODO: store the hooks in the mod themselves
		lilac::Hooks::add(address, hook);
	}

	Mod* Loader::mod_for_module(const void* module) {
		return impl::Loader::instance().mods[module];
	}

	void Mod::log(const std::string& message) {
		std::cout << "[" << this->impl->name << "] " << message << std::endl;
	}

	namespace impl {
		Loader& Loader::instance() {
			static Loader loader;
			return loader;
		}

		lilac::Mod* Loader::register_mod(const void* module) {
			auto mod = mods[module] = new lilac::Mod();
			mod->impl = new lilac::impl::Mod();
			return mod;
		}

		void Loader::on_finish_launching() {
			// pff cross platform
			const auto buffer = std::make_unique<wchar_t[]>(MAX_PATH);
			GetModuleFileNameW(GetModuleHandle(0), buffer.get(), MAX_PATH);
			const auto& gd_path = std::filesystem::path(buffer.get());
			for (const auto& entry : std::filesystem::directory_iterator(gd_path.parent_path() / "mods")) {
				const auto& path = entry.path();
				if (path.extension() == ".dll") {
					const auto handle = LoadLibraryW(path.wstring().c_str());
					std::cout << "loaded a mod " << handle << std::endl;
					const auto proc = GetProcAddress(handle, "?lilac_main@@YGXPAVMod@lilac@@@Z");
					if (proc) {
						// maybe dont pass it as an arg, use lilac::self instead
						auto mod = register_mod(reinterpret_cast<void*>(handle));
						mod->impl->name = path.filename().string();
						reinterpret_cast<void(__stdcall*)(lilac::Mod*)>(proc)(mod);
					}
				}
			}
		}

		void Loader::on_load() {
			for (auto& [_, mod] : mods) {
				const auto& el = mod->impl->callbacks.find(CallbackEvent::Load);
				std::cout << "calling the load callback for mod " << mod << " " << _ << std::endl;
				if (el != mod->impl->callbacks.end()) el->second();
			}
		}
	}
};