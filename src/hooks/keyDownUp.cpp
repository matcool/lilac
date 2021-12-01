#include "hook.hpp"

void LILAC_FASTCALL UILayer_keyDown(UILayer* self, EDX() enumKeyCodes key) {
    KeybindManager::get()->handleKeyEvent(
        KB_PLAY_CATEGORY, Keybind(key), PlayLayer::get(), true
    );
}
static CreateHookAddr<&UILayer_keyDown>$UILayer_keyDown(addressOf<&UILayer::keyDown>());

void LILAC_FASTCALL UILayer_keyUp(UILayer* self, EDX() enumKeyCodes key) {
    KeybindManager::get()->handleKeyEvent(
        KB_PLAY_CATEGORY, Keybind(key), PlayLayer::get(), false
    );
}
CREATE_HOOK(UILayer, keyUp);

void LILAC_FASTCALL EditorUI_keyDown(EditorUI* self_, EDX() enumKeyCodes key) {
    auto self = as<EditorUI*>(as<uintptr_t>(self_) - 0xf8);
    KeybindManager::get()->handleKeyEvent(
        KB_EDITOR_CATEGORY, Keybind(key), self, true
    );
}
CREATE_HOOK(EditorUI, keyDown);

void LILAC_FASTCALL EditorUI_keyUp(EditorUI* self_, EDX() enumKeyCodes key) {
    auto self = as<EditorUI*>(as<uintptr_t>(self_) - 0xf8);
    KeybindManager::get()->handleKeyEvent(
        KB_EDITOR_CATEGORY, Keybind(key), self, false
    );
}
CREATE_HOOK(EditorUI, keyUp);
