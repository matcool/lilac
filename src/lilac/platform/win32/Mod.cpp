#pragma once

#include <lilac.hpp>

#ifdef LILAC_IS_WINDOWS

USE_LILAC_NAMESPACE();

void ModBase::platformCleanup() {
    // pretty sure this is unnecessary...
    // FreeLibrary frees up the memory
    // associated with m_platformInfo
    // anyway I think
    auto hmod = this->m_platformInfo->m_hmod;
    delete this->m_platformInfo;
    FreeLibrary(hmod);
}

#endif
