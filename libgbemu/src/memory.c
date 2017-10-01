#include "memory.h"
#include "registers.h"
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>


unsigned char fcart[0x4000];
unsigned char rom_bank[0x4000];
unsigned char vram[0x2000];
unsigned char ram_bank[0x2000];
unsigned char iram[0x2000];
unsigned char echo[0x1E00];
unsigned char oam[100];
unsigned char io[0x80];
unsigned char hram[0x80];

/*
void setmbc()
{
    switch (readByte(0x147))
    {
        case 1:
        case 2:
        case 3:
            mbc_type = 1;
            break;
        case 5:
        case 6:
            mbc_type = 2;
            break;
        default:
            mbc_type = 0;
            break;
    }
}
 */


void writeByte(unsigned short address, unsigned char n)
{
    if (address < 0x8000)   // Read only
        return;
    else if (address >= 0x8000 && address <= 0x9FFF)
        vram[address - 0x8000] = n;
    else if (address >= 0xA000 && address <= 0xBFFF)
        ram_bank[address - 0xA000] = n;
    else if (address >= 0xC000 && address <= 0xDFFF)
        iram[address - 0xC000] = n;
    else if (address >= 0xE000 && address <= 0xFDFF)    // ECHO RAM
    {
        iram[address - 0x2000] = n;
        echo[address - 0xE000] = n;
    }
    else if (address >= 0xFE00 && address <= 0xFE9F)
        oam[address - 0xFE00] = n;
    else if (address >= 0xFEA0 && address <= 0xFEFF)    // Restricted area
        return;
    else if (address >= 0xFF00 && address <= 0xFF7F)
        io[address - 0xFF00] = n;
    else if (address >= 0xFF80 && address <= 0xFFFE)
        hram[address - 0xFF80] = n;
    else if (address == 0xFFFF)
        interrupts.ime = n;
    else
    {
        printf("Invalid memory write at address %X", address);
        exit(1);
    }

}

void writeShort(unsigned short address, unsigned short nn)
{
    writeByte(address, (unsigned char) (nn >> 8));
    writeByte(address + 1, (unsigned char) nn);
}

unsigned char readByte(unsigned short address)
{
    if (address >= 0 && address <= 0x3FFF)
        return fcart[address];
    else if (address >= 0x4000 && address <= 0x7FFF)
        return rom_bank[address - 0x4000];
    else if (address >= 0x8000 && address <= 0x9FFF)
        return vram[address - 0x8000];
    else if (address >= 0xA000 && address <= 0xBFFF)
        return ram_bank[address - 0x4000];
    else if (address >= 0xC000 && address <= 0xDFFF)
        return iram[address - 0xC000];
    else if (address >= 0xFE00 && address <= 0xFE9F)
        return oam[address - 0xFE00];
    else if (address >= 0xFF00 && address <= 0xFF7F)
        return io[address - 0xFF00];
    else if (address >= 0xFF80 && address <= 0xFFFE)
        return hram[address - 0xFF80];
    else if (address == 0xFFFF)
        return interrupts.ime;
    else
    {
        printf("Invalid memory read at address %X", address);
        exit(1);
    }
}

unsigned short readShort(unsigned short address)
{
    return (readByte(address) << 8) | (readByte(address + 1));
}

void pushStack(unsigned short nn)
{
    registers.sp -= 2;
    writeShort(registers.sp, nn);
}

unsigned short popStack(void)
{
    unsigned short ret = readShort(registers.sp);
    registers.sp += 2;
    return ret;
}