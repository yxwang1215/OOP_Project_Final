//
// Created by liu on 4/7/25.
//

#include "log.h"
#include <iostream>

void Log::printLog(const std::string& log, const bool clear) {
    if (clear) {
        std::cout << "\033[2J\033[1;1H";
    }
    std::cout << log;
    std::cout.flush();
}

Log& Log::operator<<(const std::string& a) {
    std::cout << a;
    std::cout.flush();
    return *this;
}

Log& Log::operator<<(const bool clear) {
    if (clear) {
        std::cout << "\033[2J\033[1;1H";
    }
    return *this;
}

Log& Log::operator<<(const char* a) {
    std::cout << a;
    std::cout.flush();
    return *this;
}

Log& Log::operator<<(const char a) {
    std::cout << a;
    std::cout.flush();
    return *this;
}

Log& Log::operator<<(const int a) {
    std::cout << a;
    std::cout.flush();
    return *this;
}

Log& Log::operator<<(const double a) {
    std::cout << a;
    std::cout.flush();
    return *this;
}

std::string Log::paddingEnd(const int len, const std::string& str) {
    std::string result = str;
    if (result.length() > len) {
        return result;
    }
    for (int i = 0; i < len - str.length(); ++i) {
        result += ' ';
    }
    return result;
}
