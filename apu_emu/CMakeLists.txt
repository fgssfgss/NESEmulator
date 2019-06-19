include_directories(boost)
include_directories(nes_apu)
set(SOURCES Simple_Apu.cpp
        Simple_Apu.h
        Sound_Queue.cpp
        Sound_Queue.h
        nes_apu/apu_snapshot.cpp
        nes_apu/apu_snapshot.h
        nes_apu/blargg_common.h
        nes_apu/blargg_source.h
        nes_apu/Blip_Buffer.cpp
        nes_apu/Blip_Buffer.h
        nes_apu/Blip_Synth.h
        nes_apu/Multi_Buffer.cpp
        nes_apu/Multi_Buffer.h
        nes_apu/Nes_Apu.cpp
        nes_apu/Nes_Apu.h
        nes_apu/Nes_Namco.cpp
        nes_apu/Nes_Namco.h
        nes_apu/Nes_Oscs.cpp
        nes_apu/Nes_Oscs.h
        nes_apu/Nes_Vrc6.cpp
        nes_apu/Nes_Vrc6.h
        nes_apu/Nonlinear_Buffer.cpp
        nes_apu/Nonlinear_Buffer.h)
add_library("apu_emu" OBJECT ${SOURCES})