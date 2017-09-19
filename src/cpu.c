#include "cpu.h"
#include "registers.h"
#include "memory.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


bool stopped = false;


const struct instruction instructions[256] = {
    {"NOP", nop, 0, 4},
    {"LD BC nn", ld_bc_nn, 2, 12},
    {"LD (BC) A", ld_bc_v_a, 0, 8},
    {"INC BC", inc_bc, 0, 8},
    {"INC B", inc_b, 0, 4},
    {"DEC B", dec_b, 0, 4},
    {"LD B n", ld_b_n, 1, 8},
    {"RLC A", rlc_a, 0, 4},
    {"LD nn SP", ld_nn_sp, 2, 20},
    {"ADD HL BC", add_hl_bc, 0, 8},
    {"LD A (BC)", ld_a_bc_v, 0, 8},
    {"DEC BC", dec_bc, 0, 8},
    {"INC C", inc_c, 0, 4},
    {"DEC C", dec_c, 0, 4},
    {"LD C n", ld_c_n, 1, 8},
    {"RRC A", rrc_a, 0, 4},
    {"STOP", stop, 0, 4},
    {"LD DE nn", ld_de_nn, 2, 12},
    {"LD (DE) A", ld_de_v_a, 0, 8},
    {"INC DE", inc_de, 0, 8},
    {"INC D", inc_d, 0, 4},
    {"DEC D", dec_d, 0, 4},
    {"LD D n", ld_d_n, 1, 8},
    {"RL A", rl_a, 0, 4},
    {"JR n", jr_n, 1, 12},
    {"ADD HL DE", add_hl_de, 0, 8},
    {"LD A (DE)", ld_a_de_v, 0, 8},
    {"DEC DE", dec_de, 0, 8},
    {"INC E", inc_e, 0, 4},
    {"DEC E", dec_e, 0, 4},
    {"LD E n", ld_e_n, 1, 8},
    {"RR A", rr_a, 0, 4},
    {"JR NZ n", jr_nz_n, 1, 8},     // 8 cycles min, 12 if jumping (handled in func)
    {"LD HL nn", ld_hl_nn, 2, 12},
    {"LDI (HL) A", ldi_hl_v_a, 0, 8},
    {"INC HL", inc_hl, 0, 8},
    {"INC H", inc_h, 0, 4},
    {"DEC H", dec_h, 0, 4},
    {"LD H n", ld_h_n, 1, 8},
    {"DAA", daa, 0, 4},
    {"JR Z n", jr_z_n, 1, 8},       // 8 cycles min, 12 if jumping (handled in func)
    {"ADD HL HL", add_hl_hl, 0, 8},
    // Template: {"", , , },
};

unsigned long cycles;


unsigned short incShort(unsigned short nn) {
    return ++nn;
}


unsigned short decShort(unsigned short nn) {
    return --nn;
}


unsigned char incChar(unsigned char n) {
    flagSet(FLAG_ZERO, ++n == 0);
    flagSet(FLAG_SUB, 0);
    // TODO Handle half carry
    return n;
}


unsigned char decChar(unsigned char n) {
    flagSet(FLAG_ZERO, --n==0);
    flagSet(FLAG_SUB, 1);
    // TODO Handle half carry
    return n;
}


void reset(void) {
    // Reset registers
    registers.pc = 0x0100;
    registers.sp = 0xFFFE;
    cycles = 0;
}

void cycle(void) {
    unsigned char instruction;

    instruction = readByte(registers.pc++);
    void *func = instructions[instruction].func;


    switch (instructions[instruction].operand_length) {
        case 0:
            ((void (*)(void)) func)();
            break;
        case 1: {
            unsigned char operand = readByte(registers.pc++);
            ((void (*)(unsigned char n)) func)(operand);
            break;
        }
        case 2: {
            unsigned short operand = readShort(registers.pc);
            registers.pc += 2;
            ((void (*)(unsigned short nn)) func)(operand);
            break;
        }
        default:
            printf("Illegal operand length!");
            exit(1);
    }

    cycles += instructions[instruction].base_cycles;
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
    flagSet(FLAG_C, registers.a >> 7);
    registers.a <<= 1;
}

// 0x08
void ld_nn_sp(unsigned short nn) { writeShort(nn, registers.sp); }

// 0x09
// TODO (on all add functions) handle carry and half-carry flags properly
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
    flagSet(FLAG_C, registers.a & 0x01);
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
    if (~flagIsSet(FLAG_ZERO)) {
        registers.pc += n;
        cycles += 4;
    }
}

// 0x21
void ld_hl_nn(unsigned short nn) { registers.hl = nn; }

// 0x22
void ldi_hl_v_a(void) { writeByte(registers.hl++, registers.a); }

// 0x23
void inc_hl(void) { registers.hl++; }

// 0x24
void inc_h(void) { registers.h++; }

// 0x25
void dec_h(void) { registers.h--; }

// 0x26
void ld_h_n(unsigned char n) { registers.h = n; }

// 0x27
void daa(void) {
    if ((registers.a & 0x0F) > 9 || flagIsSet(FLAG_HC))
        registers.a += 0x06;

    if ((registers.a >> 4) > 9 || flagIsSet(FLAG_C))
        registers.a += 0x60;
}

// 0x28
void jr_z_n (char n) {
    if (flagIsSet(FLAG_ZERO)) {
        registers.pc += n;
        cycles += 4;
    }
}

