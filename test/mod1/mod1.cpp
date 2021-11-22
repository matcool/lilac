#include "mod1.hpp"

void TestMod1::setup() {
    this->m_id          = "com.lilac.test_one";
    this->m_name        = "Lilac Test 1";
    this->m_developer   = "Lilac Team";
    this->m_description = "Test Mod #1";
    this->m_details     = "Test Mod #1 for making sure hooking, "
                          "loading and sharing interfaces works.";
    this->m_credits     = "";
    this->m_version     = { 1, 0, 0 };
}

TestMod1* TestMod1::get() {
    auto g_mod = new TestMod1;
    return g_mod;
}

LILAC_LOAD(TestMod1);
