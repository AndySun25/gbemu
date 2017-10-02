#pragma once

#define FLAG_Z  1 << 7          // Zero
#define FLAG_N  1 << 6          // Negative
#define FLAG_H  1 << 5          // Half carry
#define FLAG_C  1 << 4          // Carry

#define INT_VBLANK  1 << 0
#define INT_LCDSTAT 1 << 1
#define INT TIMER   1 << 2
#define INT_SERIAL  1 << 3
#define INT_JOYPAD  1 << 4

#define flagIsSet(flag) (unsigned char) (registers.flags & (flag))
#define flagSet(flag, value) (registers.flags = (value) ? (registers.flags | (flag)) : (registers.flags & ~(flag)))

struct instruction {
    char *name;
    void *func;
    unsigned short operand_length;
    unsigned char base_cycles;
} extern const instructions[256], extended[256];

struct interrupts {
    unsigned char ime;
    unsigned char interrupt_flags;
    unsigned char interrupt_enable;
} extern interrupts;

extern unsigned long cycles;


void reset(void);
void cycle(void);

// Opcodes
void undefined(void);

void nop(void);                     // 0x00 - No operation
void ld_bc_nn(unsigned short nn);   // 0x01 - Load 16-bit immediate into BC
void ld_bc_v_a(void);               // 0x02 - Save A to address pointed by BC
void inc_bc(void);                  // 0x03 - Increment 16-bit BC
void inc_b(void);                   // 0x04 - Increment B
void dec_b(void);                   // 0x05 - Decrement B
void ld_b_n(unsigned char n);       // 0x06 - Load 8-bit immediate into B
void rlca(void);                    // 0x07 - Rotate A left with carry
void ld_nn_sp(unsigned short nn);   // 0x08 - Save SP to given address
void add_hl_bc(void);               // 0x09 - Add 16-bit BC to HL
void ld_a_bc_v(void);               // 0x0A - Load A from address pointed to by BC
void dec_bc(void);                  // 0x0B - Decrement 16-bit BC
void inc_c(void);                   // 0x0C - Increment C
void dec_c(void);                   // 0x0D - Decrement C
void ld_c_n(unsigned char n);       // 0x0E - Load 8-bit immediate into C
void rrca(void);                    // 0x0F - Rotate A right with carry

void stop(void);                    // 0x10 - Stop
void ld_de_nn(unsigned short nn);   // 0x11 - Load 16-bit immediate into DE
void ld_de_v_a(void);               // 0x12 - Save A to address pointed by DE
void inc_de(void);                  // 0x13 - Increment 16-bit DE
void inc_d(void);                   // 0x14 - Increment D
void dec_d(void);                   // 0x15 - Decrement D
void ld_d_n(unsigned char n);       // 0x16 - Load 8-bit immediate into D
void rla(void);                     // 0x17 - Rotate A left through carry
void jr_n(short n);                 // 0x18 - Relative jump by signed immediate
void add_hl_de(void);               // 0x19 - Add 16-bit DE to HL
void ld_a_de_v(void);               // 0x1A - Load A from address pointed to by DE
void dec_de(void);                  // 0x1B - Decrement 16-bit DE
void inc_e(void);                   // 0x1C - Increment E
void dec_e(void);                   // 0x1D - Decrement E
void ld_e_n(unsigned char n);       // 0x1E - Load 8-bit immediate into E
void rra(void);                     // 0x1F - Rotate A right through carry

void jr_nz_n(short n);              // 0x20 - Relative jump by signed immediate if last result was not zero
void ld_hl_nn(unsigned short nn);   // 0x21 - Load 16 bit immediate into HL
void ldi_hl_v_a(void);              // 0x22 - Save A to address pointed by HL, then increment HL
void inc_hl(void);                  // 0x23 - Increment HL
void inc_h(void);                   // 0x24 - Increment H
void dec_h(void);                   // 0x25 - Decrement H
void ld_h_n(unsigned char n);       // 0x26 - Load 8-bit immediate into H
void daa(void);                     // 0x27 - Adjust A for BCD addition
void jr_z_n(char n);                // 0x28 - Relative jump by signed immediate if last result was zero
void add_hl_hl(void);               // 0x29 - Add 16-bit HL to HL
void ldi_a_hl_v(void);              // 0x2A - Load A from address pointed to by HL, then increment HL
void dec_hl(void);                  // 0x2B - Decrement HL
void inc_l(void);                   // 0x2C - Increment L
void dec_l(void);                   // 0x2D - Decrement L
void ld_l_n(unsigned char n);       // 0x2E - Load 8-bit immediate into L
void cpl_a(void);                   // 0x2F - Complement (NOT A)

