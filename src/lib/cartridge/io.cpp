#include "cartridge.h++"
bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
    uint32_t mapped_addr = 0;
    if (m_mapper->cpuMapWrite(addr, mapped_addr))
    {
        m_vPRGMemory[mapped_addr] = data;
    }
    return false;
}
bool Cartridge::cpuRead(uint16_t addr, uint8_t &data)
{

    uint32_t mapped_addr = 0;
    if (m_mapper->cpuMapRead(addr, mapped_addr))
    {
        data = m_vPRGMemory[mapped_addr];
        return true;
    }

    return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
    uint32_t mapped_addr = 0;
    if (m_mapper->ppuMapWrite(addr, mapped_addr))
    {
        m_vCHRMemory[mapped_addr] = data;
        return true;
    }

    return false;
}
bool Cartridge::ppuRead(uint16_t addr, uint8_t &data)
{

    uint32_t mapped_addr = 0;
    if (m_mapper->ppuMapRead(addr, mapped_addr))
    {
        data = m_vPRGMemory[mapped_addr];
        return true;
    }

    return false;
}
