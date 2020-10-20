#include "include/Emulator.h"
#ifdef __EMSCRIPTEN__ 
#include <emscripten.h>
#endif 

#ifndef __EMSCRIPTEN__
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "USAGE: " << argv[0] << " filename.nes" << std::endl;
        return 1;
    }
    Emulator emu(argv[1]);
#else
extern "C" {
    int start_main(void);
}    
    
EMSCRIPTEN_KEEPALIVE
int start_main(void) {
    Emulator emu("game.nes");
#endif
    int ret = emu.run();
    return ret;
}
