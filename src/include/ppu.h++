#pragma once
#ifndef Pppu2c02_Hpp
#define Pppu2c02_Hpp
#include <cstdint>
#include <memory>
#include <array>
#include "consts.h++"
#include "cartridge.h++"
#include "raylib/raylib.h"
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240

#define PATTERN_SIZE 128
class Pppu2c02
{

public:
    Pppu2c02();

public:
    std::shared_ptr<Cartridge> cart;
   uint8_t     tblName[2][1024];
	uint8_t     tblPattern[2][4096];
	uint8_t		tblPalette[32];

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
    std::array<Color, 0x40> palScreen;
    std::array<char, SCREEN_WIDTH * SCREEN_HEIGHT * 4> sprScreen;                     // 256,240
    std::array<std::array<char, SCREEN_WIDTH * SCREEN_HEIGHT * 4>, 2> sprNameTable;   // 256,240
    std::array<std::array<char, PATTERN_SIZE * PATTERN_SIZE * 4>, 2> srpPatternTable; // 128*128

public: // drawing stuff
    Image GetScreen();
    Image GetNameTable(uint8_t i);
    Image GetPatternTable(uint8_t i, uint8_t palette);
    void screenDrawPixel(int x, int y, Color c);
    void patternDrawPixel(int i, int x, int y, Color c);

    void patternDrawPixel(int i, int x, int y, uint8_t c);
    Color GetColorFromPaletteRam(uint8_t palette, uint8_t pixel);
    bool frame_complete = false;

private:
    uint16_t scanline = 0;
    uint16_t cycle = 0;

 union
	{
		struct
		{
			uint8_t unused : 5;
			uint8_t sprite_overflow : 1;
			uint8_t sprite_zero_hit : 1;
			uint8_t vertical_blank : 1;
		};

		uint8_t reg;
	} status;


    union
    {
        struct
        {
            uint8_t grayscale : 1;
            uint8_t render_background_left : 1;
            uint8_t render_sprites_left : 1;
            uint8_t render_background : 1;
            uint8_t render_sprites : 1;
            uint8_t enhance_red : 1;
            uint8_t enhance_green : 1;
            uint8_t enhance_blue : 1;
        };

        uint8_t reg;
    } mask;
    union PPUCTRL
    {
        struct
        {
            uint8_t nametable_x : 1;
            uint8_t nametable_y : 1;
            uint8_t increment_mode : 1;
            uint8_t pattern_sprite : 1;
            uint8_t pattern_background : 1;
            uint8_t sprite_size : 1;
            uint8_t slave_mode : 1; // unused
            uint8_t enable_nmi : 1;
        };

        uint8_t reg;
    } control;

    uint8_t address_latch=0x00;
    uint8_t ppu_data_buffer=0x00;
    uint16_t ppu_address=0x0000;
};
#endif