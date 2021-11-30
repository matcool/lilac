# Keybinds

Lilac provides an API for Mods to easily add & customize in-game keybinds. These keybinds are modifiable by the user from the in-game menu, and the Mod can also provide its own default keybinds and modify bindings at runtime.

It is important to distinguish terminology between **keybinds** and **actions**. A keybind is a shortcut (i.e. `Ctrl + C`), while an action is some function that can be binded to any amount of keybinds. Mods should primarily only add actions and may provide a set of default keybinds for them, while runtime modification of keybinds should be left up to the user. Mods can, however, change the keybinds of any action if they so please, for example in order to make a mod that lets the player jump using any key in the keyboard.

# Action Types

In code, all actions inherit from the `KeybindAction` class. Despite the class not being explicitly abstract, you should not create your instances of the base `KeybindAction` class. Instead, to add new keybind actions, it should be one of the 3 provided types.

Each action has an unique human-readable string ID, a set of categories the action belongs in, info about the keybind and a Mod that owns it. The category set is important, as when a keybind is pressed, it will execute its corresponding action if that action matches a given category ID. Default categories can be found in `api/keybinds/KeybindAction.hpp`.

Do note that if an action is in two or more categories, the context provided may be different, and therefore the action callback should be of type `actionWithID`, which provides the intended category as a third parameter. Check the third category to make sure the context is interpreted correctly. For example, if you make a playing action that can also be used in the editor, give it both `KB_PLAY_CATEGORY` and `KB_EDITOR_CATEGORY` as categories and then check which category was provided. If the category is `KB_PLAY_CATEGORY`, the context will be a pointer to the current `PlayLayer`, whereas in the editor the context will be a pointer to the current `EditorUI`.

### Default categories

 * `KB_PLAY_CATEGORY`: A playing action. When the user is playing a level, if the associated keybind is pressed, this action will be run. If the action is of type `TriggerableAction`, the context will be the current `PlayLayer`.

 * `KB_EDITOR_CATEGORY`: An editor action. When the user is in the editor, if the associated keybind is pressed, this action will be run. If the action is of type `TriggerableAction`, the context will be the current `EditorUI`.

 * `KB_GLOBAL_CATEGORY`: A global action; if the keybind is pressed in any scene, this action will be run. If the action is of type `TriggerableAction`, the context will be the currently running `CCScene`.

You can also add your own categories; see [Setting up Custom Keybinds for Your Layer](#custom-keybinds-for-your-layer).

## KeybindModifier

A keybind modifier is an action that doesn't by itself do anything, but other parts of the code can check if the modifier is currently active and change their behaviour accordingly. For example, the `Swipe Modifier` keybind is a modifier which changes the behaviour of the mouse in the editor.

Modifiers are unique in the way that they can be bound to a lone modifier key, whereas other action types require to be bound to a key or a key + modifier combo.

To check if a modifier is currently active, use `KeybindManager::isModifierPressed`.

## TriggerableAction

A triggerable action is an action that, when the keybind is pressed / released, runs a bit of code. The code has access to 2 parameters: a pointer to the `CCNode` context (usually the current scene or a relevant layer in it) and a `bool` depicting whether or not the keybind was pressed or released. You may also instead create a callback which has a third parameter of type `keybind_category_id`, which describes what category the keybind was run in. If your keybind is in multiple categories, you can use this type of callback to make sure of the type of the context.

## RepeatableAction

The same as TriggerableAction, but if the keybind is held down, after a while it will start repeating at a set rate. The waiting time, rate and whether or not to repeat may be customized by the user.

# Example: Ctrl + X for Cut in the Editor

```cpp
USE_LILAC_NAMESPACE();

// Add actions through your Mod interface
MyMod::get()->addKeybindAction(
    // Could also be RepeatableAction, but 
    // there's no point as repeating Cut 
    // makes no sense
    TriggerableAction { 
        // Humnan-readable name of the keybind
        "Cut",
        // Keybind ID
        "my_mod.edit.cut",
        // Default subcategory
        KB_SUBCATEGORY_MODIFY,
        // Action callback
        [](CCNode* context, bool down) -> stop_propagation {
            // Only run action when the keybind 
            // is pressed down
            if (down) {
                auto ui = as<EditorUI*>(context);
                // For most editor keybinds, you don't 
                // want to run them when playtesting is 
                // active as doing so can cause severe 
                // bugs and potentially mess up a level
                if (ui->m_pEditorLayer->m_ePlaybackMode !=  kPlaybackModePlaying) {
                    // Copy selected objects
                    ui->onCopy(nullptr);
                    // Delete selected objects
                    ui->onDeleteSelected(nullptr);
                }
            }
            // Return false; if we returned true, 
            // no further actions that have the 
            // same keybind would be run (i.e. 
            // propagation would be stopped; 
            // however, no need to do that)
            return false;
        },
        "Cut Selected Objects"
    },
    // Default keybind: Ctrl + X
    {{ KEY_X, Keybind::kmControl }},
    // Insert after the Copy action in the 
    // in-game list
    "gd.edit.copy"
);
```

# Custom Keybinds for Your Layer

In case you have your own layer apart from the editor and PlayLayer, where you'd like to support customizable keybinds, you need to first add your own category, add actions to that category and then on `keyDown` and `keyUp` run `KeybindManager::handleKeyEvent`.

For example, you may add a new category like `my_mod.my_custom_category`. This category does not need to be explicitly declared in the manager; it is created by simply adding an action with it set as the category. However, if you'd like to add a human-readable name to the category (the ID should be `mod_name.category`), you can use `KeybindManager::addCategory`. This is not explicitly required however; omitting it will simply mean that the user will see the category name as `my_mod.my_custom_category`.

Next, you need to make sure you've picked what the context will be. The context may be any node, but usually it's either the running scene or a relevant layer within it. For each of your action callbacks, you should cast the provided context node to the type of the node you passed in to `handleKeyEvent`.

Finally, you need to modify the current layer's `keyDown` and `keyUp` events. If you're modifying a GD scene, hook the layer's `keyDown` and `keyUp` members without calling the original; if you're cretaing your own layer, override the `keyDown` and `keyUp` virtuals. Within them, you need to run this code:

```cpp
USE_LILAC_NAMESPACE();

// It is recommended you store your own 
// category in a variable, so it's easier 
// to use and can be modified later if 
// you so wish.
constexpr const auto MY_CATEGORY = "my_mod.my_custom_category";

void MyLayer::keyDown(enumKeyCodes key) {
    KeybindManager::get()->handleKeyEvent(
        MY_CATEGORY,    // The category which any action run should belong to
        
        Keybind(key),   // Create a new keybind from the pressed key.
                        // The default Keybind constructor will automatically
                        // check & handle modifiers.

        this,           // Pointer to your desired context.

        true            // false for keyUp.
    );
}
```
