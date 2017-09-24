#include "cpu.h"
#include "registers.h"
#include "memory.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


bool stopped = false;
bool halted = false;


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
    {"LD A (HL+)", ldi_a_hl_v, 0, 8},
    {"DEC HL", dec_hl, 0, 8},
    {"INC L", inc_l, 0, 4},
    {"DEC L", dec_l, 0, 4},
    {"LD L n", ld_l_n, 1, 8},
    {"CPL A", cpl_a, 0, 4},
    {"JR NC n", jr_nc_n, 1, 8},     // 8 cycles min, 12 if jumping (handled in func)
    {"LD SP nn", ld_sp_nn, 2, 12},
    {"LDD (HL) A", ldd_hl_v_a, 0, 8},
    {"INC SP", inc_sp, 0, 8},
    {"INC (HL)", inc_hl_v, 0, 12},
    {"DEC (HL)", dec_hl_v, 0, 12},
    {"LD (HL) n", ld_hl_v_n, 1, 12},
    {"SCF", scf, 0, 4},
    {"JR C n", jr_c_n, 1, 8},       // 8 min, 12 on jump
    {"ADD HL SP", add_hl_sp, 0, 8},
    {"LDD A (HL)", ldd_a_hl_v, 0, 8},
    {"DEC SP", dec_sp, 0, 8},
    {"INC A", inc_a, 0, 4},
    {"DEC A", dec_a, 0, 4},
    {"LD A n", ld_a_n, 1, 8},
    {"CCF", ccf, 0, 4},
    {"LD B B (NOP)", nop, 0, 4},
    {"LD B C", ld_b_c, 0, 4},
    {"LD B D", ld_b_d, 0, 4},
    {"LD B E", ld_b_e, 0, 4},
    {"LD B H", ld_b_h, 0, 4},
    {"LD B L", ld_b_l, 0, 4},
    {"LD B (HL)", ld_b_hl_v, 0, 4},
    {"LD B A", ld_b_a, 0, 4},
    {"LD C B", ld_c_b, 0, 4},
    {"LD C C (NOP)", nop, 0, 4},
    {"LD C D", ld_c_d, 0, 4},
    {"LD C E", ld_c_e, 0, 4},
    {"LD C H", ld_c_h, 0, 4},
    {"LD C L", ld_c_l, 0, 4},
    {"LD C (HL)", ld_c_hl_v, 0, 4},
    {"LD C A", ld_c_a, 0, 4},
    {"LD D B", ld_d_b, 0, 4},
    {"LD D C", ld_d_c, 0, 4},
    {"LD D D (NOP)", nop, 0, 4},
    {"LD D E", ld_d_e, 0, 4},
    {"LD D H", ld_d_h, 0, 4},
    {"LD D L", ld_d_l, 0, 4},
    {"LD D (HL)", ld_d_hl_v, 0, 4},
    {"LD D A", ld_d_a, 0, 4},
    {"LD E B", ld_e_b, 0, 4},
    {"LD E C", ld_e_c, 0, 4},
    {"LD E D", ld_e_d, 0, 4},
    {"LD E E (NOP)", nop, 0, 4},
    {"LD E H", ld_e_h, 0, 4},
    {"LD E L", ld_e_l, 0, 4},
    {"LD E (HL)", ld_e_hl_v, 0, 4},
    {"LD E A", ld_e_a, 0, 4},
    {"LD H B", ld_h_b, 0, 4},
    {"LD H C", ld_h_c, 0, 4},
    {"LD H D", ld_h_d, 0, 4},
    {"LD H E", ld_h_e, 0, 4},
    {"LD H H (NOP)", nop, 0, 4},
    {"LD H L", ld_h_l, 0, 4},
    {"LD H (HL)", ld_h_hl_v, 0, 4},
    {"LD H A", ld_h_a, 0, 4},
    {"LD L B", ld_l_b, 0, 4},
    {"LD L C", ld_l_c, 0, 4},
    {"LD L D", ld_l_d, 0, 4},
    {"LD L E", ld_l_e, 0, 4},
    {"LD L H", ld_l_h, 0, 4},
    {"LD L L (NOP)", nop, 0, 4},
    {"LD L (HL)", ld_l_hl_v, 0, 4},
    {"LD L A", ld_l_a, 0, 4},
    {"LD (HL) B", ld_hl_v_b, 0, 4},
    {"LD (HL) C", ld_hl_v_c, 0, 4},
    {"LD (HL) D", ld_hl_v_d, 0, 4},
    {"LD (HL) E", ld_hl_v_e, 0, 4},
    {"LD (HL) H", ld_hl_v_h, 0, 4},
    {"LD (HL) L", ld_hl_v_l, 0, 4},
    {"LD (HL) (HL) (NOP)", nop, 0, 4},
    {"HALT", halt, 0, 4},
    {"LD (HL) A", ld_hl_v_a, 0, 4},
    {"LD A B", ld_a_b, 0, 4},
    {"LD A C", ld_a_c, 0, 4},
    {"LD A D", ld_a_d, 0, 4},
    {"LD A E", ld_a_e, 0, 4},
    {"LD A H", ld_a_h, 0, 4},
    {"LD A L", ld_a_l, 0, 4},
    {"LD A (HL)", ld_a_hl_v, 0, 4},
    {"LD A A (NOP)", nop, 0, 4},
    {"ADD A B", add_a_b, 0, 4},
    {"ADD A C", add_a_c, 0, 4},
    {"ADD A D", add_a_d, 0, 4},
    {"ADD A E", add_a_e, 0, 4},
    {"ADD A H", add_a_h, 0, 4},
    {"ADD A L", add_a_l, 0, 4},
    {"ADD A (HL)", add_a_hl_v, 0, 4},
    {"ADD A A", add_a_a, 0, 4},
    // Template: {"", , , },
};

