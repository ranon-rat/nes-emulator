#include "cpu.h++"

#include "utils.h++"
#include "consts.h++"

// addressing mode: implied
// this gets the data and then we use it in the ALU?
uint8_t Cpu6502::IMP()
{
    fetched = acc_r; // this can be working for the accumulator
    return 0;
};

// Addressing Mode immediate

// the data is supplied as part of the instruction, its going to be the next byte
uint8_t Cpu6502::IMM()
{
    // this will get the register and use it for our needs

    // it will read where to read the data from when it needs to?
    addr_abs = pc_r++;

    return 0;
}
// zero page address
uint8_t Cpu6502::ZP0()
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
uint8_t Cpu6502::ZPX()
{
    addr_abs = (read(pc_r) + x_r); // it will read with an offset
    pc_r++;
    addr_abs &= OFFSET_BYTE_PART;
    return 0;
}
// addressing mode: zero page y
uint8_t Cpu6502::ZPY()
{
    addr_abs = (read(pc_r) + y_r); // it will read with an offset
    pc_r++;
    addr_abs &= 0x00ff;
    return 0;
}
uint8_t Cpu6502::ABS()
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
uint8_t Cpu6502::ABX()
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
uint8_t Cpu6502::ABY()
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
uint8_t Cpu6502::IND()
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
uint8_t Cpu6502::IZX()
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
uint8_t Cpu6502::IZY()
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

uint8_t Cpu6502::REL()
{
    addr_rel = read(pc_r);
    pc_r++;
    if (addr_rel & 0x80)
    {
        addr_rel |= PAGE_BYTE_PART;
    }
    return 0;
}