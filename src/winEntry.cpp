// "winEntry.cpp" i hate this filename

#include "entry.hpp"
#include <Windows.h>

DWORD WINAPI loadThread(void* arg) {
	return lilacEntry();
}

BOOL WINAPI DllMain(HINSTANCE lib, DWORD reason, LPVOID) {
	if (reason == DLL_PROCESS_ATTACH) {
		// not sure why this is here.
		// i just saw other mods doing it
		// and thought "sure".
		// if someone does know, please update
		// these comments.
		DisableThreadLibraryCalls(lib);

		// loading thread
		if (auto handle = CreateThread(0, nullptr, loadThread, lib, 0, nullptr))
			CloseHandle(handle);
	}
	return TRUE;
}

