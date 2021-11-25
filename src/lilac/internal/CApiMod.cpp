#include "CApiMod.hpp"

#define ASSIGN_FROM_CMI(member) \
    if (info->member) this->m_##member = info->member;

void CApiMod::setup() {}

void CApiMod::enable() {
    if (this->m_enableFunc) {
        this->m_disableFunc();
    }
}

void CApiMod::disable() {
    if (this->m_disableFunc) {
        this->m_disableFunc();
    }
}

CApiMod::~CApiMod() {
    if (this->m_unloadFunc) {
        this->m_unloadFunc();
    }
}

CApiMod::CApiMod(CModInfo* info) {
    this->setup();

    ASSIGN_FROM_CMI(version);
    ASSIGN_FROM_CMI(id);
    ASSIGN_FROM_CMI(name);
    ASSIGN_FROM_CMI(developer);
    ASSIGN_FROM_CMI(description);
    ASSIGN_FROM_CMI(details);
    ASSIGN_FROM_CMI(credits);
}
