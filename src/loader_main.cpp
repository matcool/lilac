#include <Windows.h>
#include <lilac_hook.hpp>
#include <lilac.hpp>
#include "impl.hpp"
#include <fstream>
#include <iostream>

bool (__thiscall* AppDelegate_applicationDidFinishLaunching)(void*);
void (__thiscall* LoadingLayer_loadingFinished)(void*);

bool __fastcall AppDelegate_applicationDidFinishLaunching_H(void* self) {
    if (!AppDelegate_applicationDidFinishLaunching(self)) return false;
	lilac::impl::Loader::instance().on_finish_launching();
    return true;
}

void __fastcall LoadingLayer_loadingFinished_H(void* self) {
    LoadingLayer_loadingFinished(self);
    lilac::impl::Loader::instance().on_load();
}

DWORD WINAPI load(void*) {
	AllocConsole();
	static std::ofstream conout("CONOUT$", std::ios::out);
	std::cout.rdbuf(conout.rdbuf());
	// lib isnt done yet lul
    const auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
    #define assign(f, addr) f = reinterpret_cast<decltype(f)>(addr)
    assign(AppDelegate_applicationDidFinishLaunching, base + 0x3cbb0);
    assign(LoadingLayer_loadingFinished, base + 0x18c790);

	std::cout << "adding hooks" << std::endl;

    lilac::Hooks::add(AppDelegate_applicationDidFinishLaunching, AppDelegate_applicationDidFinishLaunching_H);
    lilac::Hooks::add(LoadingLayer_loadingFinished, LoadingLayer_loadingFinished_H);
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);
        if (auto handle = CreateThread(NULL, 0, load, module, 0, 0))
			CloseHandle(handle);
		else
			return FALSE;
    }
    return TRUE;
}