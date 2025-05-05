#include <fstream>

#include "cartridge.h++"
#include "consts.h++"
Cartridge::Cartridge(const std::string &filename)
{
    struct sHeader header;

    std::ifstream ifs;
    ifs.open(filename, std::ifstream::binary);
    if (ifs.is_open())
    {
        ifs.read((char *)&header, sizeof(sHeader));
        if (header.mapper1 & 0x04)
        {
            ifs.seekg(512, std::ios_base::cur);
        }
        nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
        mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

        uint8_t nFileType = 1;
        if (nFileType == 0)
        {
        }
        if (nFileType == 1)
        {
            nPRGBanks = header.pgr_rom_chunks;
            m_vPRGMemory.resize(nPRGBanks * 16 * KILOBYTE_SIZE);
            ifs.read((char *)m_vPRGMemory.data(), m_vPRGMemory.size());
            nCHRBanks = header.chr_rom_chunks;
            m_vCHRMemory.resize(nCHRBanks * 8 * KILOBYTE_SIZE);
            ifs.read((char *)m_vCHRMemory.data(), m_vCHRMemory.size());
        }
        if (nFileType == 2)
        {
        }
        switch (nMapperID)
        {
        case 0:
            m_mapper = std::make_shared<Mapper000>(nPRGBanks, nCHRBanks);
            break;
        }
        ifs.close();
    }
}