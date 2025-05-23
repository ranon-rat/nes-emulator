#ifndef MAPPER_HEADER
#define MAPPER_HEADER
#include <cstdint>
class Mapper
{
public:
    Mapper(uint8_t prgBanks, uint8_t chrBanks) : nPRGBanks(prgBanks), nCHRBanks(chrBanks)
    {
    }
    ~Mapper() {};

public:
    virtual bool cpuMapRead(uint16_t addr, uint32_t &mapper_addr) = 0;
    virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapper_addr) = 0;
    virtual bool ppuMapRead(uint16_t addr, uint32_t &mapper_addr) = 0;
    virtual bool ppuMapWrite(uint16_t addr, uint32_t &mapper_addr) = 0;

protected:
    uint8_t nPRGBanks = 0x00;
    uint8_t nCHRBanks = 0x00;
};
#endif
