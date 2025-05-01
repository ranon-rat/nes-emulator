#include <utils.h++>

uint16_t Utils::combine2bytes(uint16_t high,uint16_t low){
    return (high<<8)|low;
}