unsigned long cycles;


unsigned char add_n_n(unsigned char t, unsigned char n) {
    unsigned int res = t + n;
    flagSet(FLAG_Z, ~res);
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, ((t & 0x0F) + (n & 0x0F)) > 0x0F);
    flagSet(FLAG_C, res >> 8);
    return (unsigned char) res;
}

unsigned char adc_n(unsigned char n) {
    unsigned int res = registers.a + n + flagIsSet(FLAG_C);
    flagSet(FLAG_Z, ~res);
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, ((registers.a & 0x0F) + (n & 0x0F)) > 0x0F);
    flagSet(FLAG_C, res >> 8);
    return (unsigned char) res;
}

unsigned char sub_n(unsigned char n) {
    unsigned char res = registers.a + n;
    flagSet(FLAG_Z, ~res);
    flagSet(FLAG_N, 1);
    flagSet(FLAG_H, (registers.a & 0x0F) < (n & 0x0F));
    flagSet(FLAG_C, n > registers.a);
    return res;
}

unsigned char inc_n(unsigned char t) {
    unsigned char res = t + (unsigned char) 1;
    flagSet(FLAG_Z, ~res);
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, (t & 0x0F) + 1 > 0x0F);
    return res;
}

unsigned char dec_n(unsigned char t) {
    unsigned char res = t + (unsigned char) 1;
    flagSet(FLAG_Z, ~res);
    flagSet(FLAG_N, 1);
    flagSet(FLAG_H, (t & 0x0F) < 1);
    return res;
}

// For uniformity purposes
unsigned short inc_nn(unsigned short t) {
    return ++t;
}

// For uniformity purposes
unsigned short dec_nn(unsigned short t) {
    return --t;
}

unsigned char rlc_n(unsigned char t) {
    flagSet(FLAG_C, t & 0b10000000);
    t <<= 1;
    flagSet(FLAG_Z, t == 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_N, 0);
    return t;
}

unsigned char rl_n(unsigned char t) {
    unsigned char carry = (unsigned char) flagIsSet(FLAG_C);
    flagSet(FLAG_C, t & 0b10000000);
    t <<= 1;
    t |= carry;
    flagSet(FLAG_Z, t == 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_N, 0);
    return t;
}

unsigned char rrc_n(unsigned char t) {
    flagSet(FLAG_C, t & 0x01);
    t >>= 1;
    flagSet(FLAG_Z, t == 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_N, 0);
}

