#include "ppu.h++"
#include "consts.h++"
#include <iostream>
#include <bitset>
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Pppu2c02::cpuWrite(uint16_t addr, uint8_t data)
{
    switch (static_cast<PPU_addresses>(addr))
    {
    case PPU_addresses::CONTROLL: // controll
        control.reg = data;
        break;
    case PPU_addresses::MASK: //  mask
        mask.reg = data;
        break;
    case PPU_addresses::STATUS: // status
        break;
    case PPU_addresses::OAM_ADDRESS: // oam address
        break;
    case PPU_addresses::OAM_DATA: // oam data
        break;
    case PPU_addresses::SCROLL: // scroll
        break;
    case PPU_addresses::PPU_ADDRESS: // ppu address
        if (address_latch == 0)
        {
            ppu_address = (ppu_address & 0x00ff) | (uint16_t)(data << 8);
            address_latch = 1;
        }
        else
        {
            ppu_address = (ppu_address & 0xff00) | (uint16_t)(data);
            address_latch = 0;
        }
        break;
    case PPU_addresses::PPU_DATA: // PPU data
        ppuWrite(addr, data);
        break;
    }
}
uint8_t Pppu2c02::cpuRead(uint16_t addr, bool bReadOnly)
{

    uint8_t data = 0x00;
    switch (static_cast<PPU_addresses>(addr))
    {
    case PPU_addresses::CONTROLL: // controll
        break;
    case PPU_addresses::MASK: //  mask
        break;
    case PPU_addresses::STATUS: // status
     status.vertical_blank = 1;
    
        data = (status.reg & 0xe0) | (ppu_data_buffer & 0x1f);
        address_latch = 0;
        break;
    case PPU_addresses::OAM_ADDRESS: // oam address
        break;
    case PPU_addresses::OAM_DATA: // oam data
        break;
    case PPU_addresses::SCROLL: // scroll
        break;
    case PPU_addresses::PPU_ADDRESS: // ppu address
        break;
    case PPU_addresses::PPU_DATA: // PPU data
        data = ppu_data_buffer;
        ppu_data_buffer = ppuRead(ppu_address);
        if (ppu_address > 0x3f00)
            data = ppu_data_buffer;
        break;
    }
    return data;
}

void Pppu2c02::ppuWrite(uint16_t addr, uint8_t data)
{
    addr &= END_ACCESS_PPU;
    if (cart->ppuWrite(addr, data))
    {
    }
    // pattern memory
    else if (addr >= 0x0000 && addr < 0x1fff)
    {
		tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;

    } // name table memory?
    else if (addr >= 0x2000 && addr < 0x3eff)
    {
        addr &= 0x0FFF;
		if (cart->mirror == Cartridge::MIRROR::VERTICAL)
		{
			// Vertical
			if (addr >= 0x0000 && addr <= 0x03FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				tblName[1][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				tblName[1][addr & 0x03FF] = data;
		}
		else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL)
		{
			// Horizontal
			if (addr >= 0x0000 && addr <= 0x03FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				tblName[1][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				tblName[1][addr & 0x03FF] = data;
		}

    } // palette memory
    else if (addr >= 0x3f00 && addr > 0x3fff)
    {
       addr &= 0x001F;
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;
		tblPalette[addr] = data;
    }
}
uint8_t Pppu2c02::ppuRead(uint16_t addr, bool bReadOnly)
{
    uint8_t data = 0x00;
    addr &= END_ACCESS_PPU;
    if (cart->ppuRead(addr, data))
    {
    }
    // pattern memory
    else if (addr >= 0x0000 && addr < 0x1fff)
    {
        data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0fff];

    } // name table memory?
    else if (addr >= 0x2000 && addr < 0x3eff)
    {
        addr &= 0x0FFF;

		if (cart->mirror == Cartridge::MIRROR::VERTICAL)
		{
			// Vertical
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = tblName[1][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = tblName[1][addr & 0x03FF];
		}
		else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL)
		{
			// Horizontal
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = tblName[1][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = tblName[1][addr & 0x03FF];
		}

    } // palette memory
    else if (addr >= 0x3f00 && addr > 0x3fff)
    {
        addr &= 0x001F;
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;
		data = tblPalette[addr] & (mask.grayscale ? 0x30 : 0x3F);
    }

    return data;
}
