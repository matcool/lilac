#include "mod1.hpp"

#pragma section(".lilac", read)

__declspec(allocate(".lilac"))
static const char* g_modinfo =
R"JSON({
	"version":      "v1.0.0",
	"id":           "com.lilac.test_one",
    "name":         "Lilac Test 1",
    "developer":    "Lilac Team",
    "description":  "Test Mod #1",
    "details":      "Test Mod #1 for making sure hooking, loading and sharing interfaces works.",
    "credits":      null,
	"dependencies": null
})JSON";

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
