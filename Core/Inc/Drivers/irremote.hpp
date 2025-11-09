#pragma once

extern "C"
{
#include "main.h"
}

#include "ptimer.hpp"

/***********************************************************
 * Public defines
 ***********************************************************/
constexpr uint8_t SIGNAL_DELAY_US = 200;

/***********************************************************
 * IRRemoteEntry struct for IR-decoded readings
 ***********************************************************/
struct IRRemoteEntry
{
    uint8_t addr;
    uint8_t addr_bar;

    uint8_t data;
    uint8_t data_bar;

    bool is_valid;
};

/***********************************************************
 * Public gpio enums for IRREMOTE buttons
 ***********************************************************/
enum IRRemoteCode : uint8_t
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

/***************************************************************
 * DriveSystem class for IR readings
 ***************************************************************/
class IRRemote
{
   private:
    /***********************************************************
     * Private Members
     ***********************************************************/
    sca::gpio&             m_gpio;
    sca::ptimer<uint32_t>& m_htim;

    /***********************************************************
     * Private Methods
     ***********************************************************/
    uint8_t read_byte();

   public:
    /***********************************************************
     * Constructors / Destructor
     ***********************************************************/
    IRRemote() = delete;
    IRRemote(sca::gpio& gpio, sca::ptimer<uint32_t>& htim) : m_gpio {gpio}, m_htim {htim}
    {
    }
    ~IRRemote() = default;

    /***********************************************************
     * Public Methods
     ***********************************************************/
    IRRemoteEntry retrieve();
};

/***************************************************************
 * Function implementations (and declarations)
 ***************************************************************/
inline bool IR_repeat_check(const IRRemoteEntry& entry)
{
    return ((entry.data) == (entry.data_bar) && (entry.data_bar) == (entry.addr) &&
            (entry.addr) == (entry.addr_bar) && (entry.addr_bar) == 0xFF);
}
