#pragma once
#ifndef BUS_6502
#define BUS_6502
#include <cstdint>
#include <array>
#include "olc6502.h++"
#include "consts.h++"
class Bus{
public:
    Bus();
    ~Bus();
public:
    Olc6502 cpu;
    std::array<uint8_t,64*KILOBYTE_SIZE> ram;

public:
void write(uint16_t addr,uint8_t data);
uint8_t read(uint16_t addr,bool bReadOnly=false);

};
#endif