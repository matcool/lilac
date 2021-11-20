#include "Internal.hpp"
#include <Windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

Lilac::Lilac() {
    // todo: init internal stuff like default keybinds

    #ifdef LILAC_PLATFORM_CONSOLE
    this->setupPlatformConsole();
    #endif
}

Lilac* Lilac::get() {
    static auto g_lilac = new Lilac;
    return g_lilac;
}

void Lilac::setupPlatformConsole() {
    if (AllocConsole() == 0)
        return;
    // redirect console output
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
}

void Lilac::awaitPlatformConsole() {
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
    fclose(stdin);
    fclose(stdout);
    FreeConsole();
}