unsigned char rr_n(unsigned char t) {
    unsigned char carry = ((unsigned char) flagIsSet(FLAG_C)) << 7;
    flagSet(FLAG_C, t & 0x01);
    t >>= 1;
    t |= carry;
    flagSet(FLAG_Z, t == 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_N, 0);
    return t;
}

unsigned short add_nn_nn(unsigned short t, unsigned short nn) {
    unsigned long res = t + nn;
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, ((t & 0xFFF) + (nn & 0xFFF)) > 0xFFF);
    flagSet(FLAG_C, res & 0xFFFF0000);
    return (unsigned short) res;
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
void inc_bc(void) { registers.bc = inc_nn(registers.bc); }

// 0x04
void inc_b(void) { registers.b = inc_n(registers.b); }

// 0x05
void dec_b(void) { registers.b = dec_n(registers.b); }

// 0x06
void ld_b_n(unsigned char n) { registers.b = n; }

// 0x07
void rlc_a(void) {
    flagSet(FLAG_C, registers.a & 0b10000000);
    registers.a <<= 1;
    flagSet(FLAG_Z, registers.a == 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_N, 0);
}

// 0x08
void ld_nn_sp(unsigned short nn) { writeShort(nn, registers.sp); }

// 0x09
void add_hl_bc(void) { registers.hl = add_nn_nn(registers.hl, registers.bc); }

// 0x0A
void ld_a_bc_v(void) { registers.a = readByte(registers.bc); }

// 0x0B
void dec_bc(void) { registers.bc = dec_nn(registers.bc); }

// 0x0C
void inc_c(void) { registers.c = inc_n(registers.c); }

// 0x0D
void dec_c(void) { registers.c = dec_n(registers.c); }

// 0x0E
void ld_c_n(unsigned char n) { registers.c = n; }

// 0x0F
void rrc_a(void) {
    flagSet(FLAG_C, registers.a & 0x01);
    registers.a >>= 1;
    flagSet(FLAG_Z, registers.a == 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_N, 0);
}

// 0x10
void stop(void) { stopped = true; }

// 0x11
void ld_de_nn(unsigned short nn) { registers.de = nn; }

// 0x12
void ld_de_v_a(void) { writeByte(registers.de, registers.a); }

// 0x13
void inc_de(void) { registers.de = inc_nn(registers.de); }

// 0x14
void inc_d(void) { registers.d = inc_n(registers.d); }

// 0x15
void dec_d(void) { registers.d = dec_n(registers.d); }

// 0x16
void ld_d_n(unsigned char n) { registers.d = n; }

// 0x17
void rl_a(void) {
    unsigned char carry = (unsigned char) flagIsSet(FLAG_C);
    flagSet(FLAG_C, registers.a & 0b10000000);
    registers.a <<= 1;
    registers.a |= carry;
    flagSet(FLAG_Z, registers.a == 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_N, 0);
}

// 0x18
void jr_n(short n) { registers.pc += n; }

// 0x19
void add_hl_de(void) { registers.hl = add_nn_nn(registers.hl, registers.de); }

// 0x1A
void ld_a_de_v(void) { registers.a = readByte(registers.de); }

// 0x1B
void dec_de(void) { registers.de = dec_nn(registers.de); }

// 0x1C
void inc_e(void) { registers.e = inc_n(registers.e); }

// 0x1D
void dec_e(void) { registers.e = dec_n(registers.e); }

// 0x1E
void ld_e_n(unsigned char n) { registers.e = n; }

// 0x1F
void rr_a(void) {
    unsigned char carry = ((unsigned char) flagIsSet(FLAG_C)) << 7;
    flagSet(FLAG_C, registers.a & 0x01);
    registers.a >>= 1;
    registers.a |= carry;
    flagSet(FLAG_Z, registers.a == 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_N, 0);
}

// 0x20
void jr_nz_n(short n) {
    if (~flagIsSet(FLAG_Z)) {
        registers.pc += n;
        cycles += 4;
    }
}

// 0x21
void ld_hl_nn(unsigned short nn) { registers.hl = nn; }

// 0x22
void ldi_hl_v_a(void) { writeByte(registers.hl++, registers.a); }

// 0x23
void inc_hl(void) { registers.hl = inc_nn(registers.hl); }

// 0x24
void inc_h(void) { registers.h = inc_n(registers.h); }

