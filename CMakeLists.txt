cmake_minimum_required(VERSION 2.8)
project(NESEmulator)

if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -DEMSCRIPTEN_BUILD -s FORCE_FILESYSTEM=1 -r -std=c++11 -s USE_SDL=2 -s USE_PTHREADS=1 -O3 -finline-functions -Wno-unused-value")
    set(CMAKE_EXECUTABLE_SUFFIX ".bc")
else()
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${NESEmulator_SOURCE_DIR}/cmake")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -finline-functions -funroll-all-loops")
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR})
endif()

add_subdirectory("apu_emu")
include_directories("apu_emu")

set(SOURCE_FILES main.cpp src/Emulator.cpp include/Emulator.h src/Memory.cpp include/Memory.h include/Common.h src/CPU.cpp include/CPU.h src/ROM.cpp include/ROM.h src/PPU.cpp include/PPU.h src/Controller.cpp include/Controller.h include/IMapper.h src/Mapper000.cpp include/Mapper000.h src/Mapper004.cpp include/Mapper004.h src/Console.cpp include/Console.h src/APU.cpp include/APU.h)
add_executable(NESEmulator ${SOURCE_FILES} $<TARGET_OBJECTS:apu_emu>)

if (EMSCRIPTEN)
    add_custom_command(TARGET NESEmulator
        COMMAND ${CMAKE_CXX_COMPILER} -O3 -o index.html ${CMAKE_CURRENT_BINARY_DIR}/NESEmulator.bc -s WASM=1 -s FORCE_FILESYSTEM=1 -s USE_SDL=2 -s USE_PTHREADS=1 --shell-file "${NESEmulator_SOURCE_DIR}/emscripten_page.html"
                       POST_BUILD
                       COMMENT "Creating HTML file, please copy index.* files to your server root dir")
else()
    target_link_libraries(NESEmulator "${SDL2_LIBRARY}")
endif()
