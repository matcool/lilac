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
#include <CLIManager.hpp>

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
    static auto inst = new Lilac;
    return inst;
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

    InternalMod::get()->addKeybindAction(TriggerableAction {
        "Yeetus Feetus",
        "lilac.yeetus",
        KB_GLOBAL_CATEGORY,
        [](auto node, bool down) -> bool {
            if (down) {
                auto count = Loader::get()->updateMods();
                FLAlertLayer::create(
                    nullptr, "yea", "OK", nullptr,
                    "woo wee " + std::to_string(count)
                )->show();
            }
            return false;
        }
    }, {{ KEY_G, Keybind::kmControl | Keybind::kmAlt }});

    return true;
}

#ifdef LILAC_IS_WINDOWS

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
}

void Lilac::awaitPlatformConsole() {
    if (!m_platformConsoleReady) return;

    for (auto const& log : this->m_logQueue) {
        std::cout << log->toString(true) << "\n";
        this->m_logQueue.clear();
    }

    std::string inp;
    getline(std::cin, inp);
    std::string inpa;
    std::stringstream ss(inp);
    std::vector<std::string> args;

    while (ss >> inpa) args.push_back(inpa);
    ss.clear();

    // CLIManager::get()->execute(args);

    if (inp == "reload") {
        Loader::get()->updateMods();
    }

    if (inp != "e") this->awaitPlatformConsole();
}

void Lilac::closePlatformConsole() {
    if (!m_platformConsoleReady) return;

    fclose(stdin);
    fclose(stdout);
    FreeConsole();
}

#endif

