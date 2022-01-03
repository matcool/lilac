#include "InternalMod.hpp"

void InternalMod::setup() {
    this->m_id          = "com.lilac.lilac";
    this->m_name        = "Lilac";
    this->m_developer   = "Lilac Team";
    this->m_description = "Internal representation";
    this->m_details     = "Internal representation of Lilac.";
    this->m_credits     = "";
    this->m_version     = { 1, 0, 0 };
}

InternalMod* InternalMod::get() {
    static auto inst = new InternalMod;
    return inst;
}
