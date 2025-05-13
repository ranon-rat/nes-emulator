#include "bus.h++"

#include <bitset>
#include <iostream>
void Bus::cpuWrite(uint16_t addr, uint8_t data)
{
    // aqui definimos el rango en el cual la ram es utilizable

    if (cart->cpuWrite(addr, data))
    {
    }

    else if (addr >= BEGIN_ACCESS_RAM && addr <= END_ACCESS_RAM)
        cpuRam[addr & 0x07ff] = data;
    else if (addr >= BEGIN_ACCESS_PPU && addr <= END_ACCESS_PPU)
        ppu.cpuWrite(addr & 0x0007, data); // this will write in 8 different states of the memory :)
}

uint8_t Bus::cpuRead(uint16_t addr, bool readOnly)
{

    uint8_t data = 0x00;

    if (cart->cpuRead(addr, data))
    {
    }
    else if (addr >= BEGIN_ACCESS_RAM && addr <= END_ACCESS_RAM)
    {
        data = cpuRam[addr & 0x07ff];
    }
    else if (addr >= BEGIN_ACCESS_PPU && addr <= END_ACCESS_PPU)
    {
        data = ppu.cpuRead(addr & 0x0007, readOnly); // this will write in 8 different states of the memory :)
        if((addr&0x007)== 0x0002){
            std::cout<<std::bitset<8>(data)<<"\n";
        }
    }


   
    return data;
}