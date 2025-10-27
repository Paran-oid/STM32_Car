#pragma once

extern "C"
{
#include "main.h"
}

extern "C"
{
    void error_handle(void (*alert)(void));
}

void critical_error(void);
void warning_error(void);
