#include "cpu.h"
#include "registers.h"
#include "memory.h"

const struct instruction instructions[256] = {
    {"NOP", nop},
    {"LD BC nn", ld_bc_nn},
    {"LD BC A", ld_bc_v_a},
    {"INC BC", inc_bc},
    {"INC B", inc_b},
    {"DEC B", dec_b},
    {"LD B n", ld_b_n},
    {"RLC A", rlc_a},
};

void reset(void) {
    // Reset registers
    registers.pc = 0x0100;
    registers.sp = 0xFFFE;
}

// 0x00
void nop(void) {}

// 0x01
void ld_bc_nn(unsigned short nn) { registers.bc = nn; }

// 0x02
void ld_bc_v_a(void) { writeByte(registers.bc, registers.a); }

// 0x03
void inc_bc(void) { registers.bc++; }

// 0x04
void inc_b(void) { registers.b++; }

// 0x05
void dec_b(void) { registers.b--; }

// 0x06
void ld_b_n(unsigned char n) { registers.b = n; }

// 0x07
void rlc_a(void) {
    if (registers.a >> 7)
        flagSet(FLAG_C);
    else
        flagUnset(FLAG_C);

    registers.a <<= 1;
}

// 0x08
void ld_sp_nn(unsigned short nn) { writeShort(nn, registers.sp); }

// 0x09
void add_hl_bc(void) { registers.hl += registers.bc; }

// 0x0A
void ld_a_bc_v(void) { registers.a = readByte(registers.bc); }

// 0x0B
void dec_bc(void) { registers.bc--; }

// 0x0C
void inc_c(void) { registers.c++; }

// 0x0D
void dec_c(void) { registers.c--; }

// 0x0E
void ld_c_n(unsigned char n) { registers.c = n; }

// 0x0F
void rrc_a(void) {
    if (registers.a & 0x01)
        flagSet(FLAG_C);
    else
        flagUnset(FLAG_C);

    registers.a >>= 1;
}
