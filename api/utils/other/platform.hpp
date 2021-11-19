#pragma once

#include "../../lilac/Macros.hpp"
#include "../../lilac/Result.hpp"
#include <string>
#include <vector>
#include <functional>
#include "win32.hpp"

namespace lilac::utils {
    LILAC_DLL bool copyToClipboard(std::string const& data);
    LILAC_DLL std::string readClipboard();

    LILAC_DLL bool loadConsole();
    LILAC_DLL bool unloadConsole();
    LILAC_DLL void bufferConsoleInput();
}
