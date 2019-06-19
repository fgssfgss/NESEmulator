### NESEmulator
This is simple NES Emulator, which was written as self education project.
Uses Blargg's NES sound library inside for the accurate APU emulation.

#### Works on
1. GNU/Linux
2. MS Win
3. Emscripten(need support for SharedBufferArray in your browser)

#### TODO
1. _"Slow down this shit, dog"_ - 180fps isn't normal for NES. // Done
2. Changing controller settings.
3. Remove shitcode in CPU, or in input. // Impossible
4. More mappers...

#### Controls
**Your can read in code, or press any of these buttons:**

Up, Down, Left, Right, Z, X, Space, Return

#### How to Build on Linux and Build Requirements
You just need libsdl2

    mkdir build
    cd build
    cmake ..
    make
    ./NESEmulator filename.nes
    