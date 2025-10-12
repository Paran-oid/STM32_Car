#pragma once

extern "C"
{
#include "main.h"
}

class IRremote
{
   private:
    struct Entry
    {
        uint16_t addr;
        uint16_t data;
    };

   public:
    IRremote() = delete;
    IRremote(GPIO_TypeDef* port, uint16_t pin);

    uint16_t receive();

    ~IRremote() = default;
};
