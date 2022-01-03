#pragma once

// ???? why is this here

#include <lilac.hpp>

USE_LILAC_NAMESPACE();

template<auto Func>
static uintptr_t addressOf();

#ifdef LILAC_IS_WINDOWS

template<>
static uintptr_t addressOf<&EditorUI::keyDown>() {
    return as<uintptr_t>(gd_base + 0x91a30);
}

template<>
static uintptr_t addressOf<&EditorUI::keyUp>() {
    return as<uintptr_t>(gd_base + 0x92180);
}

template<>
static uintptr_t addressOf<&UILayer::keyUp>() {
    return as<uintptr_t>(gd_base + 0x25fa10);
}

template<>
static uintptr_t addressOf<&UILayer::keyDown>() {
    return as<uintptr_t>(gd_base + 0x25f890);
}

#endif