// 0x29
void add_hl_hl(void) { registers.hl += registers.hl; }

// 0x2A
void ldi_a_hl_v(void) {
    registers.a = readByte(registers.hl++);
}

// 0x2B
void dec_hl(void) { registers.hl--; }

// 0x2C
void inc_l(void) { registers.l++; }

// 0x2D
void dec_l(void) { registers.l--; }

// 0x2E
void ld_l_n(unsigned char n) { registers.l = n; }

// 0x2F
void cpl_a(void) { registers.a = ~registers.a; }

// 0x30
void jr_nc_n(short n) {
    if (~flagIsSet(FLAG_C)) {
        registers.pc += n;
        cycles += 4;
    }
}

// 0x31
void ld_sp_nn(unsigned short nn) { registers.sp = nn; }

// 0x32
void ldd_hl_v_a(void) { writeByte(registers.hl--, registers.a); }

// 0x33
void inc_sp(void) { registers.sp++; }

// 0x34
void inc_hl_v(void) { writeByte(registers.hl, readByte(registers.hl) + (unsigned char) 1); }

// 0x35
void dec_hl_v(void) { writeByte(registers.hl, readByte(registers.hl) - (unsigned char) 1); }

// 0x36
void ld_hl_v_n(unsigned char n) { writeByte(registers.hl, n); }

// 0x37
void scf(void) { flagSet(FLAG_C, 1); }

// 0x38
void jr_c_n(short n) {
    if (flagIsSet(FLAG_C)) {
        registers.pc += n;
        cycles += 4;
    }
}

// 0x39
void add_hl_sp(void) { registers.sp += registers.hl; }

// 0x3A
void ldd_a_hl_v(void) {
    registers.a = readByte(registers.hl--);
}

// 0x3B
void dec_sp(void) { registers.sp--; }

// 0x3C
void inc_a(void) { registers.a++; }

// 0x3D
void dec_a(void) { registers.a--; }

// 0x3E
void ld_a_n(unsigned char n) { registers.a = n; }

// 0x3F
void ccf(void) { flagSet(FLAG_C, 0); }

// 0x41
void ld_b_c(void) { registers.b = registers.c; }

// 0x42
void ld_b_d(void) { registers.b = registers.d; }

// 0x43
void ld_b_e(void) { registers.b = registers.e; }

// 0x44
void ld_b_h(void) { registers.b = registers.h; }

// 0x45
void ld_b_l(void) { registers.b = registers.l; }

// 0x46
void ld_b_hl_v(void) { registers.b = readByte(registers.hl); }

// 0x47
void ld_b_a(void) { registers.b = registers.a; }

// 0x48
void ld_c_b(void) { registers.c = registers.b; }

// 0x4A
void ld_c_d(void) { registers.c = registers.d; }

// 0x4B
void ld_c_e(void) { registers.c = registers.e; }

// 0x4C
void ld_c_h(void) { registers.c = registers.h; }

// 0x4D
void ld_c_l(void) { registers.c = registers.l; }

// 0x46
void ld_c_hl_v(void) { registers.c = readByte(registers.hl); }

// 0x4F
void ld_c_a(void) { registers.c = registers.a; }

// 0x50
void ld_d_b(void) { registers.d = registers.b; }

// 0x51
void ld_d_c(void) { registers.d = registers.c; }

// 0x53
void ld_d_e(void) { registers.d = registers.e; }

// 0x54
void ld_d_h(void) { registers.d = registers.h; }

// 0x55
void ld_d_l(void) { registers.d = registers.l; }

// 0x56
void ld_d_hl_v(void) { registers.d = readByte(registers.hl); }

// 0x57
void ld_d_a(void) { registers.d = registers.a; }

// 0x58
void ld_e_b(void) { registers.e = registers.b; }

// 0x59
void ld_e_c(void) { registers.e = registers.c; }

// 0x5A
void ld_e_d(void) { registers.e = registers.d; }

// 0x5C
void ld_e_h(void) { registers.e = registers.h; }

// 0x5D
void ld_e_l(void) { registers.e = registers.l; }

// 0x5E
void ld_e_hl_v(void) { registers.e = readByte(registers.hl); }

// 0x5F
void ld_e_a(void) { registers.e = registers.a; }

// 0x60
void ld_h_b(void) { registers.h = registers.b; }

// 0x61
void ld_h_c(void) { registers.h = registers.c; }

// 0x62
void ld_h_d(void) { registers.h = registers.d; }

// 0x63
void ld_h_e(void) { registers.h = registers.e; }

// 0x65
void ld_h_l(void) { registers.h = registers.l; }

// 0x66
void ld_h_hl_v(void) { registers.h = readByte(registers.hl); }

// 0x67
void ld_h_a(void) { registers.h = registers.a; }

// 0x68
void ld_l_b(void) { registers.l = registers.b; }

// 0x69
void ld_l_c(void) { registers.l = registers.c; }

// 0x6A
void ld_l_d(void) { registers.l = registers.d; }

// 0x6B
void ld_l_e(void) { registers.l = registers.e; }

// 0x6C
void ld_l_h(void) { registers.l = registers.h; }

// 0x6E
void ld_l_hl_v(void) { registers.l = readByte(registers.hl); }

// 0x6F
void ld_l_a(void) { registers.l = registers.a; }
