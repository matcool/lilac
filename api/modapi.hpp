#pragma once

#ifdef _EXPORTING_LILAC_MOD
    #define MOD_API     __declspec(dllexport)
#else
    #define MOD_API     __declspec(dllimport)
#endif
