#include <KeybindManager.hpp>

USE_LILAC_NAMESPACE();

bool KeybindManager::init() {
    this->loadDefaultKeybinds();
    return true;
}

KeybindManager::KeybindManager() {
    this->init();
}

KeybindManager::~KeybindManager() {}

KeybindManager* KeybindManager::get() {
    static auto g_manager = new KeybindManager;
    return g_manager;
}

bool KeybindManager::addKeybindAction(
    Mod* owner,
    KeybindAction const& ogAction,
    KeybindList   const& defaults,
    keybind_action_id    const& insertAfter
) {
    auto action = ogAction.copy();
    if (!action) return false;
    if (this->m_mActions.count(action->id)) {
        delete action;
        return false;
    }
    action->defaults = defaults;
    action->owner    = owner;
    m_mActions.insert({ action->id, action });
    for (auto const& category : action->categories) {
        if (!this->m_mCategoryInfo.count(category)) {
            this->m_mCategoryInfo.insert({ category, 1 });
        } else {
            this->m_mCategoryInfo[category]++;
        }
    }
    if (this->m_mLoadedBinds.count(action->id)) {
        for (auto const& bind : this->m_mLoadedBinds[action->id]) {
            this->addKeybind(action->id, bind);
        }
    } else {
        for (auto const& bind : defaults) {
            this->addKeybind(action->id, bind);
        }
    }
    return true;
}

bool KeybindManager::removeKeybindAction(
    Mod* mod, keybind_action_id const& actionID
) {
    if (this->m_mActions.count(actionID)) {
        auto action = this->m_mActions[actionID];
        if (action->owner != mod) {
            return false;
        }
        this->clearKeybinds(actionID);
        for (auto const& category : action->categories) {
            this->m_mCategoryInfo[category]--;
            if (this->m_mCategoryInfo[category] <= 0) {
                this->m_mCategoryInfo.erase(category);
            }
            this->m_mActions.erase(actionID);
        }
        delete action;
        return true;
    }
    return false;
}

void KeybindManager::addKeybind(
    keybind_action_id const& id,
    Keybind const& bind
) {
    if (this->m_mKeybinds.count(bind) && this->m_mKeybinds[bind].size()) {
        this->m_mKeybinds[bind].push_back(id);
    } else {
        this->m_mKeybinds[bind] = { id };
    }
}

void KeybindManager::removeKeybind(
    keybind_action_id const& id,
    Keybind const& bind
) {
    if (this->m_mKeybinds.count(bind) && this->m_mKeybinds[bind].size()) {
        vector_erase<keybind_action_id>(this->m_mKeybinds[bind], [id](keybind_action_id const& kid) -> bool {
            return kid == id;
        });
    }
}

void KeybindManager::replaceKeybind(
    keybind_action_id const& id,
    Keybind const& bind,
    Keybind const& replBind
) {
    this->removeKeybind(id, bind);
    this->addKeybind(id, replBind);
}

void KeybindManager::clearKeybinds(
    keybind_action_id const& action
) {
    for (auto & [key, vals] : m_mKeybinds) {
        auto ix = 0;
        for (auto & val : vals) {
            if (val == action) {
                vals.erase(vals.begin() + ix);
                ix--;
                if (!vals.size())
                    m_mKeybinds.erase(key);
            }
            ix++;
        }
    }
}

void KeybindManager::clearActions(
    Keybind const& bind
) {
    if (bind.key != KEY_None && m_mKeybinds.count(bind))
        m_mKeybinds.erase(bind);
}

