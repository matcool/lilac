#include "hook.hpp"

bool __fastcall CCKeyboardDispatcher_dispatchKeyboardMSG(
    CCKeyboardDispatcher* self,
    void*,
    enumKeyCodes key,
    bool down
) {
    KeybindManager::get()->registerKeyPress(key, down);
    if (!KeybindManager::keyIsModifier(key)) {
        if (KeybindManager::get()->handleKeyEvent(
            KB_GLOBAL_CATEGORY,
            Keybind(key),
            CCDirector::sharedDirector()->getRunningScene(),
            down
        )) return true;
    }
    return self->dispatchKeyboardMSG(key, down);
}
static CreateHookMod<&CCKeyboardDispatcher_dispatchKeyboardMSG>$cckddkmsg(
    "libcocos2d.dll",
    "?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QAE_NW4enumKeyCodes@2@_N@Z"
);

void __fastcall CCScheduler_update(CCScheduler* self, void*, float dt) {
    KeybindManager::get()->handleRepeats(dt);
    return self->update(dt);
}
static CreateHookMod<&CCScheduler_update>$ccsu(
    "libcocos2d.dll",
    "?update@CCScheduler@cocos2d@@UAEXM@Z"
);

class CCEGLView_hook : public CCEGLView {
public:
    static void __fastcall onGLFWMouseCallBack(
        CCEGLView_hook* self, void*, GLFWwindow* wnd, int btn, int pressed, int z
    ) {
        KeybindManager::get()->registerMousePress(
            static_cast<MouseButton>(btn), pressed
        );
        return self->CCEGLView::onGLFWMouseCallBack(wnd, btn, pressed, z);
    }
};
static CreateHookMod<&CCEGLView_hook::onGLFWMouseCallBack>$ccevogmc(
    "libcocos2d.dll",
    "?onGLFWMouseCallBack@CCEGLView@cocos2d@@IAEXPAUGLFWwindow@@HHH@Z"
);
