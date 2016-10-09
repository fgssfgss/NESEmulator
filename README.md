### NESEmulator
This is simple NES Emulator, which was written as self education project.
PPU code was partially "спизжен"(stolen) to work anyhow.

#### TODO
1. _"Slow down this shit, dog"_ - 180fps isn't normal for NES.
2. Changing controller settings.
3. Sound, APU
4. Remove shitcode in CPU, or in input.
5. More mappers...

#### Controls
**Your can read in code, or press any of these buttons:**

Up, Down, Left, Right, Z, X, Space, Return

#### How to Build and Build Requirements
You need libsdl2 and libsdl2_image

    mkdir build
    cd build
    cmake ..
    make
    ./NESEmulator
    