void jr_nc_n(short n);              // 0x30 - Relative jump by signed immediate if last result caused no carry
void ld_sp_nn(unsigned short nn);   // 0x31 - Load 16 bit immediate into SP
void ldd_hl_v_a(void);              // 0x32 - Save A to address pointed by HL, then decrement HL
void inc_sp(void);                  // 0x33 - Increment SP
void inc_hl_v(void);                // 0x34 - Increment value pointed by HL
void dec_hl_v(void);                // 0x35 - Decrement value pointed by HL
void ld_hl_v_n(unsigned char n);    // 0x36 - Load 8-bit immediate into address pointed by HL
void scf(void);                     // 0x37 - Set carry flag
void jr_c_n(short n);               // 0x38 - Relative jump by signed immediate if last result caused carry
void add_hl_sp(void);               // 0x39 - Add 16-bit SP to HL
void ldd_a_hl_v(void);              // 0x3A - Load A from address pointed to by HL, then decrement HL
void dec_sp(void);                  // 0x3B - Decrement SP
void inc_a(void);                   // 0x3C - Increment A
void dec_a(void);                   // 0x3D - Decrement A
void ld_a_n(unsigned char n);       // 0x3E - Load 8-bit immediate into A
void ccf(void);                     // 0x3F - Clear carry flag

// Equivalent of NOP                // 0x40 - Copy B to B
void ld_b_c(void);                  // 0x41 - Copy C to B
void ld_b_d(void);                  // 0x42 - Copy D to B
void ld_b_e(void);                  // 0x43 - Copy E to B
void ld_b_h(void);                  // 0x44 - Copy H to B
void ld_b_l(void);                  // 0x45 - Copy L to B
void ld_b_hl_v(void);               // 0x46 - Copy value pointed by HL to B
void ld_b_a(void);                  // 0x47 - Copy A to B
void ld_c_b(void);                  // 0x48 - Copy B to C
// Equivalent of NOP                // 0x49 - Copy C to C
void ld_c_d(void);                  // 0x4A - Copy D to C
void ld_c_e(void);                  // 0x4B - Copy E to C
void ld_c_h(void);                  // 0x4C - Copy H to C
void ld_c_l(void);                  // 0x4D - Copy L to C
void ld_c_hl_v(void);               // 0x4E - Copy value pointed by HL to C
void ld_c_a(void);                  // 0x4F - Copy A to C

void ld_d_b(void);                  // 0x50 - Copy B to D
void ld_d_c(void);                  // 0x51 - Copy C to D
// Equivalent of NOP                // 0x52 - Copy D to D
void ld_d_e(void);                  // 0x53 - Copy E to D
void ld_d_h(void);                  // 0x54 - Copy H to D
void ld_d_l(void);                  // 0x55 - Copy L to D
void ld_d_hl_v(void);               // 0x56 - Copy value pointed by HL to D
void ld_d_a(void);                  // 0x57 - Copy A to D
void ld_e_b(void);                  // 0x58 - Copy B to E
void ld_e_c(void);                  // 0x59 - Copy C to E
void ld_e_d(void);                  // 0x5A - Copy D to E
// Equivalent of NOP                // 0x5B - Copy E to E
void ld_e_h(void);                  // 0x5C - Copy H to E
void ld_e_l(void);                  // 0x5D - Copy L to E
void ld_e_hl_v(void);               // 0x5E - Copy value pointed by HL to E
void ld_e_a(void);                  // 0x5D - Copy A to E

void ld_h_b(void);                  // 0x60 - Copy B to H
void ld_h_c(void);                  // 0x61 - Copy C to H
void ld_h_d(void);                  // 0x62 - Copy D to H
void ld_h_e(void);                  // 0x63 - Copy E to H
// Equivalent of NOP                // 0x64 - Copy H to H
void ld_h_l(void);                  // 0x65 - Copy L to H
void ld_h_hl_v(void);               // 0x66 - Copy value pointed by HL to H
void ld_h_a(void);                  // 0x67 - Copy A to H
void ld_l_b(void);                  // 0x68 - Copy B to L
void ld_l_c(void);                  // 0x69 - Copy C to L
void ld_l_d(void);                  // 0x6A - Copy D to L
void ld_l_e(void);                  // 0x6B - Copy E to L
void ld_l_h(void);                  // 0x6C - Copy H to L
// Equivalent of NOP                // 0x6D - Copy L to L
void ld_l_hl_v(void);               // 0x6E - Copy value pointed by HL to L
void ld_l_a(void);                  // 0x6F - Copy A to L

void ld_hl_v_b(void);               // 0x70 - Copy B to address pointed by HL
void ld_hl_v_c(void);               // 0x71 - Copy C to address pointed by HL
void ld_hl_v_d(void);               // 0x72 - Copy D to address pointed by HL
void ld_hl_v_e(void);               // 0x73 - Copy E to address pointed by HL
void ld_hl_v_h(void);               // 0x74 - Copy H to address pointed by HL
void ld_hl_v_l(void);               // 0x75 - Copy L to address pointed by HL
void halt(void);                    // 0x76 - Halt
void ld_hl_v_a(void);               // 0x77 - Copy A to address pointed by HL
void ld_a_b(void);                  // 0x78 - Copy B to A
void ld_a_c(void);                  // 0x79 - Copy C to A
void ld_a_d(void);                  // 0x7A - Copy D to A
void ld_a_e(void);                  // 0x7B - Copy E to A
void ld_a_h(void);                  // 0x7C - Copy H to A
void ld_a_l(void);                  // 0x7D - Copy L to A
void ld_a_hl_v(void);               // 0x7E - Copy value pointed by HL to A
// Equivalent of NOP                // 0x7F - Copy A to A

