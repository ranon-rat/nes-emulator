#include "ppu.h++"

Image Olc2c02::GetScreen()
{
    return {
        .data = sprScreen.data(),
        .width = SCREEN_WIDTH,
        .height = SCREEN_HEIGHT,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
};

Image Olc2c02::GetNameTable(uint8_t i)
{
    return {
        .data = sprNameTable[i].data(),
        .width = SCREEN_WIDTH,
        .height = SCREEN_HEIGHT,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
}

Image Olc2c02::GetPatternTable(uint8_t i)
{
    return {
        .data = srpPatternTable[i].data(),
        .width = PATTERN_SIZE,
        .height = PATTERN_SIZE,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
}

void Olc2c02::screenDrawPixel(int x, int y, Color c)
{
    if(x<0||x>=SCREEN_WIDTH||y<0||y>=SCREEN_HEIGHT) return;
    const size_t index = (y * SCREEN_WIDTH + x) * 4;
    sprScreen[index + 0] = c.r;
    sprScreen[index + 1] = c.g;
    sprScreen[index + 2] = c.b;
    sprScreen[index + 3] = c.a;
}