#pragma once

#include "../utils/utils.hpp"
#include "MouseButton.hpp"
#include <set>
#include <unordered_set>

namespace lilac {
    LILAC_DLL std::string keyToStringFixed(cocos2d::enumKeyCodes code);

    struct LILAC_DLL Keybind {
        cocos2d::enumKeyCodes key = cocos2d::KEY_None;
        enum Modifiers : int {
            kmNone      = 0,
            kmControl   = 1,
            kmShift     = 2,
            kmAlt       = 4,
            kmCommand   = 8,
        };
        using ModifierFlags = int;
        ModifierFlags modifiers;
        MouseButton mouse = kMouseButtonNone;

        bool operator==(Keybind const&) const;
        bool operator<(Keybind const&) const;

        std::string toString() const;
        void save(DS_Dictionary*) const;

        Keybind();
        Keybind(cocos2d::enumKeyCodes);
        Keybind(cocos2d::enumKeyCodes, Modifiers);
        Keybind(cocos2d::enumKeyCodes, int);
        Keybind(Modifiers);
        Keybind(int);
        Keybind(MouseButton);
        Keybind(MouseButton, int);
        Keybind(MouseButton, Modifiers);
        Keybind(DS_Dictionary*, int version);
    };

    using KeybindList = std::unordered_set<Keybind>;

    struct LILAC_DLL keybind_action_id {
        std::string m_value;
        keybind_action_id();
        keybind_action_id(std::string const&);
        keybind_action_id(const char*);
        ~keybind_action_id();
        const char* c_str() const;
        size_t size() const;
        bool operator==(keybind_action_id const&) const;
        operator int()  const;
        operator std::string() const;
        keybind_action_id operator=(std::string const&);
        keybind_action_id operator=(std::nullptr_t const&);
    };

    LILAC_DLL std::ostream& operator<<(std::ostream& stream, Keybind const& bind);
    LILAC_DLL std::ostream& operator<<(std::ostream& stream, keybind_action_id const& id);
}

namespace std {
    template<>
    struct hash<lilac::Keybind> {
        LILAC_DLL std::size_t operator()(lilac::Keybind const&) const;
    };
    template<>
    struct hash<lilac::keybind_action_id> {
        LILAC_DLL std::size_t operator()(lilac::keybind_action_id const&) const;
    };
}
