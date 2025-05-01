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
// addressing mode: implied
// this gets the data and then we use it in the ALU?
uint8_t Olc6502::IMP()
{
    fetched = acc_r; // this can be working for the accumulator
    return 0;
};

// Addressing Mode immediate

// the data is supplied as part of the instruction, its going to be the next byte
uint8_t Olc6502::IMM()
{
    // this will get the register and use it for our needs

    // it will read where to read the data from when it needs to?
    addr_abs = pc_r++;

    return 0;
}
// zero page address
uint8_t Olc6502::ZP0()
{

    // 0x[ff][55]
    //     a   b
    // the a part of that byte is the page
    // the b part is the offset
    // a has 256 pages
    // b has 256 bytes

    // this means that it will try to find data somewhere in page 0
    addr_abs = read(pc_r); // so i get the instruction

    // then it will continue to the next instruction
    pc_r++; // i check for the next program

    // and then set an offset?
    // this will set the pages to 0

    addr_abs &= OFFSET_BYTE_PART; // and then i will just write abs, and uhh if the instruction was for example uhh

    return 0;
}
// addressing mode: zero page x
uint8_t Olc6502::ZPX()
{
    addr_abs = (read(pc_r) + x_r); // it will read with an offset
    pc_r++;
    addr_abs &= OFFSET_BYTE_PART;
    return 0;
}
// addressing mode: zero page y
uint8_t Olc6502::ZPY()
{
    addr_abs = (read(pc_r) + y_r); // it will read with an offset
    pc_r++;
    addr_abs &= 0x00ff;
}
uint8_t Olc6502::ABS()
{

    // so this is the offset
    uint16_t low_byte = read(pc_r); //
    pc_r++;
    // this is the page
    uint16_t high_byte = read(pc_r); // oh now i see,
    pc_r++;
    // high byte le estoy pasando un uint8
    // asi que la salida seria como de
    // 0b00011111
    // but we are trying to combine those values
    // so i have to move those values 8 bits to the left
    // and then combine it with the low byte
    // so, if the low byte is 0x00ff

    // and the high byte is
    // 0x00ee
    // then after combining it here
    // we would have 0xEEFF, sheesh, thats nice
    addr_abs |= Utils::combine2bytes(high_byte, low_byte);

    // i think that we write to the absolute address
    // after combining the high byte and the low byte
    // why do i need to move the high_byte 8 points? uhh
    return 0;
}

// Addressing mode:: Absolute with x offset
// we make the same, but we pass an offset to addr_abs
uint8_t Olc6502::ABX()
{
    // offset
    uint16_t low_byte = read(pc_r);
    pc_r++;

    // this is the page
    uint16_t high_byte = read(pc_r);
    pc_r++;
    addr_abs |= Utils::combine2bytes(high_byte, low_byte);

    addr_abs += x_r;

    // we need to check if the page have changed
    // if the high byte is different to the page part of
    // addr_abs then that means that we have an overflow error
    // if that happens what we do is to continue the cycle, because
    // that means that we need more
    if ((addr_abs & PAGE_BYTE_PART) != high_byte << 8)
    {
        return 1; // if that happens we continue the
    }
    // if its not the case, then there isnt really a big problem :)
    return 0;
}
// Addressing Mode: Absolute Y
// its the same as ABX but with the register Y
uint8_t Olc6502::ABY()
{
    uint16_t low_byte = read(pc_r);
    pc_r++;

    uint16_t high_byte = read(pc_r);
    pc_r++;
    addr_abs |= (high_byte << 8) | low_byte;

    addr_abs += y_r;

    if ((addr_abs & PAGE_BYTE_PART) != high_byte << 8)
    {
        return 1; // if that happens we continue the
    }
    // if its not the case, then there isnt really a big problem :)
    return 0;
}
// indirect, it uses pointer :)
uint8_t Olc6502::IND()
{

    uint16_t ptr_low = read(pc_r);
    pc_r++;
    uint16_t ptr_high = read(pc_r);
    pc_r++;
    // so we combine the high and low parts :)
    uint16_t ptr = Utils::combine2bytes(ptr_high, ptr_low);

    // then we need to read the pointer
    if (ptr_low == 0x00FF) // Simulate page boundary hardware bug
    {
        addr_abs = Utils::combine2bytes(read(ptr & PAGE_BYTE_PART), read(ptr + 0));
    }
    else // Behave normally
    {
        addr_abs = Utils::combine2bytes(read(ptr + 1), read(ptr + 0));
    }
    return 0;
}
// indirect addressing
uint8_t Olc6502::IZX()
{ // somewhere in the zero page
    uint16_t t = read(pc_r);
    pc_r++;

    // then we get the
    uint16_t low_byte = read((t + (uint16_t)x_r) & OFFSET_BYTE_PART);
    uint16_t high_byte = read((t + (uint16_t)x_r + 1) & OFFSET_BYTE_PART);

    addr_abs = Utils::combine2bytes(high_byte, low_byte);
    return 0;
}
// why both of this behave differently?
uint8_t Olc6502::IZY()
{

    uint16_t t = read(pc_r);
    pc_r++;

    // then we get the
    uint16_t low_byte = read((t)&OFFSET_BYTE_PART);
    uint16_t high_byte = read((t + 1) & OFFSET_BYTE_PART);

    addr_abs = Utils::combine2bytes(high_byte, low_byte);
    addr_abs += y_r;
    if ((addr_abs << 8) != high_byte)
    {
        return 1;
    }
    return 0;
}

