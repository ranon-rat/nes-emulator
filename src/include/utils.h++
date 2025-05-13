#pragma once
#ifndef UTILS_6502
#define UTILS6502
#include <cstdint>
#include <functional>

namespace Utils{
   
    uint16_t combine2bytes(uint16_t high,uint16_t low);
    
}



class DeferClass
{
public:
    DeferClass(std::function<void()> on_finish) : on_finish(on_finish)
    {
    }
    ~DeferClass() // since this is executed when we finish our scope, we are going to use this.
    {
        if (on_finish)
        {
            on_finish();
        }
    }

private:
    std::function<void()> on_finish;
};
// this simple macro is quite simple, all it does is to create a variable :)
#define defer(action) DeferClass __DEFER__VAR##__COUNTER__(action)

#define COMBINE2BYTES(high,low) (((uint16_t)(high))<<8)|((uint16_t)(low))


#endif
