#include "bus.h++"
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wunused-parameter"

Bus::Bus()
{

    for (auto &i : ram)
        i = 0x00;
    cpu.ConnectBus(this);
}
Bus::~Bus()
{
}

void Bus::write(uint16_t addr, uint8_t data)
{

    // aqui definimos el rango en el cual la ram es utilizable
    if (addr >= 0x0000 && addr <= 0xffff)
        ram[addr] = data;
}

uint8_t Bus::read(uint16_t addr, bool readOnly) {

    if (addr >= 0x0000 && addr <= 0xffff){
        return ram[addr];
    }
    return 0x0000;
    
}