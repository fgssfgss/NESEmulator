//
// Created by lin-k on 01.10.2016.
//

#ifndef NESEMULATOR_COMMON_H
#define NESEMULATOR_COMMON_H

#include <cstdint>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <bitset>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>

#ifdef EMSCRIPTEN_BUILD
#include <emscripten.h>
#endif

const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 240;

namespace patch {
    template<typename T>
    std::string to_string(const T &n) {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}

#endif //NESEMULATOR_COMMON_H