void add_a_b(void);                 // 0x80 - Add B to A
void add_a_c(void);                 // 0x81 - Add C to A
void add_a_d(void);                 // 0x82 - Add D to A
void add_a_e(void);                 // 0x83 - Add E to A
void add_a_h(void);                 // 0x84 - Add H to A
void add_a_l(void);                 // 0x85 - Add L to A
void add_a_hl_v(void);              // 0x86 - Add value pointed by HL to A
void add_a_a(void);                 // 0x87 - Add A to A
void adc_a_b(void);                 // 0x88 - Add B and carry flag to A
void adc_a_c(void);                 // 0x89 - Add C and carry flag to A
void adc_a_d(void);                 // 0x8A - Add D and carry flag to A
void adc_a_e(void);                 // 0x8B - Add E and carry flag to A
void adc_a_h(void);                 // 0x8C - Add H and carry flag to A
void adc_a_l(void);                 // 0x8D - Add L and carry flag to A
void adc_a_hl_v(void);              // 0x8E - Add value pointed by HL and carry flag to A
void adc_a_a(void);                 // 0x8F - Add A and carry flag to A

void sub_a_b(void);                 // 0x90 - Subtract B from A
void sub_a_c(void);                 // 0x91 - Subtract C from A
void sub_a_d(void);                 // 0x92 - Subtract D from A
void sub_a_e(void);                 // 0x93 - Subtract E from A
void sub_a_h(void);                 // 0x94 - Subtract H from A
void sub_a_l(void);                 // 0x95 - Subtract L from A
void sub_a_hl_v(void);              // 0x96 - Subtract value pointed by HL from A
void sub_a_a(void);                 // 0x97 - Subtract A from A
void sbc_a_b(void);                 // 0x98 - Subtract B and carry flag from A
void sbc_a_c(void);                 // 0x99 - Subtract C and carry flag from A
void sbc_a_d(void);                 // 0x9A - Subtract D and carry flag from A
void sbc_a_e(void);                 // 0x9B - Subtract E and carry flag from A
void sbc_a_h(void);                 // 0x9C - Subtract H and carry flag from A
void sbc_a_l(void);                 // 0x9D - Subtract L and carry flag from A
void sbc_a_hl_v(void);              // 0x9E - Subtract value pointed by HL and carry flag from A
void sbc_a_a(void);                 // 0x9D - Subtract A and carry flag from A

void and_a_b(void);                 // 0xA0 - Logical AND B against A
void and_a_c(void);                 // 0xA1 - Logical AND C against A
void and_a_d(void);                 // 0xA2 - Logical AND D against A
void and_a_e(void);                 // 0xA3 - Logical AND E against A
void and_a_h(void);                 // 0xA4 - Logical AND H against A
void and_a_l(void);                 // 0xA5 - Logical AND L against A
void and_a_hl_v(void);              // 0xA6 - Logical AND value pointed by HL against A
void and_a_a(void);                 // 0xA7 - Logical AND A against A
void xor_a_b(void);                 // 0xA8 - Logical XOR B against A
void xor_a_c(void);                 // 0xA9 - Logical XOR C against A
void xor_a_d(void);                 // 0xAA - Logical XOR D against A
void xor_a_e(void);                 // 0xAB - Logical XOR E against A
void xor_a_h(void);                 // 0xAC - Logical XOR H against A
void xor_a_l(void);                 // 0xAD - Logical XOR B against A
void xor_a_hl_v(void);              // 0xAE - Logical XOR value pointed by HL against A
void xor_a_a(void);                 // 0xAF - Logical XOR A against A

void or_a_b(void);                  // 0xB0 - Logical OR B against A
void or_a_c(void);                  // 0xB1 - Logical OR C against A
void or_a_d(void);                  // 0xB2 - Logical OR D against A
void or_a_e(void);                  // 0xB3 - Logical OR E against A
void or_a_h(void);                  // 0xB4 - Logical OR H against A
void or_a_l(void);                  // 0xB5 - Logical OR L against A
void or_a_hl_v(void);               // 0xB6 - Logical OR value pointed by HL against A
void or_a_a(void);                  // 0xB7 - Logical OR A against A
void cp_a_b(void);                  // 0xB8 - Compare B against A
void cp_a_c(void);                  // 0xB9 - Compare C against A
void cp_a_d(void);                  // 0xBA - Compare D against A
void cp_a_e(void);                  // 0xBB - Compare E against A
void cp_a_h(void);                  // 0xBC - Compare H against A
void cp_a_l(void);                  // 0xBD - Compare L against A
void cp_a_hl_v(void);               // 0xBE - Compare value pointed by HL against A
void cp_a_a(void);                  // 0xBF - Compare A against A

