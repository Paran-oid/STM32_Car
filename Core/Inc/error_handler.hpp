#pragma once

extern "C"
{
#include "main.h"
}

/***************************************************************
 * Function declarations
 ***************************************************************/
extern "C"
{
    void error_handle(void (*alert)(void));
}

// Functions that get passed as parameter to error_handle function handler
void critical_error(void);
void warning_error(void);
