#pragma once

#include "../lilac/macros.hpp"

#ifdef LILAC_IS_WINDOWS

#include <Windows.h>

struct GLFWwindow;

namespace lilac::utils {
    HWND glfwGetWin32Window(GLFWwindow* window);
    HWND getGDHWND();
}

#endif
