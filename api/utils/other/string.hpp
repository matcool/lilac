#pragma once

#include "../../lilac/Macros.hpp"
#include <string>
#include <vector>
#include <functional>

namespace lilac {
    /**
     * Convert std::wstring to std::string (UTF-8)
     * @param str String to convert
     * @returns std::string
     */
    LILAC_DLL std::string  string_convert(std::wstring const& str);
    /**
     * Convert std::string (UTF-8) to std::wstring
     * @param str String to convert
     * @returns std::wstring
     */
    LILAC_DLL std::wstring string_convert(std::string  const& str);

    LILAC_DLL bool string_ends_with(std::string  const& str, std::string  const& substr);
    LILAC_DLL bool string_ends_with(std::wstring const& str, std::wstring const& substr);

    LILAC_DLL std::string & string_to_lower_self(std::string & str);
    LILAC_DLL std::wstring& string_to_lower_self(std::wstring& str);

    LILAC_DLL std::string  string_to_lower(std::string  const& str);
    LILAC_DLL std::wstring string_to_lower(std::wstring const& str);

    LILAC_DLL std::string& string_replace_self(
        std::string & str,
        std::string const& orig,
        std::string const& repl
    );
    LILAC_DLL std::wstring& string_replace_self(
        std::wstring & str,
        std::wstring const& orig,
        std::wstring const& repl
    );

    LILAC_DLL std::string string_replace(
        std::string const& str,
        std::string const& orig,
        std::string const& repl
    );
    LILAC_DLL std::wstring string_replace(
        std::wstring const& str,
        std::wstring const& orig,
        std::wstring const& repl
    );

    LILAC_DLL std::vector<std::string> string_split(
        std::string const& str,
        std::string const& split
    );
    LILAC_DLL std::vector<std::wstring> string_split(
        std::wstring const& str,
        std::wstring const& split
    );

    LILAC_DLL std::vector<char>    string_split(std::string  const& str);
    LILAC_DLL std::vector<wchar_t> string_split(std::wstring const& str);

    LILAC_DLL bool string_contains(std::string  const& str, std::string  const& subs);
    LILAC_DLL bool string_contains(std::wstring const& str, std::wstring const& subs);

    LILAC_DLL bool string_contains(std::string  const& str, typename char    c);
    LILAC_DLL bool string_contains(std::wstring const& str, typename wchar_t c);

    LILAC_DLL bool string_contains_any(
        std::string const& str,
        std::vector<std::string> const& subs
    );
    LILAC_DLL bool string_contains_any(
        std::wstring const& str,
        std::vector<std::wstring> const& subs
    );

    LILAC_DLL bool string_contains_all(
        std::string const& str,
        std::vector<std::string> const& subs
    );
    LILAC_DLL bool string_contains_all(
        std::wstring const& str,
        std::vector<std::wstring> const& subs
    );

    LILAC_DLL size_t string_count(std::string  const& str, char c);
    LILAC_DLL size_t string_count(std::wstring const& str, wchar_t c);

    LILAC_DLL std::string & string_trim_left_self(std::string & str);
    LILAC_DLL std::wstring& string_trim_left_self(std::wstring& str);
    LILAC_DLL std::string & string_trim_right_self(std::string & str);
    LILAC_DLL std::wstring& string_trim_right_self(std::wstring& str);
    LILAC_DLL std::string & string_trim_self(std::string & str);
    LILAC_DLL std::wstring& string_trim_self(std::wstring& str);

    LILAC_DLL std::string   string_trim_left(std::string  const& str);
    LILAC_DLL std::wstring  string_trim_left(std::wstring const& str);
    LILAC_DLL std::string   string_trim_right(std::string  const& str);
    LILAC_DLL std::wstring  string_trim_right(std::wstring const& str);
    LILAC_DLL std::string   string_trim(std::string  const& str);
    LILAC_DLL std::wstring  string_trim(std::wstring const& str);

    LILAC_DLL std::string & string_normalize_self(std::string  & str);
    LILAC_DLL std::wstring& string_normalize_self(std::wstring & str);
    LILAC_DLL std::string   string_normalize(std::string  const& str);
    LILAC_DLL std::wstring  string_normalize(std::wstring const& str);

    std::string  operator"" _s  (const char*    string, size_t);
    std::wstring operator"" _ws (const char*    string, size_t);
    std::wstring operator"" _ws (const wchar_t* string, size_t);
}

