#include "olc6502.h++"
#include "bus.h++"
#include "utils.h++"
#include "consts.h++"
#pragma GCC diagnostic ignored "-Wreturn-type"

uint8_t Olc6502::read(uint16_t address)
{
    return m_bus->read(address);
}

void Olc6502::write(uint16_t address, uint8_t data)
{
    m_bus->write(address, data);
}


void Olc6502::clock()
{
    if (cycles == 0)
    {
        opcode = read(pc_r);
        pc_r++;
        cycles = m_lookup[opcode].cycles;
        uint8_t additional_cycle1 = (this->*m_lookup[opcode].addrmode)();
        uint8_t additional_cycle2 = (this->*m_lookup[opcode].operate)();
        cycles += (additional_cycle1 & additional_cycle2);
    }
    cycles--;
}

void Olc6502::SetFlag(Olc6502::FLAG6502 f, bool v)
{
    if (v)
    {
        // this will write to our status
        status_r |= f;
        // 0b00001000
        // 0b01000000|
        //____________
        // 0b01001000
        return;
    }
    // this will delete a specific byte i think
    // 0b01000000 // lets say this is status
    // 0b00010000 // this is the flag we are passing
    // then when we are evaluationg it uhh
    // 0b11101111
    // then we make the comparison
    // 0b01000000
    // 0b11101111 &
    //____________
    // 0b01000000 // okay yes it will delete a specific bit :)
    status_r &= ~f;
}
uint8_t Olc6502::GetFlag(FLAG6502 f)
{
	return ((status_r & f) > 0) ? 1 : 0;
}
