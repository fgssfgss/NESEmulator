#include "include/Emulator.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "USAGE: " << argv[0] << " filename.nes" << std::endl;
        return 1;
    }
    Emulator *emu = new Emulator(argv[1]);
    int ret = emu->run();
    delete emu;
    return ret;
}