#include "_iwdg.hpp"

bool _IWDG::refresh()
{
    return HAL_IWDG_Refresh(&m_hiwdg) == HAL_OK;
}
