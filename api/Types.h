#ifndef __INCLUDE_LILAC_TYPES_H__
#define __INCLUDE_LILAC_TYPES_H__

#include <string>
#include <vector>

namespace cocos2d {
    class CCPoint;
    class CCSize;
    class CCNode;
}

namespace lilac {

    using l_addr = uintptr_t;
    using l_byte = unsigned char;
    using l_byte_array = std::vector<l_byte>;

    enum class ModLoadTime {
        startup = 0,
        loading_screen = 1,
        main_menu = 2,
    };

    enum class TargetContainer {
        MenuLayer_main_menu,
        MenuLayer_bottom_menu,
    };

    enum class TargetPosition {
        start,
        middle,
        end,
    };

}

#endif
