#include "hook.hpp"

// void __fastcall UILayer_keyDown(UILayer* self, edx_t, enumKeyCodes key) {
//     KeybindManager::get()->handleKeyEvent(
//         KB_PLAY_CATEGORY, Keybind(key), PlayLayer::get(), true
//     );
// }
// static CreateHookAddr<&UILayer_keyDown>$UILayer_keyDown(addressOf<&UILayer::keyDown>());

// void __fastcall UILayer_keyUp(UILayer* self, edx_t, enumKeyCodes key) {
//     KeybindManager::get()->handleKeyEvent(
//         KB_PLAY_CATEGORY, Keybind(key), PlayLayer::get(), false
//     );
// }
// CREATE_HOOK(UILayer, keyUp);

// void __fastcall EditorUI_keyDown(EditorUI* self_, edx_t, enumKeyCodes key) {
//     auto self = as<EditorUI*>(as<uintptr_t>(self_) - 0xf8);
//     KeybindManager::get()->handleKeyEvent(
//         KB_EDITOR_CATEGORY, Keybind(key), self, true
//     );
// }
// CREATE_HOOK(EditorUI, keyDown);

// void __fastcall EditorUI_keyUp(EditorUI* self_, edx_t, enumKeyCodes key) {
//     auto self = as<EditorUI*>(as<uintptr_t>(self_) - 0xf8);
//     KeybindManager::get()->handleKeyEvent(
//         KB_EDITOR_CATEGORY, Keybind(key), self, false
//     );
// }
// CREATE_HOOK(EditorUI, keyUp);
