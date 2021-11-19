#include <utils/gd/convert.hpp>

USE_LILAC_NAMESPACE();

void lilac::cocos::ccDrawColor4B(ccColor4B const& c) {
    cocos2d::ccDrawColor4B(c.r, c.g, c.b, c.a);
}

ccColor4B lilac::cocos::invert4B(ccColor4B const& c) {
    return {
        static_cast<GLubyte>(255 - c.r),
        static_cast<GLubyte>(255 - c.g),
        static_cast<GLubyte>(255 - c.b),
        c.a
    };
}

ccColor3B lilac::cocos::invert3B(ccColor3B const& c) {
    return {
        static_cast<GLubyte>(255 - c.r),
        static_cast<GLubyte>(255 - c.g),
        static_cast<GLubyte>(255 - c.b)
    };
}

ccColor3B lilac::cocos::to3B(ccColor4B const& c) {
    return { c.r, c.g, c.b };
}

ccColor4B lilac::cocos::to4B(ccColor3B const& c, GLubyte alpha) {
    return { c.r, c.g, c.b, alpha };
}

ccColor4F lilac::cocos::to4F(ccColor4B const& c) {
    return { c.r / 255.f, c.g / 255.f, c.b / 255.f, c.a / 255.f };
}
