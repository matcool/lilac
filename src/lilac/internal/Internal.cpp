#include <KeybindManager.hpp>
#include "Internal.hpp"
#include <Windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "InternalMod.hpp"
#include <Log.hpp>
#include <Loader.hpp>

Lilac::Lilac() {
    // init KeybindManager & load default keybinds
    KeybindManager::get();

    #ifdef LILAC_PLATFORM_CONSOLE
    this->setupPlatformConsole();
    #endif
}

Lilac::~Lilac() {
    this->closePlatformConsole();
    delete Loader::get();
}

Lilac* Lilac::get() {
    static auto g_lilac = new Lilac;
    return g_lilac;
}

bool Lilac::setup() {
    InternalMod::get()->setup();

    InternalMod::get()->log()
        << Severity::Debug << "Set up internal mod representation" << lilac::endl;

    InternalMod::get()->log()
        << Severity::Debug << "Loading hooks... " << lilac::endl;

    this->loadHooks();

    InternalMod::get()->log()
        << Severity::Debug << "Loaded hooks" << lilac::endl;

    return true;
}

#ifdef LILAC_IS_WIN32

void Lilac::queueConsoleMessage(LogMessage* msg) {
    this->m_logQueue.push_back(msg);
}

bool Lilac::platformConsoleReady() const {
    return m_platformConsoleReady;
}

void Lilac::platformMessageBox(const char* title, const char* info) {
    MessageBoxA(nullptr, title, info, MB_OK);
}

void Lilac::setupPlatformConsole() {
    if (m_platformConsoleReady) return;
    if (AllocConsole() == 0)    return;
    // redirect console output
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);

    m_platformConsoleReady = true;

    for (auto const& log : this->m_logQueue) {
        std::cout << log->toString(true) << "\n";
    }
}

void Lilac::awaitPlatformConsole() {
    if (!m_platformConsoleReady) return;

    std::string inp;
    getline(std::cin, inp);
    std::string inpa;
    std::stringstream ss(inp);
    std::vector<std::string> args;

    while (ss >> inpa) args.push_back(inpa);
    ss.clear();

    if (inp != "e") this->awaitPlatformConsole();
}

void Lilac::closePlatformConsole() {
    if (!m_platformConsoleReady) return;

    fclose(stdin);
    fclose(stdout);
    FreeConsole();
}

#endif

