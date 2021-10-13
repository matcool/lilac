#pragma once
#include <functional>
#include <string>

#ifdef LILAC_EXPORT
#define LILAC_DLL __declspec(dllexport)
#else
#define LILAC_DLL __declspec(dllimport)
#endif

#if defined(_WIN32)
#define lilac_main __declspec(dllexport) __stdcall lilac_main
struct _IMAGE_DOS_HEADER;
extern "C" _IMAGE_DOS_HEADER __ImageBase;
#endif

namespace lilac {
	enum class CallbackEvent {
		Load
	};
	
	namespace impl {
		class Mod;
		class Loader;
	}

	class LILAC_DLL Mod {
	private:
		friend class impl::Loader;
		impl::Mod* impl;
		~Mod();
	public:
		void on(CallbackEvent event, std::function<void()> callback);
		void add_hook(const void* addr, const void* hook);
		void log(const std::string& message);
	};

	class LILAC_DLL Loader {
	public:
		static Mod* mod_for_module(const void* module);
	};

	inline Mod* self() {
		#if defined(_WIN32)
		return Loader::mod_for_module(reinterpret_cast<void*>(&__ImageBase));
		#else
		cry
		#endif
	}

	inline void log(const std::string& message) {
		self()->log(message);
	}
}
