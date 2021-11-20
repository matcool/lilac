#pragma once

#include "../../lilac/Macros.hpp"

#ifdef LILAC_IS_WIN32

#include <Windows.h>

struct GLFWwindow;

namespace lilac::utils {
    HWND glfwGetWin32Window(GLFWwindow* window);
    HWND getGDHWND();
}

#endif
