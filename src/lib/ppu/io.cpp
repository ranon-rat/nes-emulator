#include "ppu.h++"
#include "consts.h++"
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Olc2c02::cpuWrite(uint16_t addr, uint8_t data)
{
    switch (static_cast<PPU_addresses>(addr))
    {
    case PPU_addresses::CONTROLL: // controll
        break;
    case PPU_addresses::MASK: //  mask
        break;
    case PPU_addresses::STATUS: // status
        break;
    case PPU_addresses::OAM_ADDRESS: // oam address
        break;
    case PPU_addresses::OAM_DATA: // oam data
        break;
    case PPU_addresses::SCROLL: // scroll
        break;
    case PPU_addresses::PPU_ADDRESS: // ppu address
        break;
    case PPU_addresses::PPU_DATA: // PPU data
        break;
    }
}
uint8_t Olc2c02::cpuRead(uint16_t addr, bool bReadOnly)
{

    uint8_t data = 0x00;
    switch (static_cast<PPU_addresses>(addr))
    {
    case PPU_addresses::CONTROLL: // controll
        break;
    case PPU_addresses::MASK: //  mask
        break;
    case PPU_addresses::STATUS: // status
        break;
    case PPU_addresses::OAM_ADDRESS: // oam address
        break;
    case PPU_addresses::OAM_DATA: // oam data
        break;
    case PPU_addresses::SCROLL: // scroll
        break;
    case PPU_addresses::PPU_ADDRESS: // ppu address
        break;
    case PPU_addresses::PPU_DATA: // PPU data
        break;
    }
    return data;
}

void Olc2c02::ppuWrite(uint16_t addr, uint8_t data)
{
    addr &= END_ACCESS_PPU;
    if (cart->ppuWrite(addr, data))
    {
    }
}
uint8_t Olc2c02::ppuRead(uint16_t addr, bool bReadOnly)
{
    uint8_t data = 0x00;
    addr &= END_ACCESS_PPU;
    if (cart->ppuRead(addr, data))
    {
    }

    return data;
}
