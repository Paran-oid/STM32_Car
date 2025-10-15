#pragma once

extern "C"
{
#include "main.h"
}

#include "ptimer.hpp"

struct IRRemoteEntry
{
    uint16_t addr;
    uint16_t addr_bar;

    uint16_t data;
    uint16_t data_bar;

    bool state;
};

enum IRRemoteCode
{
    IR_REMOTE_OFF           = 0x45,
    IR_REMOTE_MODE          = 0x46,
    IR_REMOTE_MUTE          = 0x47,
    IR_REMOTE_PAUSE_UNPAUSE = 0x44,

    IR_REMOTE_PREV     = 0x40,
    IR_REMOTE_NEXT     = 0x43,
    IR_REMOTE_EQ       = 0x07,
    IR_REMOTE_VOL_DOWN = 0x15,
    IR_REMOTE_VOL_UP   = 0x09,

    IR_REMOTE_0 = 0x16,
    IR_REMOTE_1 = 0x0C,
    IR_REMOTE_2 = 0x18,
    IR_REMOTE_3 = 0x5E,
    IR_REMOTE_4 = 0x08,
    IR_REMOTE_5 = 0x1C,
    IR_REMOTE_6 = 0x5A,
    IR_REMOTE_7 = 0x42,
    IR_REMOTE_8 = 0x52,
    IR_REMOTE_9 = 0x4A,

    IR_REMOTE_RPT  = 0x19,
    IR_REMOTE_U_SD = 0x0D,

    IR_REMOTE_REPEAT_CODE = 0xFF  // gets sent when a button is held
};

#define IR_REPEAT_CHECK(entry)                                               \
    ((entry.data) == (entry.data_bar) && (entry.data_bar) == (entry.addr) && \
     (entry.addr) == (entry.addr_bar) && (entry.addr_bar) == 0xFF)

class IRRemote
{
   private:
    uint8_t read_byte();

    GPIO&             m_gpio;
    PTimer<uint32_t>& m_htim;

   public:
    IRRemote() = delete;
    IRRemote(GPIO& gpio, PTimer<uint32_t>& htim) : m_gpio {gpio}, m_htim {htim}
    {
    }

    IRRemoteEntry receive();
    bool          refresh();

    ~IRRemote() = default;
};
