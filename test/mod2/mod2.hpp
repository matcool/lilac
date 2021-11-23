#pragma once

#include <lilac.hpp>
#include "../mod1/mod1.hpp"

USE_LILAC_NAMESPACE();

class TestMod2 : public Mod {
    protected:
        void setup() override;
    
    public:
        static TestMod2* get();
};
