#include "cpu.h"
#include "registers.h"
#include "memory.h"
#include <stdbool.h>


bool stopped = false;


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

// 0x10
void stop(void) { stopped = true; }

// 0x11
void ld_de_nn(unsigned short nn) { registers.de = nn; }

// 0x12
void ld_de_v_a(void) { writeByte(registers.de, registers.a); }

// 0x13
void inc_de(void) { registers.de++; }

// 0x14
void inc_d(void) { registers.d++; }

// 0x15
void dec_d(void) { registers.d--; }

// 0x16
void ld_d_n(unsigned char n) { registers.d = n; }

// 0x17
void rl_a(void) { registers.a <<= 1; }

// 0x18
void jr_n(short n) { registers.pc += n; }

// 0x19
void add_hl_de(void) { registers.hl += registers.de; }

// 0x1A
void ld_a_de_v(void) { registers.a = readByte(registers.de); }

// 0x1B
void dec_de(void) { registers.de--; }

// 0x1C
void inc_e(void) { registers.de++; }

// 0x1D
void dec_e(void) { registers.e--; }

// 0x1E
void ld_e_n(unsigned char n) { registers.e = n; }

// 0x1F
void rr_a(void) { registers.a >>= 1; }

// 0x20
void jr_nz_n(short n) {
    if (~flagIsSet(FLAG_ZERO))
        registers.pc += n;
    else
        // TODO Handle this somehow
        ;
}

// 0x21
void ld_hl_nn(unsigned short nn) { registers.hl = nn; }

// 0x22
void ldi_hl_v_a(void) { writeByte(registers.hl, registers.a + (unsigned char) 1); }

// 0x23
void inc_hl(void) { registers.hl++; }

// 0x24
void inc_h(void) { registers.h++; }

// 0x25
void dec_h(void) { registers.h--; }

// 0x26
void ld_h_n(unsigned char n) { registers.h = n; }

// 0x27
void daa(void) { /* TODO Figure out how this shit works*/ }
