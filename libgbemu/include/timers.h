#pragma once

#define TIMA    0xFF05
#define TMA     0xFF06
#define TAC     0xFF07

#define TIMER_00    4096
#define TIMER_01    262144
#define TIMER_10    65536
#define TIMER_11    16384

#define TIMER_ENABLED   1 << 3


void updateTimers(int cycles);
void updateFrequency(void);
bool isClockEnabled(void);