void ret_nz(void);                  // 0xC0 - Return if last result was not zero
void pop_bc(void);                  // 0xC1 - Pop 16-bit value from stack into BC
void jp_nz_nn(unsigned short nn);   // 0xC2 - Absolute jump to 16-bit location if last result was not zero
void jp_nn(unsigned short nn);      // 0xC3 - Absolute jump to 16-bit location
void call_nz_nn(unsigned short nn); // 0xC4 - Call routine at 16-bit location if last result was not zero
void push_bc(void);                 // 0xC5 - Push 16-bit BC onto stack
void add_a_n(unsigned char n);      // 0xC6 - Add 8-bit immediate to A
void rst_0(void);                   // 0xC7 - Call routine at address 0x0000
void ret_z(void);                   // 0xC8 - Return if last result was zero
void ret(void);                     // 0xC9 - Return to calling subroutine
void jp_z_nn(unsigned short nn);    // 0xCA - Absolute jump to 16-bit location if last result was zero
void ext_ops(void);                 // 0xCB - Extended operations (two-byte instruction code)
void call_z_nn(unsigned short nn);  // 0xCC - Call routine at 16-bit location if last result was zero
void call_nn(unsigned short nn);    // 0xCD - Call routine at 16-bit location
void adc_a_n(unsigned char n);      // 0xCE - Add 8-bit immediate and carry to A
void rst_8(void);                   // 0xCF - Call routine at address 0x0008

void ret_nc(void);                  // 0xD0 - Return if last result caused no carry
void pop_de(void);                  // 0xD1 - Pop 16-bit value from stack into DE
void jp_nc_nn(unsigned short nn);   // 0xD2 - Absolute jump to 16-bit location if last result caused no carry
// Undefined                        // 0xD3
void call_nc_nn(unsigned short nn); // 0xD4 - Call routine at 16-bit location if last result caused no carry
void push_de(void);                 // 0xD5 - Push 16-bit DE onto stack
void sub_a_n(unsigned char n);      // 0xD6 - Subtract 8-bit immediate from A
void rst_10(void);                  // 0xD7 - Call routine at address 0x0010
void ret_c(void);                   // 0xD8 - Return if last result caused carry
void reti(void);                    // 0xD9 - Enable interrupts and return to calling routine
void jp_c_nn(unsigned short nn);    // 0xDA - Absolute jump to 16-bit location if last result caused carry
// Undefined                        // 0xDB
void call_c_nn(unsigned short nn);  // 0xDC - Call routine at 16-bit location if last result caused carry
// Undefined                        // 0xDD
void sbc_a_n(unsigned char n);      // 0xDE - Subtract 8-bit immediate and carry from A
void rst_18(void);                  // 0xDF - Call routine at address 0x0018

void ldh_n_a(unsigned char n);      // 0xE0 - Save A at address pointed to by (0xFF00 + 8-bit immediate)
void pop_hl(void);                  // 0xE1 - Pop 16-bit value from stack to HL
void ldh_c_a(void);                 // 0xE2 - Save A at address pointed to by (0xFF00 + C)
// Undefined                        // 0xE3
// Undefined                        // 0xE4
void push_hl(void);                 // 0xE5 - Push 16-bit HL onto stack
void and_a_n(unsigned char n);      // 0xE6 - Logical AND 8-bit immediate and A
void rst_20(void);                  // 0xE7 - Call routine at address 0x0020
void add_sp_d(char d);              // 0xE8 - Add signed 8-bit immediate to SP
void jp_hl(void);                   // 0xE9 - Jump to 16-bit value pointed by HL
void ld_nn_a(unsigned short nn);    // 0xEA - Save A at given 16-bit address
// Undefined                        // 0xEB
// Undefined                        // 0xEC
// Undefined                        // 0xED
void xor_a_n(unsigned char n);      // 0xEE - Logical XOR 8-bit immediate against A
void rst_28(void);                  // 0xEF - Call routine at address 0x0028

void ldh_a_n(unsigned char n);      // 0xF0 - Load A from address pointed to by (0xFF00 + 8-bit immediate)
void pop_af(void);                  // 0xF1 - Pop 16-bit value from stack into AF
// Undefined                        // 0xF2
void di(void);                      // 0xF3 - Disable interrupts
// Undefined                        // 0xF4
void push_af(void);                 // 0xF5 - Push 16-bit AF onto stack
void or_a_n(unsigned char n);       // 0xF6 - Logical OR 8-bit immediate against A
void rst_30(void);                  // 0xF7 - Call routine at 0x0030
void ldhl_sp_d(char d);             // 0xF8 - Add signed 8-bit immediate to SP and save result in HL
void ld_sp_hl(void);                // 0xF9 - Copy HL to SP
void ld_a_nn_v(unsigned short nn);  // 0xFA - Load A from given 16-bit address
void ei(void);                      // 0xFB - Enable interrupts
// Undefined                        // 0xFC
// Undefined                        // 0xFD
void cp_a_n(unsigned char n);       // 0xFE - Compare 8-bit immediate against A
void rst_38(void);                  // 0xFF - Call routine at address 0x0038


