#include "ppu.h++"

void Pppu2c02::connectCartidge(const std::shared_ptr<Cartridge> &cartridge)
{
    this->cart = cartridge;
}

void Pppu2c02::clock()
{
    screenDrawPixel(cycle - 1, scanline, palScreen[(rand() % 2) ? 0x3f : 0x30]);

    cycle++;

    if (cycle >= 341)
    {
        cycle = 0;
        scanline++;
        if (scanline >= 261)
        {
            scanline = -1;
            frame_complete = true;
        }
    }
}