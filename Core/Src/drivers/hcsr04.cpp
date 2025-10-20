#include "hcsr04.hpp"

#include "rtos.hpp"

uint16_t HCSR04::retrieve()
{
    osMutexAcquire(ptimerMutexHandle, osWaitForever);

    osMutexRelease(ptimerMutexHandle);
    return 0;
}
