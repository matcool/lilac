#pragma once

#define GDH_IS_DLL

// not feeling like making GD.h dll-interface,
// so just disable the warnings ig
#pragma warning(disable: 4275)  // inheritance
#pragma warning(disable: 4251)  // members
#pragma warning(disable: 4067)  // shut up about unexpected tokens in cocos2d

#include "../../lilac/Macros.hpp"

#include <cocos2d.h>
#include <gd.hpp>
#include <GUI/CCControlExtension/CCScale9Sprite.h>