// Extended instructions
void rlc_b(void);                   // 0x00 - Rotate B left with carry
void rlc_c(void);                   // 0x01 - Rotate C left with carry
void rlc_d(void);                   // 0x02 - Rotate D left with carry
void rlc_e(void);                   // 0x03 - Rotate E left with carry
void rlc_h(void);                   // 0x04 - Rotate H left with carry
void rlc_l(void);                   // 0x05 - Rotate L left with carry
void rlc_hl_v(void);                // 0x06 - Rotate value pointed by HL left with carry
// rlca                             // 0x07 - Rotate A left with carry
void rrc_b(void);                   // 0x08 - Rotate B right with carry
void rrc_c(void);                   // 0x09 - Rotate C right with carry
void rrc_d(void);                   // 0x0A - Rotate D right with carry
void rrc_e(void);                   // 0x0B - Rotate E right with carry
void rrc_h(void);                   // 0x0C - Rotate H right with carry
void rrc_l(void);                   // 0x0D - Rotate L right with carry
void rrc_hl_v(void);                // 0x0E - Rotate value pointed by HL right with carry
// rrca                             // 0x0F - Rotate A right with carry

void rl_b(void);                    // 0x10 - Rotate B left through carry
void rl_c(void);                    // 0x11 - Rotate C left through carry
void rl_d(void);                    // 0x12 - Rotate D left through carry
void rl_e(void);                    // 0x13 - Rotate E left through carry
void rl_h(void);                    // 0x14 - Rotate H left through carry
void rl_l(void);                    // 0x15 - Rotate L left through carry
void rl_hl_v(void);                 // 0x16 - Rotate value pointed by HL left through carry
// rla                              // 0x17 - Rotate A left through carry
void rr_b(void);                    // 0x18 - Rotate B right through carry
void rr_c(void);                    // 0x19 - Rotate C right through carry
void rr_d(void);                    // 0x1A - Rotate D right through carry
void rr_e(void);                    // 0x1B - Rotate E right through carry
void rr_h(void);                    // 0x1C - Rotate H right through carry
void rr_l(void);                    // 0x1D - Rotate L right through carry
void rr_hl_v(void);                 // 0x1E - Rotate value pointed by HL right through carry
// rra                              // 0x1F - Rotate A right through carry

void sla_b(void);                   // 0x20 - Shift B left into carry, LSBit of n set to 0
void sla_c(void);                   // 0x21 - Shift C left into carry, LSBit of n set to 0
void sla_d(void);                   // 0x22 - Shift D left into carry, LSBit of n set to 0
void sla_e(void);                   // 0x23 - Shift E left into carry, LSBit of n set to 0
void sla_h(void);                   // 0x24 - Shift H left into carry, LSBit of n set to 0
void sla_l(void);                   // 0x25 - Shift L left into carry, LSBit of n set to 0
void sla_hl_v(void);                // 0x26 - Shift value pointed by HL left into carry, LSBit of n set to 0
void sla_a(void);                   // 0x27 - Shift A left into carry, LSBit of n set to 0
void sra_b(void);                   // 0x28 - Shift B right into carry, MSBit doesn't change
void sra_c(void);                   // 0x29 - Shift C right into carry, MSBit doesn't change
void sra_d(void);                   // 0x2A - Shift D right into carry, MSBit doesn't change
void sra_e(void);                   // 0x2B - Shift E right into carry, MSBit doesn't change
void sra_h(void);                   // 0x2C - Shift H right into carry, MSBit doesn't change
void sra_l(void);                   // 0x2D - Shift L right into carry, MSBit doesn't change
void sra_hl_v(void);                // 0x2E - Shift value pointed by HL right into carry, MSBit doesn't change
void sra_a(void);                   // 0x2F - Shift A right into carry, MSBit doesn't change

void swap_b(void);                  // 0x30 - Swap upper and lower nibbles of B
void swap_c(void);                  // 0x31 - Swap upper and lower nibbles of C
void swap_d(void);                  // 0x32 - Swap upper and lower nibbles of D
void swap_e(void);                  // 0x33 - Swap upper and lower nibbles of E
void swap_h(void);                  // 0x34 - Swap upper and lower nibbles of H
void swap_l(void);                  // 0x35 - Swap upper and lower nibbles of L
void swap_hl_v(void);               // 0x36 - Swap upper and lower nibbles of value pointed by HL
void swap_a(void);                  // 0x37 - Swap upper and lower nibbles of A
void srl_b(void);                   // 0x38 - Shift B right into carry, MSBit set to 0
void srl_c(void);                   // 0x39 - Shift C right into carry, MSBit set to 0
void srl_d(void);                   // 0x3A - Shift D right into carry, MSBit set to 0
void srl_e(void);                   // 0x3B - Shift E right into carry, MSBit set to 0
void srl_h(void);                   // 0x3C - Shift H right into carry, MSBit set to 0
void srl_l(void);                   // 0x3D - Shift L right into carry, MSBit set to 0
void srl_hl_v(void);                // 0x3E - Shift value pointed by HL right into carry, MSBit set to 0
void srl_a(void);                   // 0x3F - Shift A right into carry, MSBit set to 0

