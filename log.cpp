//
// Created by liu on 4/7/25.
//

#include "log.h"
#include <iostream>

// 打印日志信息
// log: 要打印的日志字符串
// clear: 是否清空屏幕
void Log::printLog(const std::string& log, const bool clear) {
    // 如果 clear 为 true，则清空终端屏幕
    if (clear) {
        std::cout << "\033[2J\033[1;1H";
    }
    // 打印日志信息
    std::cout << log;
    // 刷新输出缓冲区
    std::cout.flush();
}

// 重载 << 运算符，用于输出字符串
Log& Log::operator<<(const std::string& a) {
    // 输出字符串
    std::cout << a;
    // 刷新输出缓冲区
    std::cout.flush();
    // 返回当前 Log 对象，支持链式调用
    return *this;
}

// 重载 << 运算符，用于控制是否清空屏幕
// clear: 是否清空屏幕
Log& Log::operator<<(const bool clear) {
    // 如果 clear 为 true，则清空终端屏幕
    if (clear) {
        std::cout << "\033[2J\033[1;1H";
    }
    // 返回当前 Log 对象，支持链式调用
    return *this;
}

// 重载 << 运算符，用于输出 C 风格字符串
Log& Log::operator<<(const char* a) {
    // 输出 C 风格字符串
    std::cout << a;
    // 刷新输出缓冲区
    std::cout.flush();
    // 返回当前 Log 对象，支持链式调用
    return *this;
}

// 重载 << 运算符，用于输出字符
Log& Log::operator<<(const char a) {
    // 输出字符
    std::cout << a;
    // 刷新输出缓冲区
    std::cout.flush();
    // 返回当前 Log 对象，支持链式调用
    return *this;
}

// 重载 << 运算符，用于输出整数
Log& Log::operator<<(const int a) {
    // 输出整数
    std::cout << a;
    // 刷新输出缓冲区
    std::cout.flush();
    // 返回当前 Log 对象，支持链式调用
    return *this;
}

// 重载 << 运算符，用于输出双精度浮点数
Log& Log::operator<<(const double a) {
    // 输出双精度浮点数
    std::cout << a;
    // 刷新输出缓冲区
    std::cout.flush();
    // 返回当前 Log 对象，支持链式调用
    return *this;
}

// 在字符串末尾填充空格，使其达到指定长度
// len: 目标长度
// str: 原始字符串
// 返回填充后的字符串
std::string Log::paddingEnd(const int len, const std::string& str) {
    // 复制原始字符串
    std::string result = str;
    // 如果原始字符串长度已大于或等于目标长度，直接返回
    if (result.length() > len) {
        return result;
    }
    // 计算需要填充的空格数量，并在末尾添加空格
    for (int i = 0; i < len - str.length(); ++i) {
        result += ' ';
    }
    // 返回填充后的字符串
    return result;
}
