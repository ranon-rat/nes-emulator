#include "cpu.h++"

#include "bus.h++"
#include "utils.h++"
#include "consts.h++"
#include <iostream>

uint8_t Cpu6502::read(uint16_t address)
{
    return m_bus->cpuRead(address, false);
}

void Cpu6502::write(uint16_t address, uint8_t data)
{
    m_bus->cpuWrite(address, data);
}

void Cpu6502::clock()
{
    if (cycles == 0)
    {
        opcode = read(pc_r);

        if (pc_r <= 0xc29f|| pc_r>=0xc91f)
        {
            std::cout <<std::hex <<"opcode: " << (int)opcode << "\n"
                      << this->m_lookup[opcode].name << "\n";
        }
      
        pc_r++;
        cycles = m_lookup[opcode].cycles;

        /*
        if(m_lookup[opcode].name=="LDA"){
            std::cout<<"LDA "<<(int)opcode<<"\n";

        }*/
        uint8_t additional_cycle1 = (this->*m_lookup[opcode].addrmode)();
        uint8_t additional_cycle2 = (this->*m_lookup[opcode].operate)();
        cycles += (additional_cycle1 & additional_cycle2);
    }
    cycles--;
}

void Cpu6502::SetFlag(Cpu6502::FLAG6502 f, bool v)
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
uint8_t Cpu6502::GetFlag(FLAG6502 f)
{
    return ((status_r & f) > 0) ? 1 : 0;
}

void Cpu6502::check_unused_or_negative(uint8_t value)
{
    SetFlag(Z, value == 0x00);
    SetFlag(N, value & 0x80);
}

void Cpu6502::generalized_rotator(uint16_t temp, uint16_t carrier)
{
    SetFlag(C, fetched & carrier);
    check_unused_or_negative(temp & OFFSET_BYTE_PART);
    if (m_lookup[opcode].addrmode == &Cpu6502::IMP)
        acc_r = temp & OFFSET_BYTE_PART;
    else
        write(addr_abs, temp & OFFSET_BYTE_PART);
}