void bit_0_b(void);                 // 0x40 - Test bit 0 of B
void bit_0_c(void);                 // 0x41 - Test bit 0 of C
void bit_0_d(void);                 // 0x42 - Test bit 0 of D
void bit_0_e(void);                 // 0x43 - Test bit 0 of E
void bit_0_h(void);                 // 0x44 - Test bit 0 of H
void bit_0_l(void);                 // 0x45 - Test bit 0 of L
void bit_0_hl_v(void);              // 0x46 - Test bit 0 of value pointed by HL
void bit_0_a(void);                 // 0x47 - Test bit 0 of A
void bit_1_b(void);                 // 0x48 - Test bit 1 of B
void bit_1_c(void);                 // 0x49 - Test bit 1 of C
void bit_1_d(void);                 // 0x4A - Test bit 1 of D
void bit_1_e(void);                 // 0x4B - Test bit 1 of E
void bit_1_h(void);                 // 0x4C - Test bit 1 of H
void bit_1_l(void);                 // 0x4D - Test bit 1 of L
void bit_1_hl_v(void);              // 0x4E - Test bit 1 of value pointed by HL
void bit_1_a(void);                 // 0x4F - Test bit 1 of A

void bit_2_b(void);                 // 0x50 - Test bit 2 of B
void bit_2_c(void);                 // 0x51 - Test bit 2 of C
void bit_2_d(void);                 // 0x52 - Test bit 2 of D
void bit_2_e(void);                 // 0x53 - Test bit 2 of E
void bit_2_h(void);                 // 0x54 - Test bit 2 of H
void bit_2_l(void);                 // 0x55 - Test bit 2 of L
void bit_2_hl_v(void);              // 0x56 - Test bit 2 of value pointed by HL
void bit_2_a(void);                 // 0x57 - Test bit 2 of A
void bit_3_b(void);                 // 0x58 - Test bit 3 of B
void bit_3_c(void);                 // 0x59 - Test bit 3 of C
void bit_3_d(void);                 // 0x5A - Test bit 3 of D
void bit_3_e(void);                 // 0x5B - Test bit 3 of E
void bit_3_h(void);                 // 0x5C - Test bit 3 of H
void bit_3_l(void);                 // 0x5D - Test bit 3 of L
void bit_3_hl_v(void);              // 0x5E - Test bit 3 of value pointed by HL
void bit_3_a(void);                 // 0x5F - Test bit 3 of A

void bit_4_b(void);                 // 0x60 - Test bit 4 of B
void bit_4_c(void);                 // 0x61 - Test bit 4 of C
void bit_4_d(void);                 // 0x62 - Test bit 4 of D
void bit_4_e(void);                 // 0x63 - Test bit 4 of E
void bit_4_h(void);                 // 0x64 - Test bit 4 of H
void bit_4_l(void);                 // 0x65 - Test bit 4 of L
void bit_4_hl_v(void);              // 0x66 - Test bit 4 of value pointed by HL
void bit_4_a(void);                 // 0x67 - Test bit 4 of A
void bit_5_b(void);                 // 0x68 - Test bit 5 of B
void bit_5_c(void);                 // 0x69 - Test bit 5 of C
void bit_5_d(void);                 // 0x6A - Test bit 5 of D
void bit_5_e(void);                 // 0x6B - Test bit 5 of E
void bit_5_h(void);                 // 0x6C - Test bit 5 of H
void bit_5_l(void);                 // 0x6D - Test bit 5 of L
void bit_5_hl_v(void);              // 0x6E - Test bit 5 of value pointed by HL
void bit_5_a(void);                 // 0x6F - Test bit 5 of A

void bit_6_b(void);                 // 0x70 - Test bit 6 of B
void bit_6_c(void);                 // 0x71 - Test bit 6 of C
void bit_6_d(void);                 // 0x72 - Test bit 6 of D
void bit_6_e(void);                 // 0x73 - Test bit 6 of E
void bit_6_h(void);                 // 0x74 - Test bit 6 of H
void bit_6_l(void);                 // 0x75 - Test bit 6 of L
void bit_6_hl_v(void);              // 0x76 - Test bit 6 of value pointed by HL
void bit_6_a(void);                 // 0x77 - Test bit 6 of A
void bit_7_b(void);                 // 0x78 - Test bit 7 of B
void bit_7_c(void);                 // 0x79 - Test bit 7 of C
void bit_7_d(void);                 // 0x7A - Test bit 7 of D
void bit_7_e(void);                 // 0x7B - Test bit 7 of E
void bit_7_h(void);                 // 0x7C - Test bit 7 of H
void bit_7_l(void);                 // 0x7D - Test bit 7 of L
void bit_7_hl_v(void);              // 0x7E - Test bit 7 of value pointed by HL
void bit_7_a(void);                 // 0x7F - Test bit 7 of A

