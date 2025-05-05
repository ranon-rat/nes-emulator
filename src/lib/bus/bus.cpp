#include "bus.h++"
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wunused-parameter"

Bus::Bus()
{

    for (auto &i : cpuRam)
        i = 0x00;
    cpu.ConnectBus(this);
}
Bus::~Bus()
{
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cartridge)
{

    cart = cartridge;
    ppu.connectCartidge(cartridge);
}


void Bus::reset(){
    cpu.reset();

    nSystemClockCounter=0;

}
void Bus::clock(){
    ppu.clock();
    if(nSystemClockCounter%3==0){
        cpu.clock();
    }
    nSystemClockCounter++;

}