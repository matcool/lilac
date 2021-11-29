#pragma once

#include "Keybind.hpp"

namespace lilac {
    class Mod;

    struct LILAC_DLL keybind_category_id {
        std::string m_value;
        keybind_category_id();
        keybind_category_id(std::string const&);
        keybind_category_id(const char*);
        ~keybind_category_id();
        const char* c_str() const;
        bool operator==(keybind_category_id const&) const;
        operator int() const;
        operator std::string() const;
        keybind_category_id operator=(std::string const&);
    };
}

namespace std {
    template<>
    struct hash<lilac::keybind_category_id> {
        LILAC_DLL std::size_t operator()(lilac::keybind_category_id const&) const;
    };
}

namespace lilac {
    using stop_propagation = bool;

    /**
     * An action that can be bound to a 
     * keybind.
     * @struct KeybindAction
     */
    struct LILAC_DLL KeybindAction {
        /**
         * Mod that created this action. Cannot 
         * be nullptr; if ít is, the action will 
         * not be added and an intercontinental 
         * ballistic missile is promptly sent 
         * to your current coordinates.
         */
        Mod* owner;
        /**
         * ID of the action. Not case-sensitive 
         * nor whitespace-sensitive. Usually in 
         * the format `owner.category.name`, 
         * i.e. `gd.play.pause`. Do NOT 
         * add keybinds whose IDs start 
         * with `gd` or `lilac`; they are 
         * reserved for GD and lilac. If the ID 
         * conflicts with an already set action, 
         * the another action is not added.
         */
        keybind_action_id id                                = "";
        /**
         * Categories this keybind belongs in. 
         * This means that when someone calls 
         * `KeybindManager::handleKeyEvent`, 
         * if this keybind is in the category, 
         * it will be executed if the set bind(s) 
         * is pressed. The first category is 
         * what the action is shown under in 
         * the in-game GUI. Default category 
         * options are KB_PLAY_CATEGORY, 
         * KB_EDITOR_CATEGORY and 
         * KB_GLOBAL_CATEGORY.
         */
        std::unordered_set<keybind_category_id> categories;
        /**
         * Human-readable name of the action.
         */
        std::string name;
        /**
         * Sub-category for the action.
         */
        std::string subcategory                             = "";
        /**
         * Action description.
         */
        std::string description                             = "";
        /**
         * Default keybinds for this action.
         */
        KeybindList defaults;

        bool operator==(KeybindAction const&) const;

        bool inCategory(keybind_category_id const& category) const;

        virtual ~KeybindAction();
        virtual KeybindAction* copy() const;
    };

    /**
     * A keybind action that is a modifier, 
     * i.e. doesn't do anything when pressed 
     * but other parts of the code may check 
     * if the modifier is pressed. Can be 
     * bound to `modifiers`, or 
     * `modifiers + key`, or `key`.
     * @struct KeybindModifier
     */
    struct LILAC_DLL KeybindModifier : public KeybindAction {
        KeybindModifier() = delete;
        KeybindModifier(
            std::string         const& name,
            keybind_action_id   const& id,
            keybind_category_id const& category,
            std::string         const& subcategory = "",
            std::string         const& description = ""
        );
        KeybindModifier(
            std::string         const& name,
            keybind_action_id   const& id,
            decltype(categories)const& categories,
            std::string         const& subcategory = "",
            std::string         const& description = ""
        );
        virtual ~KeybindModifier();
    };

    /**
     * A keybind action that is a single 
     * action ran when you press/release the 
     * keybind. 
     * May be bound to `modifiers + key` or 
     * `key`.
     * @struct TriggerableAction
     */
    struct LILAC_DLL TriggerableAction : public KeybindAction {
        std::function<bool(cocos2d::CCNode*, bool)> action = nullptr;
        std::function<bool(cocos2d::CCNode*, keybind_category_id const&, bool)> actionWithID = nullptr;

        virtual stop_propagation invoke(cocos2d::CCNode* node, bool down) const;
        virtual stop_propagation invoke(
            cocos2d::CCNode* node,
            keybind_category_id const& id,
            bool down
        ) const;

        TriggerableAction() = delete;
        TriggerableAction(
            std::string         const& name,
            keybind_action_id   const& id,
            keybind_category_id const& category,
            decltype(action)           action
        );
        TriggerableAction(
            std::string         const& name,
            keybind_action_id   const& id,
            keybind_category_id const& category,
            decltype(action)           action,
            std::string         const& description
        );
        TriggerableAction(
            std::string         const& name,
            keybind_action_id   const& id,
            keybind_category_id const& category,
            decltype(actionWithID)     action,
            std::string         const& description
        );
        TriggerableAction(
            std::string         const& name,
            keybind_action_id   const& id,
            std::unordered_set<keybind_category_id> const& categories,
            decltype(actionWithID)     action,
            std::string         const& description
        );
        TriggerableAction(
            std::string         const& name,
            keybind_action_id   const& id,
            keybind_category_id const& category,
            std::string         const& subcategory,
            decltype(action)           action,
            std::string         const& description
        );
        TriggerableAction(
            std::string         const& name,
            keybind_action_id   const& id,
            keybind_category_id const& category,
            std::string         const& subcategory,
            decltype(actionWithID)     action,
            std::string         const& description
        );
        virtual ~TriggerableAction();
        virtual KeybindAction* copy() const override;
    };

    /**
     * A keybind action that is a single 
     * action ran when you press the keybind. 
     * May be bound to `modifiers + key` or 
     * `key`. If the keybind is held down, 
     * the action will be repeated until 
     * released.
     * @struct RepeatableAction
     */
    struct LILAC_DLL RepeatableAction : public TriggerableAction {
        bool repeatChanged  = false;
        bool repeat         = true;
        int  repeatInterval = 100;
        int  repeatStart    = 300;

        using TriggerableAction::TriggerableAction;

        virtual ~RepeatableAction();
        virtual KeybindAction* copy() const override;
    };

    using KeybindActionList = std::vector<KeybindAction*>;

    /**
     * Default category for making play keybinds.
     */
    constexpr const char* KB_PLAY_CATEGORY      = "gd.play";
    /**
     * Default category for making editor keybinds.
     */
    constexpr const char* KB_EDITOR_CATEGORY    = "gd.editor";
    /**
     * Default category for making global keybinds.
     */
    constexpr const char* KB_GLOBAL_CATEGORY    = "gd.global";

    constexpr const char* KB_SUBCATEGORY_MOVE   = "move";
    constexpr const char* KB_SUBCATEGORY_GLOBAL = "global";
    constexpr const char* KB_SUBCATEGORY_SELECT = "select";
    constexpr const char* KB_SUBCATEGORY_UI     = "ui";
    constexpr const char* KB_SUBCATEGORY_MODIFY = "modify";

    LILAC_DLL std::ostream& operator<<(std::ostream& stream, keybind_category_id const& id);
}
