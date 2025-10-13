#pragma once

extern "C"
{
#include "main.h"
}

#include "ptimer.hpp"

struct IRRemoteEntry
{
    uint16_t addr;
    uint16_t data;
    bool     state;
};

class IRRemote
{
   private:
    uint8_t read_byte();

    PTimer<uint32_t>& m_htim;

    GPIO& m_gpio;

   public:
    IRRemote() = delete;
    IRRemote(GPIO& gpio, PTimer<uint32_t>& htim) : m_htim {htim}, m_gpio {gpio}
    {
    }

    IRRemoteEntry receive();

    ~IRRemote() = default;
};
