#pragma once

extern "C"
{
#include "main.h"
}

/***************************************************************
 * _IWDG class for managing IWDG
 ****************************************************************/
class _IWDG
{
   private:
    IWDG_HandleTypeDef& m_hiwdg;

   public:
    _IWDG() = delete;
    _IWDG(IWDG_HandleTypeDef& hiwdg) : m_hiwdg {hiwdg}
    {
    }

    ~_IWDG() = default;

    bool refresh();
};
