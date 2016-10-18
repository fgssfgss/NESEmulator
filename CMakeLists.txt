cmake_minimum_required(VERSION 2.8)
project(NESEmulator)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${NESEmulator_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})

set(SOURCE_FILES main.cpp src/Emulator.cpp include/Emulator.h src/Memory.cpp include/Memory.h include/Common.h src/CPU.cpp include/CPU.h src/ROM.cpp include/ROM.h src/PPU.cpp include/PPU.h src/Controller.cpp include/Controller.h include/IMapper.h src/Mapper000.cpp include/Mapper000.h src/Mapper004.cpp include/Mapper004.h src/Console.cpp include/Console.h)
add_executable(NESEmulator ${SOURCE_FILES})
target_link_libraries(NESEmulator ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY})