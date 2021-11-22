#include <VersionInfo.hpp>
#include <utils/other/ext.hpp>
#include <utils/gd/general.hpp>

USE_LILAC_NAMESPACE();

VersionInfo::VersionInfo(int major, int minor, int patch) {
    this->m_major = major;
    this->m_minor = minor;
    this->m_patch = patch;
}

VersionInfo::VersionInfo(const char* versionString) {
    if (!std::sscanf(
        versionString, "v%d.%d.%d",
        this->m_major, this->m_minor, this->m_patch
    ))
    if (!std::sscanf(
        versionString, "%d.%d.%d",
        this->m_major, this->m_minor, this->m_patch
    ))
    std::sscanf(
        versionString, "%d %d %d",
        this->m_major, this->m_minor, this->m_patch
    );
}

int VersionInfo::getMajor() const { return this->m_major; }
int VersionInfo::getMinor() const { return this->m_minor; }
int VersionInfo::getPatch() const { return this->m_patch; }

std::string VersionInfo::toString() const {
    return cstrfmt("v%d.%d.%d", this->m_major, this->m_minor, this->m_patch);
}
