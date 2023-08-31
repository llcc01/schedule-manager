#ifndef UTILS_STRING_H_
#define UTILS_STRING_H_

#include <string>

namespace utils
{
std::wstring to_wide_string(const std::string& input);
std::string to_byte_string(const std::wstring& input);


} // namespace utils
#endif // UTILS_STRING_H_