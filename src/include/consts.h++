#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H
// here goes the  macros :)
#define KILOBYTE_SIZE 1024

#define PAGE_BYTE_PART 0xff00
#define OFFSET_BYTE_PART 0x00ff
#define STACK_MEMORY_BASE 0x0100

#define BEGIN_ACCESS_RAM 0x0000
#define END_ACCESS_RAM 0x1fff

#define BEGIN_ACCESS_PPU 0x2000
#define END_ACCESS_PPU 0x3fff
// here the enum for the PPU address :)
enum class PPU_addresses
{
    CONTROLL = 0x0000,
    MASK,
    STATUS,
    OAM_ADDRESS,
    OAM_DATA,
    SCROLL,
    PPU_ADDRESS,
    PPU_DATA, // 0b111

};

#endif