#include "getDirectory.h"

std::string utils::getDirectory(const std::string &filepath){
    size_t found = filepath.find_last_of("/\\");
    if(found == std::string::npos) return "";
    return filepath.substr(0, found);
}
