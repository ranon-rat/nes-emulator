#pragma once
#ifndef CARTRIDGE_NIG
#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include "mapper_000.h++"

class Cartridge
{
    
public:
    Cartridge(const std::string &filename);
    ~Cartridge() {}
private:


    std::vector<uint8_t> m_vPRGMemory;
    std::vector<uint8_t> m_vCHRMemory;

    uint8_t nMapperID=0;
    uint8_t nPRGBanks=0;
    uint8_t nCHRBanks=0;

    struct sHeader{
        char name[4];
        uint8_t pgr_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t pgr_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    };
    
	enum MIRROR
	{
		HORIZONTAL,
		VERTICAL,
		ONESCREEN_LO,
		ONESCREEN_HI,
	} mirror = HORIZONTAL;

    std::shared_ptr<Mapper> m_mapper;
public:
    bool cpuWrite(uint16_t addr, uint8_t data);
    bool cpuRead(uint16_t addr,uint8_t &data);

public:
    bool ppuWrite(uint16_t addr, uint8_t data);
    bool ppuRead(uint16_t addr,uint8_t &data);


};
#endif