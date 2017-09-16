#include "memory.h"

unsigned char memory[0xFFFF];

void writeByte(unsigned short address, unsigned char n)
{
    memory[address] = n;
}

void writeShort(unsigned short address, unsigned short nn)
{
    memory[address] = (unsigned char) (nn >> 8);
    memory[address + 1] = (unsigned char) (nn);
}

unsigned char readByte(unsigned short address)
{
    return memory[address];
}

unsigned short readShort(unsigned short address)
{
    return (memory[address] << 8) | (memory[address + 1]);
}