#include "ppu.h++"
#include <iostream>
#include <bitset>
Image Pppu2c02::GetScreen()
{
    return {
        .data = sprScreen.data(),
        .width = SCREEN_WIDTH,
        .height = SCREEN_HEIGHT,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
};

Image Pppu2c02::GetNameTable(uint8_t i)
{
    return {
        .data = sprNameTable[i].data(),
        .width = SCREEN_WIDTH,
        .height = SCREEN_HEIGHT,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
}

Image Pppu2c02::GetPatternTable(uint8_t i,uint8_t palette)
{
    for (uint16_t nTileY = 0; nTileY < 16; nTileY++)
    {
        for (uint16_t nTileX = 0; nTileX < 16; nTileX++)
        {
            uint16_t nOffset  = nTileY * 256 + nTileX * 16; // 16 colors?
            // 16 bytes of information
            for (uint16_t row = 0; row < 8; row++)
            {
                uint8_t tile_lsb = ppuRead(i * 0x1000 + nOffset + row+0);
                uint8_t tile_msb = ppuRead(i * 0x1000 + nOffset + row+8);

                for (uint16_t col = 0; col < 8; col++)
                {

                    uint8_t pixel=(tile_lsb&0x01)+(tile_msb&0x01);
                    tile_lsb>>=1;
                    tile_msb>>=1;
                    if(scanline>261){


//                        std::cout<<std::bitset<8>(tile_lsb)<<" "<<std::bitset<8>(tile_msb)<<"\n";
//                        std::cout<<std::bitset<8>(pixel)<<"\n";
//                        std::cout<<ColorToInt(GetColorFromPaletteRam(palette,pixel))<<"\n";
                    }
                    patternDrawPixel(
                        i,
                        nTileX*8+(7-col),
                        nTileY*8+row,
                        GetColorFromPaletteRam(palette,pixel));

                }
            }
        }
        
    }

    return {
        .data = srpPatternTable[i].data(),
        .width = PATTERN_SIZE,
        .height = PATTERN_SIZE,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
}

void Pppu2c02::screenDrawPixel(int x, int y, Color c)
{
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
        return;
    const size_t index = (y * SCREEN_WIDTH + x) * 4;
    sprScreen[index + 0] = c.r;
    sprScreen[index + 1] = c.g;
    sprScreen[index + 2] = c.b;
    sprScreen[index + 3] = c.a;
}

void Pppu2c02::patternDrawPixel(int i,int x,int y,uint8_t c){
    patternDrawPixel(i,x,y,GetColor(c));
}

void Pppu2c02::patternDrawPixel(int i,int x,int y,Color c){
    if (x < 0 || x >= PATTERN_SIZE || y < 0 || y >= PATTERN_SIZE)
        return;
    const size_t index=(y*PATTERN_SIZE+x)*4;

    srpPatternTable[i][index + 0] = c.r;//*(rand() % 10<2?0:1);
    srpPatternTable[i][index + 1] = c.g;//*(rand() % 10<2?0:1);
    srpPatternTable[i][index + 2] = c.b;//*(rand() % 10<2?0:1);
    srpPatternTable[i][index + 3] = c.a;//*(rand() % 10<2?0:1);
}


Color Pppu2c02::GetColorFromPaletteRam(uint8_t palette, uint8_t pixel){
    const size_t index=ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F;


    return  palScreen[index];
}
