#include "include/Emulator.h"
#ifdef __EMSCRIPTEN__ 
#include <emscripten.h>

int isReady(void) {
    int ready = EM_ASM_INT( return isReady(); );
    return ready;
}

#endif 

int main(int argc, char *argv[]) {
#ifndef __EMSCRIPTEN__
    if (argc < 2) {
        std::cout << "USAGE: " << argv[0] << " filename.nes" << std::endl;
        return 1;
    }
    Emulator emu(argv[1]);
#else
    while (isReady() == 0) {
        emscripten_sleep(1000);
    }
    Emulator emu("game.nes");
#endif
    int ret = emu.run();
    return ret;
}
