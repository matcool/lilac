#include "mod1.hpp"

void TestMod1::setup() {
    this->logMessage("Hi from TestMod1");
}

void TestMod1::logMessage(std::string_view const& msg) {
    this->log() << "Logged by TestMod1: " << msg << lilac::endl;
}

TestMod1* TestMod1::get() {
    auto g_mod = new TestMod1;
    return g_mod;
}

LILAC_LOAD(TestMod1);
