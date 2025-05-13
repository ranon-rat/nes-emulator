#include "bus.h++"
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wunused-parameter"

void Bus::ppuWrite(uint16_t addr, uint8_t data)
{
    ppu.ppuWrite(addr, data);
    
}
uint8_t Bus::ppuRead(uint16_t addr, bool bReadOnly)
{
    return ppu.ppuRead(addr, bReadOnly);
}
