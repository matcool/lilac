#pragma once

#include "Macros.hpp"
#include <string_view>

namespace lilac {
    class LILAC_DLL VersionInfo {
        protected:
            int m_major = 1;
            int m_minor = 0;
            int m_patch = 0;
        
        public:
            VersionInfo() = delete;
            VersionInfo(int major, int minor, int patch);
            VersionInfo(const char* versionString);

            int getMajor() const;
            int getMinor() const;
            int getPatch() const;

            std::string toString() const;
    };
}
