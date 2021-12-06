#pragma once

#include "utils/utils.hpp"

#include "keybinds/Keybind.hpp"
#include "keybinds/MouseButton.hpp"
#include "keybinds/KeybindAction.hpp"
#include "keybinds/KeybindManager.hpp"

#include "lilac/macros.hpp"
#include "lilac/Result.hpp"
#include "lilac/Hook.hpp"
#include "lilac/Log.hpp"
#include "lilac/Mod.hpp"
#include "lilac/Loader.hpp"
#include "lilac/Win32.hpp"

// restore dll-interface warnings for other mods

#pragma warning(default: 4067)
#pragma warning(default: 4251)
