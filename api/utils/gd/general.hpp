#pragma once

#include "include_gd.hpp"

namespace lilac::cocos {
    LILAC_DLL void limitNodeSize(
        cocos2d::CCNode* node,
        cocos2d::CCSize const& size,
        float def,
        float min
    );
}

namespace lilac::utils {
    template<typename ... Args>
    const char* cstrfmt(const char* fmt, Args ... args) {
        return cocos2d::CCString::createWithFormat(fmt, args ... )->getCString();
    }
}