void res_0_b(void);                 // 0x80 - Reset bit 0 of B
void res_0_c(void);                 // 0x81 - Reset bit 0 of C
void res_0_d(void);                 // 0x82 - Reset bit 0 of D
void res_0_e(void);                 // 0x83 - Reset bit 0 of E
void res_0_h(void);                 // 0x84 - Reset bit 0 of H
void res_0_l(void);                 // 0x85 - Reset bit 0 of L
void res_0_hl_v(void);              // 0x86 - Reset bit 0 of value pointed by HL
void res_0_a(void);                 // 0x87 - Reset bit 0 of A
void res_1_b(void);                 // 0x88 - Reset bit 1 of B
void res_1_c(void);                 // 0x89 - Reset bit 1 of C
void res_1_d(void);                 // 0x8A - Reset bit 1 of D
void res_1_e(void);                 // 0x8B - Reset bit 1 of E
void res_1_h(void);                 // 0x8C - Reset bit 1 of H
void res_1_l(void);                 // 0x8D - Reset bit 1 of L
void res_1_hl_v(void);              // 0x8E - Reset bit 1 of value pointed by HL
void res_1_a(void);                 // 0x8F - Reset bit 1 of A

void res_2_b(void);                 // 0x90 - Reset bit 2 of B
void res_2_c(void);                 // 0x91 - Reset bit 2 of C
void res_2_d(void);                 // 0x92 - Reset bit 2 of D
void res_2_e(void);                 // 0x93 - Reset bit 2 of E
void res_2_h(void);                 // 0x94 - Reset bit 2 of H
void res_2_l(void);                 // 0x95 - Reset bit 2 of L
void res_2_hl_v(void);              // 0x96 - Reset bit 2 of value pointed by HL
void res_2_a(void);                 // 0x97 - Reset bit 2 of A
void res_3_b(void);                 // 0x98 - Reset bit 3 of B
void res_3_c(void);                 // 0x99 - Reset bit 3 of C
void res_3_d(void);                 // 0x9A - Reset bit 3 of D
void res_3_e(void);                 // 0x9B - Reset bit 3 of E
void res_3_h(void);                 // 0x9C - Reset bit 3 of H
void res_3_l(void);                 // 0x9D - Reset bit 3 of L
void res_3_hl_v(void);              // 0x9E - Reset bit 3 of value pointed by HL
void res_3_a(void);                 // 0x9F - Reset bit 3 of A

void res_4_b(void);                 // 0xA0 - Reset bit 4 of B
void res_4_c(void);                 // 0xA1 - Reset bit 4 of C
void res_4_d(void);                 // 0xA2 - Reset bit 4 of D
void res_4_e(void);                 // 0xA3 - Reset bit 4 of E
void res_4_h(void);                 // 0xA4 - Reset bit 4 of H
void res_4_l(void);                 // 0xA5 - Reset bit 4 of L
void res_4_hl_v(void);              // 0xA6 - Reset bit 4 of value pointed by HL
void res_4_a(void);                 // 0xA7 - Reset bit 4 of A
void res_5_b(void);                 // 0xA8 - Reset bit 5 of B
void res_5_c(void);                 // 0xA9 - Reset bit 5 of C
void res_5_d(void);                 // 0xAA - Reset bit 5 of D
void res_5_e(void);                 // 0xAB - Reset bit 5 of E
void res_5_h(void);                 // 0xAC - Reset bit 5 of H
void res_5_l(void);                 // 0xAD - Reset bit 5 of L
void res_5_hl_v(void);              // 0xAE - Reset bit 5 of value pointed by HL
void res_5_a(void);                 // 0xAF - Reset bit 5 of A

void res_6_b(void);                 // 0xB0 - Reset bit 6 of B
void res_6_c(void);                 // 0xB1 - Reset bit 6 of C
void res_6_d(void);                 // 0xB2 - Reset bit 6 of D
void res_6_e(void);                 // 0xB3 - Reset bit 6 of E
void res_6_h(void);                 // 0xB4 - Reset bit 6 of H
void res_6_l(void);                 // 0xB5 - Reset bit 6 of L
void res_6_hl_v(void);              // 0xB6 - Reset bit 6 of value pointed by HL
void res_6_a(void);                 // 0xB7 - Reset bit 6 of A
void res_7_b(void);                 // 0xB8 - Reset bit 7 of B
void res_7_c(void);                 // 0xB9 - Reset bit 7 of C
void res_7_d(void);                 // 0xBA - Reset bit 7 of D
void res_7_e(void);                 // 0xBB - Reset bit 7 of E
void res_7_h(void);                 // 0xBC - Reset bit 7 of H
void res_7_l(void);                 // 0xBD - Reset bit 7 of L
void res_7_hl_v(void);              // 0xBE - Reset bit 7 of value pointed by HL
void res_7_a(void);                 // 0xBF - Reset bit 7 of A

