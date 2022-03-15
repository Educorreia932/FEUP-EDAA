#include "urlencode.h"

#include <iomanip>

std::string urlencode(const std::string &s){
    std::stringstream os;
    os.fill('0');
    os << std::hex;
    for(const char &c:s){
        if(isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            os << c;
        }else{
            os << std::uppercase << '%'
               << std::setw(2) << int((unsigned char)c)
               << std::nouppercase;
        }
    }
    return os.str();
}

std::string urldecode(const std::string &str){
    std::string s = "";
    size_t i = 0;
    while(i < str.size()){
        if(str[i] == '%'){
            if(i+2 >= str.size()) throw std::invalid_argument("invalid argument "+str);
            std::stringstream ss;
            ss << std::hex << str.substr(i+1,2);
            int a; ss >> a;
            s += char(a);
            i += 3;
        }else{
            s += str[i++];
        }
    }
    return s;
}
