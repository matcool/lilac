#include <Loader.hpp>
#include <Windows.h>
#include <Internal.hpp>

USE_LILAC_NAMESPACE();

DWORD WINAPI load_thread(LPVOID hModule) {
    // set up loader, load mods, etc.
    Loader::get()->setup();

    // debugging console
    #ifdef LILAC_PLATFORM_CONSOLE
    Lilac::get()->setupPlatformConsole();
    Lilac::get()->awaitPlatformConsole();
    Lilac::get()->closePlatformConsole();
    #endif

    return 0;
}

BOOL WINAPI DllMain(
    HINSTANCE hModule,
    DWORD fdwReason,
    LPVOID lpReserved
) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:

            // not sure why this is here.
            // i just saw other mods doing it
            // and thought "sure".
            // if someone does know, please update
            // these comments.
            DisableThreadLibraryCalls(hModule);

            // loading thread
            HANDLE _ = CreateThread(0, 0, load_thread, hModule, 0, nullptr);
            if (_) CloseHandle(_);

            break;
    }
    return TRUE;
}