void set_0_b(void);                 // 0xC0 - Set bit 0 of B
void set_0_c(void);                 // 0xC1 - Set bit 0 of C
void set_0_d(void);                 // 0xC2 - Set bit 0 of D
void set_0_e(void);                 // 0xC3 - Set bit 0 of E
void set_0_h(void);                 // 0xC4 - Set bit 0 of H
void set_0_l(void);                 // 0xC5 - Set bit 0 of L
void set_0_hl_v(void);              // 0xC6 - Set bit 0 of value pointed by HL
void set_0_a(void);                 // 0xC7 - Set bit 0 of A
void set_1_b(void);                 // 0xC8 - Set bit 1 of B
void set_1_c(void);                 // 0xC9 - Set bit 1 of C
void set_1_d(void);                 // 0xCA - Set bit 1 of D
void set_1_e(void);                 // 0xCB - Set bit 1 of E
void set_1_h(void);                 // 0xCC - Set bit 1 of H
void set_1_l(void);                 // 0xCD - Set bit 1 of L
void set_1_hl_v(void);              // 0xCE - Set bit 1 of value pointed by HL
void set_1_a(void);                 // 0xCF - Set bit 1 of A

void set_2_b(void);                 // 0xD0 - Set bit 2 of B
void set_2_c(void);                 // 0xD1 - Set bit 2 of C
void set_2_d(void);                 // 0xD2 - Set bit 2 of D
void set_2_e(void);                 // 0xD3 - Set bit 2 of E
void set_2_h(void);                 // 0xD4 - Set bit 2 of H
void set_2_l(void);                 // 0xD5 - Set bit 2 of L
void set_2_hl_v(void);              // 0xD6 - Set bit 2 of value pointed by HL
void set_2_a(void);                 // 0xD7 - Set bit 2 of A
void set_3_b(void);                 // 0xD8 - Set bit 3 of B
void set_3_c(void);                 // 0xD9 - Set bit 3 of C
void set_3_d(void);                 // 0xDA - Set bit 3 of D
void set_3_e(void);                 // 0xDB - Set bit 3 of E
void set_3_h(void);                 // 0xDC - Set bit 3 of H
void set_3_l(void);                 // 0xDD - Set bit 3 of L
void set_3_hl_v(void);              // 0xDE - Set bit 3 of value pointed by HL
void set_3_a(void);                 // 0xDF - Set bit 3 of A

void set_4_b(void);                 // 0xE0 - Set bit 4 of B
void set_4_c(void);                 // 0xE1 - Set bit 4 of C
void set_4_d(void);                 // 0xE2 - Set bit 4 of D
void set_4_e(void);                 // 0xE3 - Set bit 4 of E
void set_4_h(void);                 // 0xE4 - Set bit 4 of H
void set_4_l(void);                 // 0xE5 - Set bit 4 of L
void set_4_hl_v(void);              // 0xE6 - Set bit 4 of value pointed by HL
void set_4_a(void);                 // 0xE7 - Set bit 4 of A
void set_5_b(void);                 // 0xE8 - Set bit 5 of B
void set_5_c(void);                 // 0xE9 - Set bit 5 of C
void set_5_d(void);                 // 0xEA - Set bit 5 of D
void set_5_e(void);                 // 0xEB - Set bit 5 of E
void set_5_h(void);                 // 0xEC - Set bit 5 of H
void set_5_l(void);                 // 0xED - Set bit 5 of L
void set_5_hl_v(void);              // 0xEE - Set bit 5 of value pointed by HL
void set_5_a(void);                 // 0xEF - Set bit 5 of A

void set_6_b(void);                 // 0xF0 - Set bit 6 of B
void set_6_c(void);                 // 0xF1 - Set bit 6 of C
void set_6_d(void);                 // 0xF2 - Set bit 6 of D
void set_6_e(void);                 // 0xF3 - Set bit 6 of E
void set_6_h(void);                 // 0xF4 - Set bit 6 of H
void set_6_l(void);                 // 0xF5 - Set bit 6 of L
void set_6_hl_v(void);              // 0xF6 - Set bit 6 of value pointed by HL
void set_6_a(void);                 // 0xF7 - Set bit 6 of A
void set_7_b(void);                 // 0xF8 - Set bit 7 of B
void set_7_c(void);                 // 0xF9 - Set bit 7 of C
void set_7_d(void);                 // 0xFA - Set bit 7 of D
void set_7_e(void);                 // 0xFB - Set bit 7 of E
void set_7_h(void);                 // 0xFC - Set bit 7 of H
void set_7_l(void);                 // 0xFD - Set bit 7 of L
void set_7_hl_v(void);              // 0xFE - Set bit 7 of value pointed by HL
void set_7_a(void);                 // 0xFF - Set bit 7 of A
