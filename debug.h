#pragma once
#include <iostream>

#define DEBUG_MODE 1

inline void LOG(const std::string& msg) {
#if DEBUG_MODE
    std::cout << msg << std::endl;
#endif
}