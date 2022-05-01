#pragma once

#include <string>
#include <iostream>

namespace utils {
    std::string urlEncode(const std::string &s);
    std::string urlEncode(const std::string &s, const std::string &encode);

    std::string urlDecode(const std::string &str);
}
