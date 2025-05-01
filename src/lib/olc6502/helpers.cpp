#include "bus.h++"
#include "olc6502.h++"
#include "utils.h++"
#include "consts.h++"


bool Olc6502::complete()
{
	return cycles == 0;
}

// This is the disassembly function. Its workings are not required for emulation.
// It is merely a convenience function to turn the binary instruction code into
// human readable form. Its included as part of the emulator because it can take
// advantage of many of the CPUs internal operations to do this.
std::map<uint16_t, std::string> Olc6502::disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convenient utility to convert variables into
	// hex strings because "modern C++"'s method with 
	// streams is atrocious
	auto hex = [](uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	// Starting at the specified address we read an instruction
	// byte, which in turn yields information from the lookup table
	// as to how many additional bytes we need to read and what the
	// addressing mode is. I need this info to assemble human readable
	// syntax, which is different depending upon the addressing mode

	// As the instruction is decoded, a std::string is assembled
	// with the readable output
	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		// Prefix line with instruction address
		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t opcode = m_bus->read(addr, true); addr++;
		sInst += m_lookup[opcode].name + " ";

		// Get oprands from desired locations, and form the
		// instruction based upon its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 6502 in order to get accurate data as part of the
		// instruction
		if (m_lookup[opcode].addrmode == &Olc6502::IMP)
		{
			sInst += " {IMP}";
		}
		else if (m_lookup[opcode].addrmode == &Olc6502::IMM)
		{
			value = m_bus->read(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (m_lookup[opcode].addrmode == &Olc6502::ZP0)
		{
			lo = m_bus->read(addr, true); addr++;
			hi = 0x00;												
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (m_lookup[opcode].addrmode == &Olc6502::ZPX)
		{
			lo = m_bus->read(addr, true); addr++;
			hi = 0x00;														
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (m_lookup[opcode].addrmode == &Olc6502::ZPY)
		{
			lo = m_bus->read(addr, true); addr++;
			hi = 0x00;														
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (m_lookup[opcode].addrmode == &Olc6502::IZX)
		{
			lo = m_bus->read(addr, true); addr++;
			hi = 0x00;								
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (m_lookup[opcode].addrmode == &Olc6502::IZY)
		{
			lo = m_bus->read(addr, true); addr++;
			hi = 0x00;								
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (m_lookup[opcode].addrmode == &Olc6502::ABS)
		{
			lo = m_bus->read(addr, true); addr++;
			hi = m_bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (m_lookup[opcode].addrmode == &Olc6502::ABX)
		{
			lo = m_bus->read(addr, true); addr++;
			hi = m_bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (m_lookup[opcode].addrmode == &Olc6502::ABY)
		{
			lo = m_bus->read(addr, true); addr++;
			hi = m_bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (m_lookup[opcode].addrmode == &Olc6502::IND)
		{
			lo = m_bus->read(addr, true); addr++;
			hi = m_bus->read(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (m_lookup[opcode].addrmode == &Olc6502::REL)
		{
			value = m_bus->read(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + value, 4) + "] {REL}";
		}

		// Add the formed string to a std::map, using the instruction's
		// address as the key. This makes it convenient to look for later
		// as the instructions are variable in length, so a straight up
		// incremental index is not sufficient.
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}