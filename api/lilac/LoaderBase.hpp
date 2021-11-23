#pragma once

#include "Macros.hpp"
#include <vector>
#include <string>

namespace lilac {
    class Mod;

    class LILAC_DLL LoaderBase {
        protected:
            virtual ~LoaderBase();
    };
}
