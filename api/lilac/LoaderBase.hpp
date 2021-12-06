#pragma once

#include "macros.hpp"
#include <vector>
#include <string>

namespace lilac {
    class Mod;

    class LILAC_DLL LoaderBase {
        protected:
            virtual ~LoaderBase();
    };
}
