#include <iostream>
#include <raylib/raylib.h>
#include <iostream>
#include <sstream>
#include <string>
#include "bus.h++"
#include "utils.h++"
#define FONT_SIZE 8
class Demo_Cpu6502
{
public:
  Demo_Cpu6502()
  {
    unscii = LoadFontEx("font/unscii-8.ttf", 8, nullptr, 0);
    SetWindowTitle("Cpu6502 Demonstration");
  }
  std::shared_ptr<Cartridge> cart;
  Bus nes;
  std::map<uint16_t, std::string> mapAsm;
  Font unscii;
  bool bEmulationRun = false;
  float residualTime = 0.0f;
  uint8_t nSelectedPalette = 0x00;

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
        sOffset += " " + hex(nes.cpuRead(nAddr, true), 2);
        nAddr += 1;
      }
      DrawString(sOffset, nRamX, nRamY, FONT_SIZE, BLACK);
      nRamY += FONT_SIZE;
    }
  }
  void DrawTextI(std::string text, int x, int y, int fontSize, Color color)
  {
    DrawTextEx(unscii, text.c_str(), {(float)x, (float)y}, fontSize, 0.5, color);
  }
  void DrawString(std::string text, int x, int y, int fontSize, Color color)
  {
    DrawTextEx(unscii, text.c_str(), {(float)x, (float)y}, fontSize, 0.5, color);
  }

  void DrawCpu(int x, int y)
  {
    std::string status = "STATUS: ";
    this->DrawTextI("STATUS:", x, y, FONT_SIZE, BLACK);
    this->DrawTextI("N", x + 64, y, FONT_SIZE, nes.cpu.status_r & Cpu6502::N ? GREEN : RED);
    this->DrawTextI("N", x + 64, y, FONT_SIZE, nes.cpu.status_r & Cpu6502::N ? GREEN : RED);
    this->DrawTextI("V", x + 80, y, FONT_SIZE, nes.cpu.status_r & Cpu6502::V ? GREEN : RED);
    this->DrawTextI("-", x + 96, y, FONT_SIZE, nes.cpu.status_r & Cpu6502::U ? GREEN : RED);
    this->DrawTextI("B", x + 112, y, FONT_SIZE, nes.cpu.status_r & Cpu6502::B ? GREEN : RED);
    this->DrawTextI("D", x + 128, y, FONT_SIZE, nes.cpu.status_r & Cpu6502::D ? GREEN : RED);
    this->DrawTextI("I", x + 144, y, FONT_SIZE, nes.cpu.status_r & Cpu6502::I ? GREEN : RED);
    this->DrawTextI("Z", x + 160, y, FONT_SIZE, nes.cpu.status_r & Cpu6502::Z ? GREEN : RED);
    this->DrawTextI("C", x + 178, y, FONT_SIZE, nes.cpu.status_r & Cpu6502::C ? GREEN : RED);

    DrawString("PC: $" + hex(nes.cpu.pc_r, 4), x, y + FONT_SIZE, FONT_SIZE, BLACK);
    DrawString("A: $" + hex(nes.cpu.acc_r, 2) + "  [" + std::to_string(nes.cpu.acc_r) + "]", x, y + 20, FONT_SIZE, BLACK);
    DrawString("X: $" + hex(nes.cpu.x_r, 2) + "  [" + std::to_string(nes.cpu.x_r) + "]", x, y + 30, FONT_SIZE, BLACK);
    DrawString("Y: $" + hex(nes.cpu.y_r, 2) + "  [" + std::to_string(nes.cpu.y_r) + "]", x, y + 40, FONT_SIZE, BLACK);
    DrawString("Stack P: $" + hex(nes.cpu.stkp_r, 4), x, y + 50, FONT_SIZE, BLACK);
  }

  void DrawCode(int x, int y, int nLines)
  {
    auto it_a = mapAsm.find(nes.cpu.pc_r);

    int nLineY = (nLines >> 1) * FONT_SIZE + y;
    if (it_a != mapAsm.end())
    {
      DrawString((*it_a).second, x, nLineY, FONT_SIZE, BLACK);
      while (nLineY < (nLines * FONT_SIZE) + y)
      {
        nLineY += FONT_SIZE;
        if (++it_a != mapAsm.end())
        {

          DrawString((*it_a).second, x, nLineY, FONT_SIZE, BLACK);
        }
      }
    }

    it_a = mapAsm.find(nes.cpu.pc_r);
    nLineY = (nLines >> 1) * FONT_SIZE + y;
    if (it_a != mapAsm.end())
    {
      while (nLineY > y)
      {
        nLineY -= FONT_SIZE;
        if (--it_a != mapAsm.end())
        {
          DrawString((*it_a).second, x, nLineY, FONT_SIZE / 1, {.r = 150, .g = 0, .b = 150, .a = 255});
        }
      }
    }
  }

  void Setup()
  {
    cart = std::make_shared<Cartridge>("test-files/nestest.nes");
    nes.insertCartridge(cart);
    mapAsm = nes.cpu.disassemble(0x0000, 0xffff);
    nes.reset();
  }

  void Draw(float deltaTime)
  {
    BeginDrawing();

    ClearBackground({.r=240,.g=240,.b=240,.a=255,});
    if (bEmulationRun)
    {
      if (residualTime > 0.0f)
      {
        residualTime -= deltaTime;
      }
      else
      {
        residualTime = 1.0f / 60.0f - deltaTime;
        do
        {
          nes.clock();
        } while (!nes.ppu.frame_complete);
        nes.ppu.frame_complete = false;
      }
    }
    else
    {
      if (IsKeyPressed(KEY_C))
      {
        do
        {
          nes.clock();
        } while (!nes.cpu.complete());
        do
        {
          nes.clock();
        } while (nes.cpu.complete());
      }
      if (IsKeyPressed(KEY_F))
      {
        do
        {
          nes.clock();
        } while (!nes.ppu.frame_complete);
        do
        {
          nes.clock();
        } while (!nes.cpu.complete());
        nes.ppu.frame_complete = false;
      }
    }
    if (IsKeyPressed(KEY_R))
      nes.reset();
    if (IsKeyPressed(KEY_SPACE))
      bEmulationRun = !bEmulationRun;

    if (IsKeyPressed(KEY_P))
    {
      (++nSelectedPalette) &= 0x07;
    }

    DrawCpu(516, 2);
    DrawCode(516, 72, 26);

    Texture2D a1 = DrawSprite(0, 0, nes.ppu.GetScreen(),2);
    Texture2D a2 = DrawSprite(516, 348, nes.ppu.GetPatternTable(0, nSelectedPalette),1);
    Texture2D a3 = DrawSprite(516+128+10, 348, nes.ppu.GetPatternTable(1, nSelectedPalette),1);
    defer([&]
          {
            UnloadTexture(a1);
            UnloadTexture(a2);

            UnloadTexture(a3); });

    const int nSwatchSize = 6;
    for (int p = 0; p < 8; p++)
    {
      for (int s = 0; s < 4; s++)
      {
        DrawRectangle(516 + p * (nSwatchSize * 5) + s * nSwatchSize, 340, nSwatchSize, nSwatchSize, nes.ppu.GetColorFromPaletteRam(p, s));
      }
    }
    DrawRectangle(516+nSelectedPalette*(nSwatchSize*5)-1,339,nSwatchSize*4,nSwatchSize,WHITE);
    EndDrawing();
  }

  Texture2D DrawSprite(float x, float y, Image image,float scale)
  {
    Texture2D sprite = LoadTextureFromImage(image);

    DrawTextureEx(sprite, {.x = x, .y = y}, 0.0f, scale, WHITE);
    return sprite;
  }
};

int main()
{
  const int screenWidth = 800;
  const int screenHeight = 580;

  InitWindow(screenWidth, screenHeight, "Cpu6502 Demonstration");
  Demo_Cpu6502 demo;
  SetTargetFPS(60);
  SetTraceLogLevel(LOG_NONE);

  demo.Setup();

  while (!WindowShouldClose())

  {
    demo.Draw(GetFrameTime());
  }
  CloseWindow();

  return 0;
}