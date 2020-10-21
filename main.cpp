#include "include/Emulator.h"
#ifdef __EMSCRIPTEN__ 
#include <emscripten.h>
#endif 

int main(int argc, char *argv[]) {
#ifndef __EMSCRIPTEN__
    if (argc < 2) {
        std::cout << "USAGE: " << argv[0] << " filename.nes" << std::endl;
        return 1;
    }
    Emulator emu(argv[1]);
#else
    Emulator emu("game.nes");
#endif
    int ret = emu.run();
    return ret;
}
