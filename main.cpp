#include "include/Emulator.h"

int main(int argc, char *argv[]) {
#ifndef __EMSCRIPTEN__
    if (argc < 2) {
        std::cout << "USAGE: " << argv[0] << " filename.nes" << std::endl;
        return 1;
    }
    Emulator *emu = new Emulator(argv[1]);
#else
    Emulator *emu = new Emulator("game.nes");
#endif
    int ret = emu->run();
    delete emu;
    return ret;
}