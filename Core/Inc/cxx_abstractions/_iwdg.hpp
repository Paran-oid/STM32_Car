#pragma once

extern "C"
{
#include "main.h"
}

/***************************************************************
 * _IWDG class for managing independent watchdog timer
 ****************************************************************/
class _IWDG
{
   private:
    /***********************************************************
     * Private Members
     ***********************************************************/
    IWDG_HandleTypeDef& m_hiwdg;

   public:
    /***********************************************************
     * Constructors / Destructor
     ***********************************************************/
    _IWDG() = delete;
    _IWDG(IWDG_HandleTypeDef& hiwdg) : m_hiwdg {hiwdg}
    {
    }

    ~_IWDG() = default;

    /***********************************************************
     * Public Methods
     ***********************************************************/
    bool refresh();
};