uint8_t Olc6502::REL()
{
    addr_rel = read(pc_r);
    pc_r++;
    if (addr_rel & 0x80)
    {
        addr_rel |= PAGE_BYTE_PART;
    }
    return 0;
}
// INSTRUCTIONS
uint8_t Olc6502::fetch()
{
    if (!(m_lookup[opcode].addrmode == &Olc6502::IMP))
    {
        fetched = read(addr_abs);
    }
    return fetched;
}

uint8_t Olc6502::AND()
{
    fetch();
    acc_r = acc_r & fetched;
    SetFlag(Z, acc_r == 0x00);
    SetFlag(N, acc_r & 0x80);
    return 1;
}

void Olc6502::general_branching()
{
    cycles++;
    addr_abs = pc_r + addr_rel;
    if ((addr_abs & PAGE_BYTE_PART) != (pc_r & PAGE_BYTE_PART))
        cycles++;
    pc_r = addr_abs;
}
// branching instruction
uint8_t Olc6502::BCS()
{
    if (GetFlag(C) == 1)
        general_branching();

    return 0;
}

// Instruction: Branch if Carry Clear
// Function:    if(C == 0) pc = address
uint8_t Olc6502::BCC()
{
    if (GetFlag(C) == 0)
        general_branching();
    return 0;
}

// branching instruction
uint8_t Olc6502::BEQ()
{
    if (GetFlag(Z) == 1) // branch if zero
        general_branching();
    return 0;
}

// BMI
uint8_t Olc6502::BMI()
{

    if (GetFlag(N) == 1) // branch if negative
        general_branching();
    return 0;
}
uint8_t Olc6502::BPL()
{
    if (GetFlag(N) == 0)
        general_branching(); // branch if positive
    return 0;
}

// branch if not equal
uint8_t Olc6502::BNE()
{
    if (GetFlag(Z) == 0)
        general_branching();
    return 0;
}
// branch if overflow clear
uint8_t Olc6502::BVC()
{
    // branch if overflow
    if (GetFlag(V) == 0)
        general_branching();
    return 0;
}
uint8_t Olc6502::BVS()
{
    if (GetFlag(V) == 1)
        general_branching();
    return 0;
}

// CLEAR INSTRUCTIONS
// clear the carry bit
uint8_t Olc6502::CLC()
{
    SetFlag(C, false);
    return 0;
}
// clear decimal mode
uint8_t Olc6502::CLD()
{
    SetFlag(D, false);
    return 0;
}
// clear interrupter
uint8_t Olc6502::CLI()
{
    SetFlag(I, false);
    return 0;
}
// clear unused
uint8_t Olc6502::CLV()
{
    SetFlag(V, false);
    return 0;
}

