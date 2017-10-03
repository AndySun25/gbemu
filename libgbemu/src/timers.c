#include <stdbool.h>
#include "cpu.h"
#include "memory.h"
#include "timers.h"

int frequency;


void updateTimers(int cycles)
{
    if (isClockEnabled())
    {

    }
}

void updateFrequency(void)
{
    switch (readByte(TAC) & 0b11)
    {
        case 0b00:
            frequency = CLOCKSPEED / TIMER_00;
            break;
        case 0b01:
            frequency = CLOCKSPEED / TIMER_01;
            break;
        case 0b10:
            frequency = CLOCKSPEED / TIMER_10;
            break;
        case 0b11:
            frequency = CLOCKSPEED / TIMER_11;
            break;
    }
}

bool isClockEnabled(void)
{
    return readByte(TAC) >> 2;
}
