#pragma once

#include "macros.hpp"

#ifdef LILAC_IS_WINDOWS

#include <Windows.h>

namespace lilac {
    struct PlatformInfo {
        HMODULE m_hmod;
    };
}

#endif
