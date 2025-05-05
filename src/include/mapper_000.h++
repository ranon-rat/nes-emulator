#ifndef MAPPER_000_H
#define MAPPER_000_H
#include <cstdint>
#include "mapper.h++"
class Mapper000 : public Mapper
{
public:
    Mapper000(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks) {}
    ~Mapper000() {}

public:
    bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
    bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;
    bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
    bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;
};
#endif