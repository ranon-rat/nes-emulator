#include "mapper_000.h++"
#include "consts.h++"

// Note: Since constructor and destructor are defined inline in the header, 
// we don't need to redefine them here

bool Mapper000::cpuMapRead(uint16_t addr, uint32_t &mapped_addr)
{
    if (addr >= 0x8000 && addr <= 0xffff)
    {
        mapped_addr = addr & (nPRGBanks > 1 ? 32 * KILOBYTE_SIZE - 1 : 16 * KILOBYTE_SIZE - 1);
        return true;
    }

    return false;
}

bool Mapper000::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr)
{
    if (addr >= 0x8000 && addr <= 0xffff)
    {
        mapped_addr = addr & (nPRGBanks > 1 ? 32 * KILOBYTE_SIZE - 1 : 16 * KILOBYTE_SIZE - 1);
        return true;
    }
    return false;
}

bool Mapper000::ppuMapRead(uint16_t addr, uint32_t &mapped_addr)
{
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        mapped_addr = addr;
        return true;
    }

    return false;
}

bool Mapper000::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr)
{
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        if (nCHRBanks == 0)
        {
            // Treat as RAM
            mapped_addr = addr;
            return true;
        }
    }

    return false;
}