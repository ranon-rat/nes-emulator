#include <iostream>
#include <raylib/raylib.h>
#include <iostream>
#include <sstream>
#include <string>

#include "bus.h++"


void DrawString(std::string text, int x,int y, int fontSize, Color color){
    DrawText(text.c_str(),x,y,fontSize,color);
}

class Demo_olc6502 
{
public:
	Demo_olc6502() { 
    
    SetWindowTitle("olc6502 Demonstration");
  }

	Bus nes;
	std::map<uint16_t, std::string> mapAsm;

	std::string hex(uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns)
	{
		int nRamX = x, nRamY = y;
		for (int row = 0; row < nRows; row++)
		{
			std::string sOffset = "$" + hex(nAddr, 4) + ":";
			for (int col = 0; col < nColumns; col++)
			{
				sOffset += " " + hex(nes.read(nAddr, true), 2);
				nAddr += 1;
			
      DrawString(sOffset,nRamX,nRamY,10,BLACK);
			nRamY += 10;
		}
	}
}

	void DrawCpu(int x, int y)
	{
		std::string status = "STATUS: ";
		DrawText("STATUS:",x , y ,10, BLACK);
    DrawText("N",x  + 64, y,10, nes.cpu.status_r & Olc6502::N ? GREEN : RED);
		DrawText("N",x  + 64, y, 10, nes.cpu.status_r & Olc6502::N ? GREEN : RED);
		DrawText("V",x  + 80, y ,10, nes.cpu.status_r & Olc6502::V ? GREEN : RED);
		DrawText("-",x  + 96, y ,10, nes.cpu.status_r & Olc6502::U ? GREEN : RED);
		DrawText("B",x  + 112, y,10, nes.cpu.status_r & Olc6502::B ? GREEN : RED);
		DrawText("D",x  + 128, y,10, nes.cpu.status_r & Olc6502::D ? GREEN : RED);
		DrawText("I",x  + 144, y,10, nes.cpu.status_r & Olc6502::I ? GREEN : RED);
		DrawText("Z",x  + 160, y,10, nes.cpu.status_r & Olc6502::Z ? GREEN : RED);
		DrawText("C",x  + 178, y,10, nes.cpu.status_r & Olc6502::C ? GREEN : RED);

		DrawString("PC: $" + hex(nes.cpu.pc_r, 4) ,x , y + 10,10,BLACK );
		DrawString("A: $"  +  hex(nes.cpu.acc_r, 2) + "  [" + std::to_string(nes.cpu.acc_r) + "]",x , y + 20,10,BLACK);
		DrawString("X: $"  +  hex(nes.cpu.x_r, 2) + "  [" + std::to_string(nes.cpu.x_r) + "]",x , y + 30,10,BLACK);
		DrawString("Y: $"  +  hex(nes.cpu.y_r, 2) + "  [" + std::to_string(nes.cpu.y_r) + "]",x , y + 40,10,BLACK);
		DrawString("Stack P: $" + hex(nes.cpu.stkp_r, 4),x , y + 50,10,BLACK);
	}

	void DrawCode(int x, int y, int nLines)
	{
		auto it_a = mapAsm.find(nes.cpu.pc_r);
		int nLineY = (nLines >> 1) * 10 + y;
		if (it_a != mapAsm.end())
		{
			DrawString((*it_a).second,x, nLineY,10, BLACK);
			while (nLineY < (nLines * 10) + y)
			{
				nLineY += 10;
				if (++it_a != mapAsm.end())
				{
					DrawString((*it_a).second,x, nLineY,10, BLACK );
				}
			}
		}

		it_a = mapAsm.find(nes.cpu.pc_r);
		nLineY = (nLines >> 1) * 10 + y;
		if (it_a != mapAsm.end())
		{
			while (nLineY > y)
			{
				nLineY -= 10;
				if (--it_a != mapAsm.end())
				{
					DrawString((*it_a).second,x, nLineY,10,BLACK);
				}
			}
		}
	}

	bool OnUserCreate()
	{
		// Load Program (assembled at https://www.masswerk.at/6502/assembler.html)
		/*
			*=$8000
			LDX #10
			STX $0000
			LDX #3
			STX $0001
			LDY $0000
			LDA #0
			CLC
			loop
			ADC $0001
			DEY
			BNE loop
			STA $0002
			NOP
			NOP
			NOP
		*/
		
		// Convert hex string into bytes for RAM
		std::stringstream ss;
		ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
		uint16_t nOffset = 0x8000;
		while (!ss.eof())
		{
			std::string b;
			ss >> b;
			nes.ram[nOffset++] = (uint8_t)std::stoul(b, nullptr, 16);
		}

		// Set Reset Vector
		nes.ram[0xFFFC] = 0x00;
		nes.ram[0xFFFD] = 0x80;

		// Dont forget to set IRQ and NMI vectors if you want to play with those
				
		// Extract dissassembly
		mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

		// Reset
		nes.cpu.reset();
		return true;
	}

	bool OnUserUpdate()
	{
    ClearBackground(RAYWHITE);


		if (IsKeyPressed(KEY_SPACE))
		{
			do
			{
				nes.cpu.clock();
			} 
			while (!nes.cpu.complete());
		}
  
		if (IsKeyPressed(KEY_R))
			nes.cpu.reset();

		if (IsKeyPressed(KEY_I))
			nes.cpu.irq();

		if (IsKeyPressed(KEY_N))
			nes.cpu.nmi();

		// Draw Ram Page 0x00		
		DrawRam(2, 2, 0x0000, 16, 16);
		DrawRam(2, 182, 0x8000, 16, 16);
		DrawCpu(448, 2);
		DrawCode(448, 72, 26);


		DrawString("SPACE = Step Instruction    R = RESET    I = IRQ    N = NMI",10, 370,10,BLACK);

		return true;
	}
};

int main() {
  const int screenWidth = 680;
  const int screenHeight = 480;
  InitWindow(screenWidth, screenHeight, "olc6502 Demonstration");
  Demo_olc6502 demo;
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    demo.OnUserUpdate();
    EndDrawing();
  }
  CloseWindow();
  return 0;
}