#include "mod2.hpp"

void TestMod2::setup() {
    this->m_id          = "com.lilac.test_two";
    this->m_name        = "Lilac Test 2";
    this->m_developer   = "Lilac Team";
    this->m_description = "Test Mod #2";
    this->m_details     = "Test Mod #2 for making sure hooking, "
                          "loading and sharing interfaces works.";
    this->m_credits     = "";
    this->m_version     = { 1, 0, 0 };

    this->setDependencies({
        { "com.lilac.test_one", DependencyType::Required }
    });
}

TestMod2* TestMod2::get() {
    auto g_mod = new TestMod2;
    return g_mod;
}

LILAC_LOAD(TestMod2);
