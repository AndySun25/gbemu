#include <stdio.h>

int main()
{
    unsigned char lo = 0xF0;
    unsigned char hi = 0x0F;
    unsigned short comb = (lo << 8) | hi;
    printf("%x", comb);
}