// 0x25
void dec_h(void) { registers.h = dec_n(registers.h); }

// 0x26
void ld_h_n(unsigned char n) { registers.h = n; }

// 0x27
void daa(void) {
    unsigned int res = registers.a;
    if ((res & 0x0F) > 9 || flagIsSet(FLAG_H)) {
        res += 0x06;
    }

    if ((res >> 4) > 9 || flagIsSet(FLAG_C)) {
        res += 0x60;
    }

    flagSet(FLAG_Z, res == 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_C, res > 0xFF);
    registers.a = (unsigned char) res;
}

// 0x28
void jr_z_n (char n) {
    if (flagIsSet(FLAG_Z)) {
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
void dec_hl(void) { registers.hl = dec_nn(registers.hl); }

// 0x2C
void inc_l(void) { registers.l = inc_n(registers.l); }

// 0x2D
void dec_l(void) { registers.l = dec_n(registers.l); }

// 0x2E
void ld_l_n(unsigned char n) { registers.l = n; }

// 0x2F
void cpl_a(void) {
    registers.a = ~registers.a;
    flagSet(FLAG_N, 1);
    flagSet(FLAG_H, 1);
}

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
void inc_sp(void) { registers.sp = inc_nn(registers.sp); }

// 0x34
void inc_hl_v(void) { writeByte(registers.hl, inc_n(readByte(registers.hl))); }

// 0x35
void dec_hl_v(void) { writeByte(registers.hl, dec_n(readByte(registers.hl))); }

// 0x36
void ld_hl_v_n(unsigned char n) { writeByte(registers.hl, n); }

// 0x37
void scf(void) {
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_C, 1);
}

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
void dec_sp(void) { registers.sp = dec_nn(registers.sp); }

// 0x3C
void inc_a(void) { registers.a = inc_n(registers.a); }

// 0x3D
void dec_a(void) { registers.a = dec_n(registers.a); }

// 0x3E
void ld_a_n(unsigned char n) { registers.a = n; }

// 0x3F
void ccf(void) {
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_C, ~flagIsSet(FLAG_C));
}

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
void ld_l_a(void) { writeByte(registers.hl, registers.a); }

// 0x70
void ld_hl_v_b(void) { writeByte(registers.hl, registers.b); }

// 0x71
void ld_hl_v_c(void) { writeByte(registers.hl, registers.c); }

// 0x72
void ld_hl_v_d(void) { writeByte(registers.hl, registers.d); }

// 0x73
void ld_hl_v_e(void) { writeByte(registers.hl, registers.e); }

// 0x74
void ld_hl_v_h(void) { writeByte(registers.hl, registers.h); }

// 0x75
void ld_hl_v_l(void) { writeByte(registers.hl, registers.l); }

// 0x76
void halt(void) { halted = true; }

// 0x77
void ld_hl_v_a(void) { writeByte(registers.hl, registers.a); }

// 0x78
void ld_a_b(void) { registers.a = registers.b; }

// 0x79
void ld_a_c(void) { registers.a = registers.c; }

// 0x7A
void ld_a_d(void) { registers.a = registers.d; }

// 0x7B
void ld_a_e(void) { registers.a = registers.e; }

// 0x7C
void ld_a_h(void) { registers.a = registers.h; }

// 0x7D
void ld_a_l(void) { registers.a = registers.l; }

// 0x7E
void ld_a_hl_v(void) { registers.a = readByte(registers.hl); }

// 0x80
void add_a_b(void) { registers.a = add_n_n(registers.a, registers.b); }

// 0x81
void add_a_c(void) { registers.a = add_n_n(registers.a, registers.c); }

// 0x82
void add_a_d(void) { registers.a = add_n_n(registers.a, registers.d); }

// 0x83
void add_a_e(void) { registers.a = add_n_n(registers.a, registers.e); }

// 0x84
void add_a_h(void) { registers.a = add_n_n(registers.a, registers.h); }

// 0x85
void add_a_l(void) { registers.a = add_n_n(registers.a, registers.l); }

// 0x86
void add_a_hl_v(void) { registers.a = add_n_n(registers.a, readByte(registers.hl)); }

// 0x87
void add_a_a(void) { registers.a = add_n_n(registers.a, registers.a); }
