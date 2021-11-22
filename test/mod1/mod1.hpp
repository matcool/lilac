#pragma once

#include <lilac.hpp>

USE_LILAC_NAMESPACE();

class TestMod1 : public Mod {
    protected:
        void setup() override;
    
    public:
        static TestMod1* get();
};
