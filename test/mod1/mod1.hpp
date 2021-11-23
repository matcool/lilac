#pragma once

#include <lilac.hpp>
#include <modapi.hpp>

USE_LILAC_NAMESPACE();

class MOD_API TestMod1 : public Mod {
    protected:
        void setup() override;
    
    public:
        void logMessage(std::string_view const& msg);

        static TestMod1* get();
};
