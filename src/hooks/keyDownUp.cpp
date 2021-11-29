#include "hook.hpp"

void __fastcall UILayer_keyDown(UILayer* self, edx_t, enumKeyCodes key) {
    KeybindManager::get()->handleKeyEvent(
        KB_PLAY_CATEGORY, Keybind(key), PlayLayer::get(), true
    );
}
static CreateHookAddr<&UILayer_keyDown>$uilkd(gd_base + 0x25f890);

void __fastcall UILayer_keyUp(UILayer* self, edx_t, enumKeyCodes key) {
    KeybindManager::get()->handleKeyEvent(
        KB_PLAY_CATEGORY, Keybind(key), PlayLayer::get(), false
    );
}
static CreateHookAddr<&UILayer_keyUp>$uilku(gd_base + 0x25fa10);

void __fastcall EditorUI_keyDown(EditorUI* self_, edx_t, enumKeyCodes key) {
    auto self = as<EditorUI*>(as<uintptr_t>(self_) - 0xf8);
    KeybindManager::get()->handleKeyEvent(
        KB_EDITOR_CATEGORY, Keybind(key), self, true
    );
}
static CreateHookAddr<&EditorUI_keyDown>$euikd(gd_base + 0x91a30);

void __fastcall EditorUI_keyUp(EditorUI* self_, edx_t, enumKeyCodes key) {
    auto self = as<EditorUI*>(as<uintptr_t>(self_) - 0xf8);
    KeybindManager::get()->handleKeyEvent(
        KB_EDITOR_CATEGORY, Keybind(key), self, false
    );
}
static CreateHookAddr<&EditorUI_keyUp>$euiku(gd_base + 0x92180);
