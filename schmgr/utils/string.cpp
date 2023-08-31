
#include <string>
#include <locale>
#include <codecvt>
#include "schmgr/utils/string.h"

namespace utils
{
//convert string to wstring
std::wstring to_wide_string(const std::string& input)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(input);
}

//convert wstring to string 
std::string to_byte_string(const std::wstring& input)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(input);
}
}