void KeybindManager::resetToDefault(keybind_action_id const& actionID, KeybindResetFlags resetType) {
    auto action = this->m_mActions[actionID];
    RepeatableAction* mod = nullptr;
    if (
        (resetType & kfKeybindResetModifierSettings) &&
        (mod = dynamic_cast<RepeatableAction*>(action))
    ) {
        // do the other resets
        mod->repeatChanged = false;
    }
    if (resetType & kfKeybindResetAll) {
        resetType |= kfKeybindResetWithDefaults;
        resetType |= kfKeybindResetWithoutDefaults;
    }
    if (
        (
            (resetType & kfKeybindResetWithDefaults) ||
            (resetType & kfKeybindResetWithoutDefaults)
        ) &&
        this->m_mActions.count(actionID)
    ) {
        if (!(
            (resetType & kfKeybindResetWithoutDefaults) ||
            action->defaults.size()
        )) return;

        this->clearKeybinds(actionID);
        for (auto const& bind : action->defaults) {
            this->addKeybind(actionID, bind);
        }
    }
}

void KeybindManager::resetAllToDefault(KeybindResetFlags resetType) {
    for (auto const& [action, _] : this->m_mActions) {
        this->resetToDefault(action, resetType);
    }
}

decltype(KeybindManager::m_mKeybinds) KeybindManager::getAllKeybinds() {
    return this->m_mKeybinds;
}

KeybindActionList KeybindManager::getAllActions(
    keybind_category_id const& categoryFilter
) {
    if (!categoryFilter) {
        return map_values(this->m_mActions);
    }
    return map_select_all<keybind_action_id, KeybindAction*>(
        this->m_mActions,
        [categoryFilter](KeybindAction* action) -> bool {
            return action->inCategory(categoryFilter);
        }
    );
}

KeybindActionList KeybindManager::getAllActionsForKeybind(
    keybind_category_id const& category, 
    Keybind const& bind
) {
    if (!this->m_mKeybinds.count(bind)) {
        return {};
    }
    KeybindActionList res;
    for (auto & actionID : m_mKeybinds[bind]) {
        auto action = this->m_mActions[actionID];
        if (action->inCategory(category)) {
            res.push_back(action);
        }
    }
    return res;
}

KeybindList KeybindManager::getKeybindsForAction(keybind_action_id const& action) {
    KeybindList res;
    for (auto & [key, vals] : m_mKeybinds) {
        for (auto & val : vals) {
            if (val == action) {
                res.insert(key);
            }
        }
    }
    return res;
}

bool KeybindManager::handleKeyEvent(
    keybind_category_id const& category,
    Keybind             const& rbind,
    CCNode*                    context,
    bool                       down
) {
    auto bind = rbind;
    if (!down) {
        bind.modifiers = this->m_nPreviousModifiers;
    }
    this->m_nPreviousModifiers = bind.modifiers;
    if (!this->m_mKeybinds.count(bind)) {
        return false;
    }
    for (auto const& id : this->m_mKeybinds[bind]) {
        auto action = this->m_mActions[id];
        if (action->inCategory(category)) {
            if (down) {
                if (dynamic_cast<RepeatableAction*>(action)) {
                    this->m_mRepeat.insert({ id, { 0.f, context, category }});
                }
            } else {
                this->m_mRepeat.erase(id);
            }
            if (this->invokeAction(action, category, context, down)) {
                return true;
            }
        }
    }
    return false;
}

std::vector<keybind_category_id> KeybindManager::getAllCategories() const {
    return map_keys(this->m_mCategoryInfo);
}

int KeybindManager::getActionCountInCategory(keybind_category_id const& id) {
    return this->m_mCategoryInfo[id];
}

KeybindActionList KeybindManager::getAllActionsInCategory(keybind_category_id const& id) const {
    KeybindActionList res;
    for (auto const& [_, action] : this->m_mActions) {
        if (action->inCategory(id)) {
            res.push_back(action);
        }
    }
    return res;
}

bool KeybindManager::invokeAction(keybind_action_id const& id, CCNode* context, bool down) {
    if (this->m_mActions.count(id)) {
        auto action = dynamic_cast<TriggerableAction*>(this->m_mActions[id]);
        if (action) {
            return action->invoke(context, down);
        }
    }
    return false;
}

