set(SOURCES ${SOURCES}
	src/entry.cpp
	src/hooks/dispatchMsg.cpp
	src/hooks/keyDownUp.cpp
	src/keybinds/defaultBinds.cpp
	src/keybinds/Keybind.cpp
	src/keybinds/KeybindAction.cpp
	src/keybinds/KeybindManager.cpp
	src/utils/gd/convert.cpp
	src/utils/gd/node.cpp
	src/utils/gd/stream.cpp
	src/utils/gd/win32.cpp
	src/lilac/internal/CApiMod.cpp
	src/lilac/internal/Internal.cpp
	src/lilac/internal/InternalMod.cpp
	src/lilac/platform/win32/Loader.cpp
	src/lilac/platform/win32/Mod.cpp
	src/lilac/CLIManager.cpp
	src/lilac/Hook.cpp
	src/lilac/load.cpp
	src/lilac/Loader.cpp
	src/lilac/Log.cpp
	src/lilac/Mod.cpp

	PARENT_SCOPE
)
