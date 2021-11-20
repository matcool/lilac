#pragma once

#include "Macros.hpp"

#ifdef LILAC_IS_WIN32

#include <Windows.h>

namespace lilac {
    struct PlatformInfo {
        HMODULE m_hmod;
    };
}

#endif
