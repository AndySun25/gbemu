struct instruction {
    char *name;
    void *function;
} extern const instructions[256];

extern unsigned long ticks;


void reset(void);
void cycle(void);

void nop(void);                     // 0x00 - No operation
void ld_bc_nn(unsigned short nn);   // 0x01 - Load 16-bit immediate into BC
void ld_bc_a(void);                 // 0x02 - Save A to address pointed by BC
void inc_bc(void);                  // 0x03 - Increment 16-bit BC
void inc_b(void);                   // 0x04 - Increment B
void dec_b(void);                   // 0x05 - Decrement B
void ld_b_n(unsigned char n);       // 0x06 - Load 8-bit immediate into B
void rlc_a(void);                   // 0x07 - Rotate A left with carry
void ld_sp_nn(unsigned short nn);   // 0x08 - Save SP to given address
void add_hl_bc(void);               // 0x09 - Add 16-bit BC to HL
void ld_a_bc(void);                 // 0x0A - Load A from address pointed to by BC
void dec_bc(void);                  // 0x0B - Decrement 16-bit BC
void inc_c(void);                   // 0x0C - Increment C
void dec_c(void);                   // 0x0D - Decrement C
void ld_c_n(unsigned char n);       // 0x0E - Load 8-bit immediate into C
void rrc_a(void);                   // 0x0F - Rotate A right with carry

void stop(void);                    // 0x10 - Stop
void ld_de_nn(unsigned short nn);   // 0x11 - Load 16-bit immediate into DE
void ld_de_a(void);                 // 0x12 - Save A to address pointed by DE
void inc_de(void);                  // 0x13 - Increment 16-bit DE
void inc_d(void);                   // 0x14 - Increment D
void dec_d(void);                   // 0x15 - Decrement D
void ld_d_n(unsigned char n);       // 0x16 - Load 8-bit immediate into D
void rl_a(void);                    // 0x17 - Rotate A left without carry
void jr_n(short n);                 // 0x18 - Relative jump by signed immediate
void add_hl_de(void);               // 0x19 - Add 16-bit DE to HL
void ld_a_de(void);                 // 0x1A - Load A from address pointed to by DE
void dec_de(void);                  // 0x1B - Decrement 16-bit DE
void inc_e(void);                   // 0x1C - Increment E
void dec_e(void);                   // 0x1D - Decrement E
void ld_e_n(unsigned char n);       // 0x1E - Load 8-bit immediate into E
void rr_a(void);                    // 0x1F - Rotate A right without carry

void jr_nz_n(short n);              // 0x20 - Relative jump by signed immediate if last result was not zero
void ld_hl_nn(unsigned short nn);   // 0x21 - Load 16 bit immediate into HL
void ldi_hl_a(void);                // 0x22 - Save A to address pointed by HL, then increment HL
void inc_hl(void);                  // 0x23 - Increment HL
void inc_h(void);                   // 0x24 - Increment H
void dec_h(void);                   // 0x25 - Decrement H
void ld_h_n(unsigned char n);       // 0x26 - Load 8-bit immediate into H
void daa(void);                     // 0x27 - Adjust A for BCD addition
void jr_z_n(short n);               // 0x28 - Relative jump by signed immediate if last result was zero
void add_hl_hl(void);               // 0x29 - Add 16-bit HL to HL
void ldi_a_hl(void);                // 0x2A - Load A from address pointed to by HL, then increment HL
void dec_hl(void);                  // 0x2B - Decrement HL
void inc_l(void);                   // 0x2C - Increment L
void dec_l(void);                   // 0x2D - Decrement L
void ld_l_n(unsigned char n);       // 0x2E - Load 8-bit immediate into L
void cpl_a(void);                   // 0x2F - Complement (NOT A)

void jr_nc_n(short n);              // 0x30 - Relative jump by signed immediate if last result caused no carry
void ld_sp_nn(unsigned short nn);   // 0x31 - Load 16 bit immediate into SP
void ldd_hl_a(void);                // 0x32 - Save A to address pointed by HL, then decrement HL
void inc_sp(void);                  // 0x33 - Increment SP
void inc_hl_v(void);                // 0x34 - Increment value pointed by HL
void dec_hl_v(void);                // 0x35 - Decrement value pointed by HL
void ld_hl_v_n(unsigned char n);    // 0x36 - Load 8-bit immediate into address pointed by HL
void scf(void);                     // 0x37 - Set carry flag
void jr_c_n(short n);               // 0x38 - Relative jump by signed immediate if last result caused carry
void add_hl_sp(void);               // 0x39 - Add 16-bit SP to HL
void ldd_a_hl(void);                // 0x3A - Load A from address pointed to by HL, then decrement HL
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
