#define FLAG_ZERO   1 << 7          // If last CP result == 0
#define FLAG_SUB    1 << 6          // If last math instruction was subtract
#define FLAG_HC     1 << 5          // Half carry
#define FLAG_C      1 << 4          // Carry

#define flagIsSet(flag) (registers.flags & (flag))
#define flagSet(flag) (registers.flags |= (flag))
#define flagUnset(flag) (registers.flags &= ~(flag))

struct instruction {
    char *name;
    void *function;
} extern const instructions[256];

extern unsigned long ticks;


void reset(void);
void cycle(void);

void nop(void);                     // 0x00 - No operation
void ld_bc_nn(unsigned short nn);   // 0x01 - Load 16-bit immediate into BC
void ld_bc_v_a(void);               // 0x02 - Save A to address pointed by BC
void inc_bc(void);                  // 0x03 - Increment 16-bit BC
void inc_b(void);                   // 0x04 - Increment B
void dec_b(void);                   // 0x05 - Decrement B
void ld_b_n(unsigned char n);       // 0x06 - Load 8-bit immediate into B
void rlc_a(void);                   // 0x07 - Rotate A left with carry
void ld_nn_sp(unsigned short nn);   // 0x08 - Save SP to given address
void add_hl_bc(void);               // 0x09 - Add 16-bit BC to HL
void ld_a_bc_v(void);               // 0x0A - Load A from address pointed to by BC
void dec_bc(void);                  // 0x0B - Decrement 16-bit BC
void inc_c(void);                   // 0x0C - Increment C
void dec_c(void);                   // 0x0D - Decrement C
void ld_c_n(unsigned char n);       // 0x0E - Load 8-bit immediate into C
void rrc_a(void);                   // 0x0F - Rotate A right with carry

void stop(void);                    // 0x10 - Stop
void ld_de_nn(unsigned short nn);   // 0x11 - Load 16-bit immediate into DE
void ld_de_v_a(void);               // 0x12 - Save A to address pointed by DE
void inc_de(void);                  // 0x13 - Increment 16-bit DE
void inc_d(void);                   // 0x14 - Increment D
void dec_d(void);                   // 0x15 - Decrement D
void ld_d_n(unsigned char n);       // 0x16 - Load 8-bit immediate into D
void rl_a(void);                    // 0x17 - Rotate A left without carry
void jr_n(short n);                 // 0x18 - Relative jump by signed immediate
void add_hl_de(void);               // 0x19 - Add 16-bit DE to HL
void ld_a_de_v(void);               // 0x1A - Load A from address pointed to by DE
void dec_de(void);                  // 0x1B - Decrement 16-bit DE
void inc_e(void);                   // 0x1C - Increment E
void dec_e(void);                   // 0x1D - Decrement E
void ld_e_n(unsigned char n);       // 0x1E - Load 8-bit immediate into E
void rr_a(void);                    // 0x1F - Rotate A right without carry

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

void and_b(void);                   // 0xA0 - Logical AND B against A
void and_c(void);                   // 0xA1 - Logical AND C against A
void and_d(void);                   // 0xA2 - Logical AND D against A
void and_e(void);                   // 0xA3 - Logical AND E against A
void and_h(void);                   // 0xA4 - Logical AND H against A
void and_l(void);                   // 0xA5 - Logical AND L against A
void and_hl_v(void);                // 0xA6 - Logical AND value pointed by HL against A
// Equivalent of NOP                // 0xA7 - Logical AND A against A
void xor_b(void);                   // 0xA8 - Logical XOR B against A
void xor_c(void);                   // 0xA9 - Logical XOR C against A
void xor_d(void);                   // 0xAA - Logical XOR D against A
void xor_e(void);                   // 0xAB - Logical XOR E against A
void xor_h(void);                   // 0xAC - Logical XOR H against A
void xor_l(void);                   // 0xAD - Logical XOR B against A
void xor_hl_v(void);                // 0xAE - Logical XOR value pointed by HL against A
void xor_a(void);                   // 0xAF - Logical XOR A against A

void or_b(void);                    // 0xB0 - Logical OR B against A
void or_c(void);                    // 0xB1 - Logical OR C against A
void or_d(void);                    // 0xB2 - Logical OR D against A
void or_e(void);                    // 0xB3 - Logical OR E against A
void or_h(void);                    // 0xB4 - Logical OR H against A
void or_l(void);                    // 0xB5 - Logical OR L against A
void or_hl_v(void);                 // 0xB6 - Logical OR value pointed by HL against A
void or_a(void);                    // 0xB7 - Logical OR A against A
void cp_b(void);                    // 0xB8 - Compare B against A
void cp_c(void);                    // 0xB9 - Compare C against A
void cp_d(void);                    // 0xBA - Compare D against A
void cp_e(void);                    // 0xBB - Compare E against A
void cp_h(void);                    // 0xBC - Compare H against A
void cp_l(void);                    // 0xBD - Compare L against A
void cp_hl_v(void);                 // 0xBE - Compare value pointed by HL against A
void cp_a(void);                    // 0xBF - Compare A against A

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
void and_n(unsigned char n);        // 0xE6 - Logical AND 8-bit immediate and A
void rst_20(void);                  // 0xE7 - Call routine at address 0x0020
void add_sp_d(char d);              // 0xE8 - Add signed 8-bit immediate to SP
void jp_hl_v(void);                 // 0xE9 - Jump to 16-bit value pointed by HL
void ld_nn_a(unsigned short nn);    // 0xEA - Save A at given 16-bit address
// Undefined                        // 0xEB
// Undefined                        // 0xEC
// Undefined                        // 0xED
void xor_n(unsigned char n);        // 0xEE - Logical XOR 8-bit immediate against A
void rst_28(void);                  // 0xEF - Call routine at address 0x0028

void ldh_a_n(unsigned char n);      // 0xF0 - Load A from address pointed to by (0xFF00 + 8-bit immediate)
void pop_af(void);                  // 0xF1 - Pop 16-bit value from stack into AF
// Undefined                        // 0xF2
void di(void);                      // 0xF3 - Disable interrupts
// Undefined                        // 0xF4
void push_af(void);                 // 0xF5 - Push 16-bit AF onto stack
void or_n(unsigned char n);         // 0xF6 - Logical OR 8-bit immediate against A
void rst_30(void);                  // 0xF7 - Call routine at 0x0030
void ldhl_sp_d(char d);             // 0xF8 - Add signed 8-bit immediate to SP and save result in HL
void ld_sp_hl(void);                // 0xF9 - Copy HL to SP
void ld_a_nn_v(unsigned short nn);  // 0xFA - Load A from given 16-bit address
void ei(void);                      // 0xFB - Disable interrupts
// Undefined                        // 0xFC
// Undefined                        // 0xFD
void cp_n(unsigned char n);         // 0xFE - Compare 8-bit immediate against A
void rst_38(void);                  // 0xFF - Call routine at address 0x0038
