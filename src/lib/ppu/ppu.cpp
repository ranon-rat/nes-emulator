#include "ppu.h++"
Pppu2c02::Pppu2c02()
{
    palScreen[0x00] = Color{84, 84, 84,255};
	palScreen[0x01] = Color{0, 30, 116,255};
	palScreen[0x02] = Color{8, 16, 144,255};
	palScreen[0x03] = Color{48, 0, 136,255};
	palScreen[0x04] = Color{68, 0, 100,255};
	palScreen[0x05] = Color{92, 0, 48,255};
	palScreen[0x06] = Color{84, 4, 0,255};
	palScreen[0x07] = Color{60, 24, 0,255};
	palScreen[0x08] = Color{32, 42, 0,255};
	palScreen[0x09] = Color{8, 58, 0,255};
	palScreen[0x0A] = Color{0, 64, 0,255};
	palScreen[0x0B] = Color{0, 60, 0,255};
	palScreen[0x0C] = Color{0, 50, 60,255};
	palScreen[0x0D] = Color{0, 0, 0,255};
	palScreen[0x0E] = Color{0, 0, 0,255};
	palScreen[0x0F] = Color{0, 0, 0,255};

	palScreen[0x10] = Color{152, 150, 152,255};
	palScreen[0x11] = Color{8, 76, 196,255};
	palScreen[0x12] = Color{48, 50, 236,255};
	palScreen[0x13] = Color{92, 30, 228,255};
	palScreen[0x14] = Color{136, 20, 176,255};
	palScreen[0x15] = Color{160, 20, 100,255};
	palScreen[0x16] = Color{152, 34, 32,255};
	palScreen[0x17] = Color{120, 60, 0,255};
	palScreen[0x18] = Color{84, 90, 0,255};
	palScreen[0x19] = Color{40, 114, 0,255};
	palScreen[0x1A] = Color{8, 124, 0,255};
	palScreen[0x1B] = Color{0, 118, 40,255};
	palScreen[0x1C] = Color{0, 102, 120,255};
	palScreen[0x1D] = Color{0, 0, 0,255};
	palScreen[0x1E] = Color{0, 0, 0,255};
	palScreen[0x1F] = Color{0, 0, 0,255};

	palScreen[0x20] = Color{236, 238, 236,255};
	palScreen[0x21] = Color{76, 154, 236,255};
	palScreen[0x22] = Color{120, 124, 236,255};
	palScreen[0x23] = Color{176, 98, 236,255};
	palScreen[0x24] = Color{228, 84, 236,255};
	palScreen[0x25] = Color{236, 88, 180,255};
	palScreen[0x26] = Color{236, 106, 100,255};
	palScreen[0x27] = Color{212, 136, 32,255};
	palScreen[0x28] = Color{160, 170, 0,255};
	palScreen[0x29] = Color{116, 196, 0,255};
	palScreen[0x2A] = Color{76, 208, 32,255};
	palScreen[0x2B] = Color{56, 204, 108,255};
	palScreen[0x2C] = Color{56, 180, 204,255};
	palScreen[0x2D] = Color{60, 60, 60,255};
	palScreen[0x2E] = Color{0, 0, 0,255};
	palScreen[0x2F] = Color{0, 0, 0,255};

	palScreen[0x30] = {236, 238, 236,255};
	palScreen[0x31] = {168, 204, 236,255};
	palScreen[0x32] = {188, 188, 236,255};
	palScreen[0x33] = {212, 178, 236,255};
	palScreen[0x34] = {236, 174, 236,255};
	palScreen[0x35] = {236, 174, 212,255};
	palScreen[0x36] = {236, 180, 176,255};
	palScreen[0x37] = {228, 196, 144,255};
	palScreen[0x38] = {204, 210, 120,255};
	palScreen[0x39] = {180, 222, 120,255};
	palScreen[0x3A] = {168, 226, 144,255};
	palScreen[0x3B] = {152, 226, 180,255};
	palScreen[0x3C] = {160, 214, 228,255};
	palScreen[0x3D] = {160, 162, 160,255};
	palScreen[0x3E] = {0, 0, 0,255};
	palScreen[0x3F] = {0, 0, 0,255};
    for (size_t i = 0; i < sprScreen.size(); i++)
    {
        sprScreen[i] = 0x00;

        sprNameTable[0][i] = 0x00;
        sprNameTable[1][i] = 0x00;
    }
   
}



