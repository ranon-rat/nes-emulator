#pragma once
#ifndef BUS_6502
#define BUS_6502
#include <cstdint>
#include <array>
#include "consts.h++"
#include "cpu.h++"
#include "ppu.h++"
#include "cartridge.h++"
class Bus
{
public:
    Bus();
    ~Bus();

public:
    Olc6502 cpu;
    Olc2c02 ppu;

    std::array<uint8_t, 2 * KILOBYTE_SIZE> cpuRam;

    std::shared_ptr<Cartridge> cart;

public:
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

public:
    void ppuWrite(uint16_t addr, uint8_t data);
    uint8_t ppuRead(uint16_t addr, bool bReadOnly = false);

public:
    void reset();
    void clock();
    void insertCartridge(const std::shared_ptr<Cartridge> &cartridge);

private:
    // A count of how many clocks have passed
    uint32_t nSystemClockCounter = 0;
};
#endif