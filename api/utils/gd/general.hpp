#pragma once

#include "include_gd.hpp"

namespace lilac::utils {
    template<typename ... Args>
    const char* cstrfmt(const char* fmt, Args ... args) {
        return cocos2d::CCString::createWithFormat(fmt, args ... )->getCString();
    }
}