bool KeybindManager::invokeAction(keybind_action_id const& id, keybind_category_id const& category, CCNode* context, bool down) {
    if (this->m_mActions.count(id)) {
        auto action = dynamic_cast<TriggerableAction*>(this->m_mActions[id]);
        if (action) {
            return action->invoke(context, category, down);
        }
    }
    return false;
}

bool KeybindManager::invokeAction(KeybindAction* action, CCNode* context, bool down) {
    auto trigger = dynamic_cast<TriggerableAction*>(action);
    if (trigger) {
        return trigger->invoke(context, down);
    }
    return false;
}

bool KeybindManager::invokeAction(KeybindAction* action, keybind_category_id const& category, CCNode* context, bool down) {
    auto trigger = dynamic_cast<TriggerableAction*>(action);
    if (trigger) {
        return trigger->invoke(context, category, down);
    }
    return false;
}

bool KeybindManager::isModifierPressed(keybind_action_id const& id) {
    auto binds = this->getKeybindsForAction(id);
    bool res = false;
    for (auto const& bind : binds) {
        if (bind.key != KEY_None) {
            if (!this->m_vPressedKeys.count(bind.key)) {
                res = false;
            }
        } else if (bind.mouse != kMouseButtonNone) {
            if (!this->m_vPressedMice.count(bind.mouse)) {
                res = false;
            }
        }

        auto kbd = CCDirector::sharedDirector()->getKeyboardDispatcher();

        if (
            static_cast<bool>(bind.modifiers & bind.kmControl) &&
            !kbd->getControlKeyPressed()
        ) res = false;

        if (
            static_cast<bool>(bind.modifiers & bind.kmCommand) &&
            !kbd->getCommandKeyPressed()
        ) res = false;

        if (
            static_cast<bool>(bind.modifiers & bind.kmShift) &&
            !kbd->getShiftKeyPressed()
        ) res = false;

        if (
            static_cast<bool>(bind.modifiers & bind.kmAlt) &&
            !kbd->getAltKeyPressed()
        ) res = false;

        if (res) return true;
    }
    return false;
}

void KeybindManager::handleRepeats(float deltaTime) {
    for (auto & [actionID, info] : this->m_mRepeat) {
        info.time += deltaTime * 1000.f; // ms
        auto action = this->isRepeatableAction(actionID);
        if (action) {
            if (action->repeat) {
                if ((info.time - action->repeatStart) >= 0) {
                    info.time -= action->repeatInterval;
                    this->invokeAction(action->id, info.category, info.context, true);
                }
            }
        }
    }
}

RepeatableAction* KeybindManager::isRepeatableAction(keybind_action_id const& id) {
    return dynamic_cast<RepeatableAction*>(this->m_mActions[id]);
}

void KeybindManager::addCategory(keybind_category_id const& id, std::string const& name) {
    this->m_mCategoryNames.insert({ id, name });
}

void KeybindManager::removeCategory(keybind_category_id const& id) {
    this->m_mCategoryNames.erase(id);
}

std::string KeybindManager::getCategoryName(keybind_category_id const& id) {
    if (this->m_mCategoryNames.count(id)) {
        return this->m_mCategoryNames[id];
    }
    return id;
}

void KeybindManager::registerKeyPress(cocos2d::enumKeyCodes key, bool down) {
    if (down) {
        this->m_vPressedKeys.insert(key);
    } else {
        this->m_vPressedKeys.erase(key);
    }
}

void KeybindManager::registerMousePress(MouseButton btn, bool down) {
    if (down) {
        this->m_vPressedMice.insert(btn);
    } else {
        this->m_vPressedMice.erase(btn);
    }
}

bool KeybindManager::keyIsModifier(cocos2d::enumKeyCodes code) {
    return
        code == KEY_Control ||
        code == KEY_Shift   ||
        code == KEY_Alt;
}