#pragma once
#ifndef OLC2c02
#define OLC2c02
#include <cstdint>
#include <memory>
#include <array>
#include "consts.h++"
#include "cartridge.h++"
#include "raylib/raylib.h"
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240

#define PATTERN_SIZE 128
class Olc2c02
{

public:
    Olc2c02();

public:
    std::shared_ptr<Cartridge> cart;
    uint8_t tblName[2][KILOBYTE_SIZE];
    uint8_t tblPalette[8];
    uint8_t tblPattern[2][KILOBYTE_SIZE * 4];

public:
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

public:
    void ppuWrite(uint16_t addr, uint8_t data);
    uint8_t ppuRead(uint16_t addr, bool bReadOnly = false);

public: // interface
    void connectCartidge(const std::shared_ptr<Cartridge> &cartridge);
    void clock();

private:
    std::array<Color,0x40>palScreen;
    std::array<char, SCREEN_WIDTH * SCREEN_HEIGHT * 4> sprScreen;                     // 256,240
    std::array<std::array<char, SCREEN_WIDTH * SCREEN_HEIGHT * 4>, 2> sprNameTable;   // 256,240
    std::array<std::array<char, PATTERN_SIZE * PATTERN_SIZE * 4>, 2> srpPatternTable; // 128*128

public: // drawing stuff
    Image GetScreen();
    Image GetNameTable(uint8_t i);
    Image GetPatternTable(uint8_t i);
    void screenDrawPixel(int x,int y,Color c);
    bool frame_complete = false;

private:
    uint16_t scanline = 0;
    uint16_t cycle = 0;
};
#endif