// ADD

uint8_t Olc6502::ADC()
{
    fetch();
    uint16_t temp = (uint16_t)acc_r + (uint16_t)fetched + (uint16_t)GetFlag(C);
    SetFlag(C, temp > 255);
    SetFlag(Z, (temp & OFFSET_BYTE_PART) == 0);
    SetFlag(N, temp & 0x80);

    //       Positive Number + Positive Number = Negative Result -> Overflow
    //       Negative Number + Negative Number = Positive Result -> Overflow
    //       Positive Number + Negative Number = Either Result -> Cannot Overflow
    //       Positive Number + Positive Number = Positive Result -> OK! No Overflow
    //       Negative Number + Negative Number = Negative Result -> OK! NO Overflow

    SetFlag(V, (~((uint16_t)acc_r ^ (uint16_t)fetched) & ((uint16_t)acc_r ^ (uint16_t)temp)) & 0x0080);
    acc_r = temp & OFFSET_BYTE_PART;
    return 1;
}
// subtract
uint8_t Olc6502::SBC()
{

    fetch();

    // then i invert the value
    uint16_t value = ((uint16_t)fetched) ^ OFFSET_BYTE_PART;

    uint16_t temp = (uint16_t)acc_r + (uint16_t)value + (uint16_t)GetFlag(C);
    SetFlag(C, temp > 255);
    SetFlag(Z, (temp & OFFSET_BYTE_PART) == 0);
    SetFlag(N, temp & 0x80);
    SetFlag(V, (~((uint16_t)acc_r ^ (uint16_t)value) & ((uint16_t)acc_r ^ (uint16_t)temp)) & 0x0080);
    acc_r = temp & OFFSET_BYTE_PART;

    // first if etch the data
    return 1;
}

uint8_t Olc6502::PHA()
{ // adds accumulator to the stack
    write(0x0100 + stkp_r, acc_r);
    stkp_r--;
    return 0;
}
uint8_t Olc6502::PLA()
{
    stkp_r++;
    acc_r = read(0x0100 + stkp_r);
    SetFlag(Z, acc_r == 0x00);
    SetFlag(Z, acc_r & 0x80);
    return 0;
}

void Olc6502::reset()
{
    acc_r = 0;
    x_r = 0;
    y_r = 0;
    stkp_r = 0xFD;
    status_r = 0x00 | U;
    addr_abs = 0xFFFC;
    uint16_t low = read(addr_abs);
    uint16_t high = read(addr_abs + 1);
    pc_r = Utils::combine2bytes(high, low);
    addr_rel = 0x00;
    addr_abs = 0x00;
    fetched = 0x00;
    cycles = 8;
}

void Olc6502::general_interrupt_request(uint16_t value,uint8_t c)
{

    write(0x0100 + stkp_r, (pc_r >> 8) & OFFSET_BYTE_PART);
    stkp_r--;
    write(0x0100 + stkp_r, pc_r & OFFSET_BYTE_PART);
    stkp_r--;
    SetFlag(B, 0);
    SetFlag(U, 1);
    SetFlag(I, 1);
    write(0x0100 + stkp_r, status_r);
    stkp_r--;
    addr_abs = value;
    uint16_t low = read(addr_abs);
    uint16_t high = read(addr_abs + 1);
    pc_r = Utils::combine2bytes(high, low);
    cycles = c;
}

void Olc6502::irq()
{
    if (GetFlag(I) == 0)
        general_interrupt_request(0xfffe,7);
}
void Olc6502::nmi()
{
    general_interrupt_request(0xfffa,8);
}

uint8_t  Olc6502::RTI(){
    stkp_r++;
    status_r=read(0x0100+stkp_r);
    status_r&=~B;
    status_r&=~U;
    stkp_r++;
    pc_r=(uint16_t)read(0x0100+stkp_r);
    stkp_r++;
    pc_r|=(uint16_t)read(0x100+stkp_r)<<8;


}