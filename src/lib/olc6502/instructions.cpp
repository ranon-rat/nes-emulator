
#include "olc6502.h++"
#include "consts.h++"
#include "utils.h++"
// INSTRUCTIONS
uint8_t Olc6502::fetch()
{
    if (!(m_lookup[opcode].addrmode == &Olc6502::IMP))
    {
        fetched = read(addr_abs);
    }
    return fetched;
}
// logical gates
uint8_t Olc6502::AND()
{
    fetch();
    acc_r = acc_r & fetched;
    SetFlag(Z, acc_r == 0x00);
    SetFlag(N, acc_r & 0x80);
    return 1;
}

uint8_t Olc6502::ORA()
{
    fetch();
    acc_r = acc_r | fetched;
    SetFlag(Z, acc_r == 0x00);
    SetFlag(N, acc_r & 0x80);
    return 1;
}
uint8_t Olc6502::EOR()
{
    fetch();
    acc_r = acc_r ^ fetched;
    SetFlag(Z, acc_r == 0x00);
    SetFlag(N, acc_r & 0x80);
    return 1;
}
uint8_t Olc6502::NOP()
{
    // TODO: IMPLEMENT THIS LATER
    return 0;
}

uint8_t Olc6502::ASL()
{

    fetch();
    uint16_t temp = (uint16_t)fetched << 1;
    SetFlag(C, (temp & PAGE_BYTE_PART) > 0);
    SetFlag(Z, (temp & OFFSET_BYTE_PART) == 0x00);
    SetFlag(N, temp & 0x80);
    if (m_lookup[opcode].addrmode == &Olc6502::IMP)
        acc_r = temp & OFFSET_BYTE_PART;
    else
        write(addr_abs, temp & OFFSET_BYTE_PART);
    return 0;
}

uint8_t Olc6502::INC()
{
    fetch();
    uint16_t temp = fetched + 1;
    write(addr_abs, temp & OFFSET_BYTE_PART);
    SetFlag(Z, (temp & OFFSET_BYTE_PART) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}

uint8_t Olc6502::INX()
{
    x_r++;
    SetFlag(Z, x_r == 0x00);
    SetFlag(N, x_r & 0x80);
    return 0;
}

uint8_t Olc6502::INY()
{
    y_r++;
    SetFlag(Z, y_r == 0x00);
    SetFlag(N, y_r & 0x80);
    return 0;
}

uint8_t Olc6502::JMP()
{
    pc_r = addr_abs;
    return 0;
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
    write(STACK_MEMORY_BASE + stkp_r, acc_r);
    stkp_r--;
    return 0;
}

uint8_t Olc6502::PLA()
{
    stkp_r++;
    acc_r = read(STACK_MEMORY_BASE + stkp_r);
    SetFlag(Z, acc_r == 0x00);
    SetFlag(Z, acc_r & 0x80);
    return 0;
}
// push status into the stack

uint8_t Olc6502::PHP()
{

    write(STACK_MEMORY_BASE + stkp_r, status_r | B | U);
    SetFlag(B, 0);
    SetFlag(U, 0);
    stkp_r--;
    return 0;
    return 0;
}

uint8_t Olc6502::PLP()
{
    stkp_r++;
    status_r = read(STACK_MEMORY_BASE + stkp_r);
    SetFlag(U, 1);
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

void Olc6502::general_interrupt_request(uint16_t value, uint8_t c)
{

    write(STACK_MEMORY_BASE + stkp_r, (pc_r >> 8) & OFFSET_BYTE_PART);
    stkp_r--;
    write(STACK_MEMORY_BASE + stkp_r, pc_r & OFFSET_BYTE_PART);
    stkp_r--;
    SetFlag(B, 0);
    SetFlag(U, 1);
    SetFlag(I, 1);
    write(STACK_MEMORY_BASE + stkp_r, status_r);
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
        general_interrupt_request(0xfffe, 7);
}
void Olc6502::nmi()
{
    general_interrupt_request(0xfffa, 8);
}

uint8_t Olc6502::RTI()
{
    stkp_r++;
    status_r = read(STACK_MEMORY_BASE + stkp_r);
    status_r &= ~B;
    status_r &= ~U;
    stkp_r++;
    pc_r = (uint16_t)read(STACK_MEMORY_BASE + stkp_r);
    stkp_r++;
    pc_r |= (uint16_t)read(STACK_MEMORY_BASE + stkp_r) << 8;
    return 0;
}

uint8_t Olc6502::XXX()
{
    return 0;
}

uint8_t Olc6502::BIT()
{
    fetch();
    uint16_t temp = acc_r & fetched;
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, fetched & (1 << 7));
    SetFlag(V, fetched & (1 << 6));
    return 0;
}
