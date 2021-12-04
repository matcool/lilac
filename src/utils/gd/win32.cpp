#include <utils/gd/include_gd.hpp>
#include <utils/platform.hpp>
#include <utils/gd/win32.hpp>

#ifdef LILAC_IS_WINDOWS

USE_LILAC_NAMESPACE();

HWND lilac::utils::glfwGetWin32Window(GLFWwindow* window) {
    static auto cocosBase = GetModuleHandleA("libcocos2d.dll");

    auto pRet = reinterpret_cast<HWND(__cdecl*)(GLFWwindow*)>(
        reinterpret_cast<uintptr_t>(cocosBase) + 0x112c10
    )(window);

    return pRet;
}

HWND lilac::utils::getGDHWND() {
    static HWND g_hwnd = nullptr;

    if (!g_hwnd) {
        auto dir = CCDirector::sharedDirector();
        if (!dir) return nullptr;
        auto opengl = dir->getOpenGLView();
        if (!opengl) return nullptr;
        auto wnd = dir->getOpenGLView()->getWindow();
        if (!wnd) return nullptr;
        g_hwnd = glfwGetWin32Window(wnd);
    }

    return g_hwnd;
}

#endif
