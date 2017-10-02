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
    {"RLC A", rlca, 0, 4},
    {"LD nn SP", ld_nn_sp, 2, 20},
    {"ADD HL BC", add_hl_bc, 0, 8},
    {"LD A (BC)", ld_a_bc_v, 0, 8},
    {"DEC BC", dec_bc, 0, 8},
    {"INC C", inc_c, 0, 4},
    {"DEC C", dec_c, 0, 4},
    {"LD C n", ld_c_n, 1, 8},
    {"RRC A", rrca, 0, 4},
    {"STOP", stop, 0, 4},
    {"LD DE nn", ld_de_nn, 2, 12},
    {"LD (DE) A", ld_de_v_a, 0, 8},
    {"INC DE", inc_de, 0, 8},
    {"INC D", inc_d, 0, 4},
    {"DEC D", dec_d, 0, 4},
    {"LD D n", ld_d_n, 1, 8},
    {"RL A", rla, 0, 4},
    {"JR n", jr_n, 1, 12},
    {"ADD HL DE", add_hl_de, 0, 8},
    {"LD A (DE)", ld_a_de_v, 0, 8},
    {"DEC DE", dec_de, 0, 8},
    {"INC E", inc_e, 0, 4},
    {"DEC E", dec_e, 0, 4},
    {"LD E n", ld_e_n, 1, 8},
    {"RR A", rra, 0, 4},
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
    {"LD B (HL)", ld_b_hl_v, 0, 8},
    {"LD B A", ld_b_a, 0, 4},
    {"LD C B", ld_c_b, 0, 4},
    {"LD C C (NOP)", nop, 0, 4},
    {"LD C D", ld_c_d, 0, 4},
    {"LD C E", ld_c_e, 0, 4},
    {"LD C H", ld_c_h, 0, 4},
    {"LD C L", ld_c_l, 0, 4},
    {"LD C (HL)", ld_c_hl_v, 0, 8},
    {"LD C A", ld_c_a, 0, 4},
    {"LD D B", ld_d_b, 0, 4},
    {"LD D C", ld_d_c, 0, 4},
    {"LD D D (NOP)", nop, 0, 4},
    {"LD D E", ld_d_e, 0, 4},
    {"LD D H", ld_d_h, 0, 4},
    {"LD D L", ld_d_l, 0, 4},
    {"LD D (HL)", ld_d_hl_v, 0, 8},
    {"LD D A", ld_d_a, 0, 4},
    {"LD E B", ld_e_b, 0, 4},
    {"LD E C", ld_e_c, 0, 4},
    {"LD E D", ld_e_d, 0, 4},
    {"LD E E (NOP)", nop, 0, 4},
    {"LD E H", ld_e_h, 0, 4},
    {"LD E L", ld_e_l, 0, 4},
    {"LD E (HL)", ld_e_hl_v, 0, 8},
    {"LD E A", ld_e_a, 0, 4},
    {"LD H B", ld_h_b, 0, 4},
    {"LD H C", ld_h_c, 0, 4},
    {"LD H D", ld_h_d, 0, 4},
    {"LD H E", ld_h_e, 0, 4},
    {"LD H H (NOP)", nop, 0, 4},
    {"LD H L", ld_h_l, 0, 4},
    {"LD H (HL)", ld_h_hl_v, 0, 8},
    {"LD H A", ld_h_a, 0, 4},
    {"LD L B", ld_l_b, 0, 4},
    {"LD L C", ld_l_c, 0, 4},
    {"LD L D", ld_l_d, 0, 4},
    {"LD L E", ld_l_e, 0, 4},
    {"LD L H", ld_l_h, 0, 4},
    {"LD L L (NOP)", nop, 0, 4},
    {"LD L (HL)", ld_l_hl_v, 0, 8},
    {"LD L A", ld_l_a, 0, 4},
    {"LD (HL) B", ld_hl_v_b, 0, 8},
    {"LD (HL) C", ld_hl_v_c, 0, 8},
    {"LD (HL) D", ld_hl_v_d, 0, 8},
    {"LD (HL) E", ld_hl_v_e, 0, 8},
    {"LD (HL) H", ld_hl_v_h, 0, 8},
    {"LD (HL) L", ld_hl_v_l, 0, 8},
    {"HALT", halt, 0, 4},
    {"LD (HL) A", ld_hl_v_a, 0, 8},
    {"LD A B", ld_a_b, 0, 4},
    {"LD A C", ld_a_c, 0, 4},
    {"LD A D", ld_a_d, 0, 4},
    {"LD A E", ld_a_e, 0, 4},
    {"LD A H", ld_a_h, 0, 4},
    {"LD A L", ld_a_l, 0, 4},
    {"LD A (HL)", ld_a_hl_v, 0, 8},
    {"LD A A (NOP)", nop, 0, 4},
    {"ADD A B", add_a_b, 0, 4},
    {"ADD A C", add_a_c, 0, 4},
    {"ADD A D", add_a_d, 0, 4},
    {"ADD A E", add_a_e, 0, 4},
    {"ADD A H", add_a_h, 0, 4},
    {"ADD A L", add_a_l, 0, 4},
    {"ADD A (HL)", add_a_hl_v, 0, 8},
    {"ADD A A", add_a_a, 0, 4},
    {"ADC A B", adc_a_b, 0, 4},
    {"ADC A C", adc_a_c, 0, 4},
    {"ADC A D", adc_a_d, 0, 4},
    {"ADC A E", adc_a_e, 0, 4},
    {"ADC A H", adc_a_h, 0, 4},
    {"ADC A L", adc_a_l, 0, 4},
    {"ADC A (HL)", adc_a_hl_v, 0, 8},
    {"ADC A A", adc_a_a, 0, 4},
    {"SUB A B", sub_a_b, 0, 4},
    {"SUB A C", sub_a_c, 0, 4},
    {"SUB A D", sub_a_d, 0, 4},
    {"SUB A E", sub_a_e, 0, 4},
    {"SUB A H", sub_a_h, 0, 4},
    {"SUB A L", sub_a_l, 0, 4},
    {"SUB A (HL)", sub_a_hl_v, 0, 8},
    {"SUB A A", sub_a_a, 0, 4},
    {"SBC A B", sbc_a_b, 0, 4},
    {"SBC A C", sbc_a_c, 0, 4},
    {"SBC A D", sbc_a_d, 0, 4},
    {"SBC A E", sbc_a_e, 0, 4},
    {"SBC A H", sbc_a_h, 0, 4},
    {"SBC A L", sbc_a_l, 0, 4},
    {"SBC A (HL)", sbc_a_hl_v, 0, 8},
    {"SBC A A", sbc_a_a, 0, 4},
    {"AND A B", and_a_b, 0, 4},
    {"AND A C", and_a_c, 0, 4},
    {"AND A D", and_a_d, 0, 4},
    {"AND A E", and_a_e, 0, 4},
    {"AND A H", and_a_h, 0, 4},
    {"AND A L", and_a_l, 0, 4},
    {"AND A (HL)", and_a_hl_v, 0, 8},
    {"AND A A", and_a_a, 0, 4},
    {"XOR A B", xor_a_b, 0, 4},
    {"XOR A C", xor_a_c, 0, 4},
    {"XOR A D", xor_a_d, 0, 4},
    {"XOR A E", xor_a_e, 0, 4},
    {"XOR A H", xor_a_h, 0, 4},
    {"XOR A L", xor_a_l, 0, 4},
    {"XOR A (HL)", xor_a_hl_v, 0, 8},
    {"XOR A A", xor_a_a, 0, 4},
    {"OR A B", or_a_b, 0, 4},
    {"OR A C", or_a_c, 0, 4},
    {"OR A D", or_a_d, 0, 4},
    {"OR A E", or_a_e, 0, 4},
    {"OR A H", or_a_h, 0, 4},
    {"OR A L", or_a_l, 0, 4},
    {"OR A (HL)", or_a_hl_v, 0, 8},
    {"OR A A", or_a_a, 0, 4},
    {"CP A B", cp_a_b, 0, 4},
    {"CP A C", cp_a_c, 0, 4},
    {"CP A D", cp_a_d, 0, 4},
    {"CP A E", cp_a_e, 0, 4},
    {"CP A H", cp_a_h, 0, 4},
    {"CP A L", cp_a_l, 0, 4},
    {"CP A (HL)", cp_a_hl_v, 0, 8},
    {"CP A A", cp_a_a, 0, 4},
    {"RET NZ", ret_nz, 0, 8},           // 8 min, 20 on ret
    {"POP BC", pop_bc, 0, 12},
    {"JP NZ nn", jp_nz_nn, 2, 12},      // 12 min, 16 on jump
    {"JP nn", jp_nn, 2, 16},
    {"CALL NZ nn", call_nz_nn, 2, 12},  // 12 min, 24 on call
    {"PUSH BC", push_bc, 0, 16},
    {"ADD A n", add_a_n, 1, 8},
    {"RST 0", rst_0, 0, 16},
    {"RET Z", ret_z, 0, 8},
    {"RET", ret, 0, 16},
    {"JP Z nn", jp_z_nn, 2, 12},        // 12 min, 16 on jump
    {"EXT OPS", ext_ops, 0, 0},
    {"CALL Z nn", call_z_nn, 2, 12},    // 12 min, 24 on call
    {"CALL nn", call_nn, 2, 24},
    {"ADC A n", adc_a_n, 1, 8},
    {"RST 8", rst_8, 0, 16},
    {"RET NC", ret_nc, 0, 8},           // 8 min, 20 on ret
    {"POP DE", pop_de, 0, 12},
    {"JP NC nn", jp_nc_nn, 2, 12},      // 12 min, 16 on jump
    {"CALL NC nn", call_nc_nn, 2, 12},  // 12 min, 24 on call
    {"Undefined", undefined, 0, 0},
    {"PUSH DE", push_de, 0, 16},
    {"SUB A n", sub_a_n, 1, 8},
    {"RST 10", rst_10, 0, 16},
    {"RET C", ret_c, 0, 8},             // 8 min, 20 on ret
    {"RETI", reti, 0, 16},
    {"JP C nn", jp_c_nn, 2, 12},        // 12 min, 16 on jump
    {"Undefined", undefined, 0, 0},
    {"CALL C nn", call_c_nn, 2, 12},    // 12 min, 24 on call
    {"Undefined", undefined, 0, 0},
    {"SBC A n", sbc_a_n, 1, 8},
    {"RST 18", rst_18, 0, 16},
    {"LDH n A", ldh_n_a, 1, 12},
    {"POP HL", pop_hl, 0, 12},
    {"LDH C A", ldh_c_a, 0, 8},
    {"Undefined", undefined, 0, 0},
    {"Undefined", undefined, 0, 0},
    {"PUSH HL", push_hl, 0, 16},
    {"AND A n", and_a_n, 1, 8},
    {"RST 20", rst_20, 0, 16},
    {"ADD SP d", add_sp_d, 1, 16},
    {"JP HL", jp_hl, 0, 4},
    {"LD nn A", ld_nn_a, 2, 16},
    {"Undefined", undefined, 0, 0},
    {"Undefined", undefined, 0, 0},
    {"Undefined", undefined, 0, 0},
    {"XOR A n", xor_a_n, 1, 8},
    {"RST 28", rst_28, 0, 16},
    {"LDH A n", ldh_a_n, 1, 12},
    {"POP AF", pop_af, 0, 12},
    {"Undefined", undefined, 0, 0},
    {"DI", di, 0, 4},
    {"Undefined", undefined, 0, 0},
    {"PUSH AF", push_af, 0, 16},
    {"OR A n", or_a_n, 1, 8},
    {"RST 30", rst_30, 0, 16},
    {"LD HL,SP+d", ldhl_sp_d, 1, 12},
    {"LD SP HL", ld_sp_hl, 0, 8},
    {"LD A (nn)", ld_a_nn_v, 2, 16},
    {"EI", ei, 0, 4},
    {"Undefined", undefined, 0, 0},
    {"Undefined", undefined, 0, 0},
    {"CP A n", cp_a_n, 1, 8},
    {"RST 38", rst_38, 0, 16},
};


const struct instruction extended[256] = {
    {"RLC B", rlc_b, 0, 8},
    {"RLC C", rlc_c, 0, 8},
    {"RLC D", rlc_d, 0, 8},
    {"RLC E", rlc_e, 0, 8},
    {"RLC H", rlc_h, 0, 8},
    {"RLC L", rlc_l, 0, 8},
    {"RLC (HL)", rlc_hl_v, 0, 16},
    {"RLC A", rlca, 0, 8},
    {"RRC B", rrc_b, 0, 8},
    {"RRC C", rrc_c, 0, 8},
    {"RRC D", rrc_d, 0, 8},
    {"RRC E", rrc_e, 0, 8},
    {"RRC H", rrc_h, 0, 8},
    {"RRC L", rrc_l, 0, 8},
    {"RRC (HL)", rrc_hl_v, 0, 16},
    {"RRC A", rrca, 0, 8},
    {"RL B", rl_b, 0, 8},
    {"RL C", rl_c, 0, 8},
    {"RL D", rl_d, 0, 8},
    {"RL E", rl_e, 0, 8},
    {"RL H", rl_h, 0, 8},
    {"RL L", rl_l, 0, 8},
    {"RL (HL)", rl_hl_v, 0, 16},
    {"RL A", rla, 0, 8},
    {"RR B", rr_b, 0, 8},
    {"RR C", rr_c, 0, 8},
    {"RR D", rr_d, 0, 8},
    {"RR E", rr_e, 0, 8},
    {"RR H", rr_h, 0, 8},
    {"RR L", rr_l, 0, 8},
    {"RR (HL)", rr_hl_v, 0, 16},
    {"RR A", rra, 0, 8},
    {"SLA B", sla_b, 0, 8},
    {"SLA C", sla_c, 0, 8},
    {"SLA D", sla_d, 0, 8},
    {"SLA E", sla_e, 0, 8},
    {"SLA H", sla_h, 0, 8},
    {"SLA L", sla_l, 0, 8},
    {"SLA (HL)", sla_hl_v, 0, 16},
    {"SLA A", sla_a, 0, 8},
    {"SRA B", sra_b, 0, 8},
    {"SRA C", sra_c, 0, 8},
    {"SRA D", sra_d, 0, 8},
    {"SRA E", sra_e, 0, 8},
    {"SRA H", sra_h, 0, 8},
    {"SRA L", sra_l, 0, 8},
    {"SRA (HL)", sra_hl_v, 0, 16},
    {"SRA A", sra_a, 0, 8},
    {"SWAP B", swap_b, 0, 8},
    {"SWAP C", swap_c, 0, 8},
    {"SWAP D", swap_d, 0, 8},
    {"SWAP E", swap_e, 0, 8},
    {"SWAP H", swap_h, 0, 8},
    {"SWAP L", swap_l, 0, 8},
    {"SWAP (HL)", swap_hl_v, 0, 16},
    {"SWAP A", swap_a, 0, 8},
    {"SRL B", srl_b, 0, 8},
    {"SRL C", srl_c, 0, 8},
    {"SRL D", srl_d, 0, 8},
    {"SRL E", srl_e, 0, 8},
    {"SRL H", srl_h, 0, 8},
    {"SRL L", srl_l, 0, 8},
    {"SRL (HL)", srl_hl_v, 0, 16},
    {"SRL A", srl_a, 0, 8},
    {"BIT 0,B", bit_0_b, 0, 8},
    {"BIT 0,C", bit_0_c, 0, 8},
    {"BIT 0,D", bit_0_d, 0, 8},
    {"BIT 0,E", bit_0_e, 0, 8},
    {"BIT 0,H", bit_0_h, 0, 8},
    {"BIT 0,L", bit_0_l, 0, 8},
    {"BIT 0,(HL)", bit_0_hl_v, 0, 8},
    {"BIT 0,A", bit_0_a, 0, 8},
    {"BIT 1,B", bit_1_b, 0, 8},
    {"BIT 1,C", bit_1_c, 0, 8},
    {"BIT 1,D", bit_1_d, 0, 8},
    {"BIT 1,E", bit_1_e, 0, 8},
    {"BIT 1,H", bit_1_h, 0, 8},
    {"BIT 1,L", bit_1_l, 0, 8},
    {"BIT 1,(HL)", bit_1_hl_v, 0, 8},
    {"BIT 1,A", bit_1_a, 0, 8},
    {"BIT 2,B", bit_2_b, 0, 8},
    {"BIT 2,C", bit_2_c, 0, 8},
    {"BIT 2,D", bit_2_d, 0, 8},
    {"BIT 2,E", bit_2_e, 0, 8},
    {"BIT 2,H", bit_2_h, 0, 8},
    {"BIT 2,L", bit_2_l, 0, 8},
    {"BIT 2,(HL)", bit_2_hl_v, 0, 8},
    {"BIT 2,A", bit_2_a, 0, 8},
    {"BIT 3,B", bit_3_b, 0, 8},
    {"BIT 3,C", bit_3_c, 0, 8},
    {"BIT 3,D", bit_3_d, 0, 8},
    {"BIT 3,E", bit_3_e, 0, 8},
    {"BIT 3,H", bit_3_h, 0, 8},
    {"BIT 3,L", bit_3_l, 0, 8},
    {"BIT 3,(HL)", bit_3_hl_v, 0, 8},
    {"BIT 3,A", bit_3_a, 0, 8},
    {"BIT 4,B", bit_4_b, 0, 8},
    {"BIT 4,C", bit_4_c, 0, 8},
    {"BIT 4,D", bit_4_d, 0, 8},
    {"BIT 4,E", bit_4_e, 0, 8},
    {"BIT 4,H", bit_4_h, 0, 8},
    {"BIT 4,L", bit_4_l, 0, 8},
    {"BIT 4,(HL)", bit_4_hl_v, 0, 8},
    {"BIT 4,A", bit_4_a, 0, 8},
    {"BIT 5,B", bit_5_b, 0, 8},
    {"BIT 5,C", bit_5_c, 0, 8},
    {"BIT 5,D", bit_5_d, 0, 8},
    {"BIT 5,E", bit_5_e, 0, 8},
    {"BIT 5,H", bit_5_h, 0, 8},
    {"BIT 5,L", bit_5_l, 0, 8},
    {"BIT 5,(HL)", bit_5_hl_v, 0, 8},
    {"BIT 5,A", bit_5_a, 0, 8},
    {"BIT 6,B", bit_6_b, 0, 8},
    {"BIT 6,C", bit_6_c, 0, 8},
    {"BIT 6,D", bit_6_d, 0, 8},
    {"BIT 6,E", bit_6_e, 0, 8},
    {"BIT 6,H", bit_6_h, 0, 8},
    {"BIT 6,L", bit_6_l, 0, 8},
    {"BIT 6,(HL)", bit_6_hl_v, 0, 8},
    {"BIT 6,A", bit_6_a, 0, 8},
    {"BIT 7,B", bit_7_b, 0, 8},
    {"BIT 7,C", bit_7_c, 0, 8},
    {"BIT 7,D", bit_7_d, 0, 8},
    {"BIT 7,E", bit_7_e, 0, 8},
    {"BIT 7,H", bit_7_h, 0, 8},
    {"BIT 7,L", bit_7_l, 0, 8},
    {"BIT 7,(HL)", bit_7_hl_v, 0, 8},
    {"BIT 7,A", bit_7_a, 0, 8},
};


struct interrupts interrupts;

unsigned long cycles;


unsigned char add_n_n(unsigned char t, unsigned char n)
{
    unsigned int res = t + n;
    flagSet(FLAG_Z, ~res);
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, ((t & 0x0F) + (n & 0x0F)) > 0x0F);
    flagSet(FLAG_C, res >> 8);
    return (unsigned char) res;
}

unsigned char adc_n(unsigned char n)
{
    unsigned int res = registers.a + n + flagIsSet(FLAG_C);
    flagSet(FLAG_Z, ~res);
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, ((registers.a & 0x0F) + (n & 0x0F)) > 0x0F);
    flagSet(FLAG_C, res >> 8);
    return (unsigned char) res;
}

unsigned char sub_n(unsigned char n)
{
    unsigned char res = registers.a - n;
    flagSet(FLAG_Z, ~res);
    flagSet(FLAG_N, 1);
    flagSet(FLAG_H, (registers.a & 0x0F) < (n & 0x0F));
    flagSet(FLAG_C, n > registers.a);
    return res;
}

unsigned char sbc_n(unsigned char n)
{
    unsigned char res = registers.a - (n + flagIsSet(FLAG_C));
    flagSet(FLAG_Z, ~res);
    flagSet(FLAG_N, 1);
    flagSet(FLAG_H, (registers.a & 0x0F) < (n & 0x0F));
    flagSet(FLAG_C, n > registers.a);
    return res;
}

unsigned char inc_n(unsigned char t)
{
    unsigned char res = t + (unsigned char) 1;
    flagSet(FLAG_Z, ~res);
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, (t & 0x0F) + 1 > 0x0F);
    return res;
}

unsigned char dec_n(unsigned char t)
{
    unsigned char res = t - (unsigned char) 1;
    flagSet(FLAG_Z, ~res);
    flagSet(FLAG_N, 1);
    flagSet(FLAG_H, (t & 0x0F) < 1);
    return res;
}

// For uniformity purposes
unsigned short inc_nn(unsigned short t)
{
    return ++t;
}

// For uniformity purposes
unsigned short dec_nn(unsigned short t)
{
    return --t;
}

unsigned char and_n(unsigned char n)
{
    unsigned char res = registers.a & n;
    flagSet(FLAG_Z, res == 0);
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, 1);
    flagSet(FLAG_C, 0);
    return res;
}

unsigned char xor_n(unsigned char n)
{
    unsigned char res = registers.a ^ n;
    flagSet(FLAG_Z, res == 0);
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_C, 0);
    return res;
}

unsigned char or_n(unsigned char n)
{
    unsigned char res = registers.a | n;
    flagSet(FLAG_Z, res == 0);
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_C, 0);
    return res;
}

unsigned char rlc_n(unsigned char t)
{
    flagSet(FLAG_C, t & 0b10000000);
    t <<= 1;
    flagSet(FLAG_Z, t == 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_N, 0);
    return t;
}

unsigned char rl_n(unsigned char t)
{
    unsigned char carry = flagIsSet(FLAG_C);
    flagSet(FLAG_C, t & 0b10000000);
    t <<= 1;
    t |= carry;
    flagSet(FLAG_Z, t == 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_N, 0);
    return t;
}

unsigned char rrc_n(unsigned char t)
{
    flagSet(FLAG_C, t & 0x01);
    t >>= 1;
    flagSet(FLAG_Z, t == 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_N, 0);
}

unsigned char rr_n(unsigned char t)
{
    unsigned char carry = flagIsSet(FLAG_C) << 7;
    flagSet(FLAG_C, t & 0x01);
    t >>= 1;
    t |= carry;
    flagSet(FLAG_Z, t == 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_N, 0);
    return t;
}

unsigned short add_nn_nn(unsigned short t, unsigned short nn)
{
    unsigned long res = t + nn;
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, ((t & 0xFFF) + (nn & 0xFFF)) > 0xFFF);
    flagSet(FLAG_C, res & 0xFFFF0000);
    return (unsigned short) res;
}

unsigned char sla_n(unsigned char t)
{
    unsigned char res = t << 1;
    flagSet(FLAG_Z, res == 0);
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_C, t & 0b10000000);
}

unsigned char sra_n(unsigned char t)
{
    unsigned char res = (t >> 1) | (unsigned char) (t & 0b10000000);
    flagSet(FLAG_Z, res == 0);
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_C, t & 0x1);
}

unsigned char swap_n(unsigned char t)
{
    unsigned char res = (t >> 4) | (unsigned char) (t << 4 & 0xF0);
    flagSet(FLAG_Z, res == 0);
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_C, 0);
    return res;
}

unsigned char srl_n(unsigned char t)
{
    unsigned char res = (t >> 1);
    flagSet(FLAG_Z, res == 0);
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_C, t & 0x1);
}

void bit_n(unsigned char t, short pos)
{
    flagSet(FLAG_Z, t & (1 << pos));
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, 1);
}

void reset(void)
{
    // Reset registers
    registers.pc = 0x0100;
    registers.sp = 0xFFFE;
    registers.af = registers.bc = registers.de = registers.hl = 0;
    cycles = 0;
}

void cycle(void)
{
    unsigned char instruction;

    instruction = readByte(registers.pc++);
    void *func = instructions[instruction].func;


    switch (instructions[instruction].operand_length)
    {
        case 0:
            ((void (*)(void)) func)();
            break;
        case 1:
        {
            unsigned char operand = readByte(registers.pc++);
            ((void (*)(unsigned char n)) func)(operand);
            break;
        }
        case 2:
        {
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

void undefined(void)
{
    printf("Illegal opcode detected at %x!", registers.pc - 1);
    exit(0);
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
void rlca(void) { registers.a = rlc_n(registers.a); }

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
void rrca(void) { registers.a = rrc_n(registers.a); }

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
void rla(void) { registers.a = rl_n(registers.a); }

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
void rra(void) { registers.a = rr_n(registers.a); }

// 0x20
void jr_nz_n(short n)
{
    if (~flagIsSet(FLAG_Z))
    {
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
void daa(void)
{
    unsigned int res = registers.a;
    if ((res & 0x0F) > 9 || flagIsSet(FLAG_H))
    {
        res += 0x06;
    }

    if ((res >> 4) > 9 || flagIsSet(FLAG_C))
    {
        res += 0x60;
    }

    flagSet(FLAG_Z, res == 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_C, res > 0xFF);
    registers.a = (unsigned char) res;
}

// 0x28
void jr_z_n (char n)
{
    if (flagIsSet(FLAG_Z))
    {
        registers.pc += n;
        cycles += 4;
    }
}

// 0x29
void add_hl_hl(void) { registers.hl += registers.hl; }

// 0x2A
void ldi_a_hl_v(void)
{
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
void cpl_a(void)
{
    registers.a = ~registers.a;
    flagSet(FLAG_N, 1);
    flagSet(FLAG_H, 1);
}

// 0x30
void jr_nc_n(short n)
{
    if (~flagIsSet(FLAG_C))
    {
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
void scf(void)
{
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, 0);
    flagSet(FLAG_C, 1);
}

// 0x38
void jr_c_n(short n)
{
    if (flagIsSet(FLAG_C))
    {
        registers.pc += n;
        cycles += 4;
    }
}

// 0x39
void add_hl_sp(void) { registers.sp += registers.hl; }

// 0x3A
void ldd_a_hl_v(void)
{
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
void ccf(void)
{
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

// 0x88
void adc_a_b(void) { registers.a = adc_n(registers.b); }

// 0x89
void adc_a_c(void) { registers.a = adc_n(registers.c); }

// 0x8A
void adc_a_d(void) { registers.a = adc_n(registers.d); }

// 0x8B
void adc_a_e(void) { registers.a = adc_n(registers.e); }

// 0x8C
void adc_a_h(void) { registers.a = adc_n(registers.h); }

// 0x8D
void adc_a_l(void) { registers.a = adc_n(registers.l); }

// 0x8E
void adc_a_hl_v(void) { registers.a = adc_n(readByte(registers.hl)); }

// 0x8F
void adc_a_a(void) { registers.a = adc_n(registers.a); }

// 0x90
void sub_a_b(void) { registers.a = sub_n(registers.b); }

// 0x91
void sub_a_c(void) { registers.a = sub_n(registers.c); }

// 0x92
void sub_a_d(void) { registers.a = sub_n(registers.d); }

// 0x93
void sub_a_e(void) { registers.a = sub_n(registers.e); }

// 0x94
void sub_a_h(void) { registers.a = sub_n(registers.h); }

// 0x95
void sub_a_l(void) { registers.a = sub_n(registers.l); }

// 0x96
void sub_a_hl_v(void) { registers.a = sub_n(readByte(registers.hl)); }

// 0x97
void sub_a_a(void) { registers.a = sub_n(registers.a); }

// 0x98
void sbc_a_b(void) { registers.a = sbc_n(registers.b); }

// 0x99
void sbc_a_c(void) { registers.a = sbc_n(registers.c); }

// 0x9A
void sbc_a_d(void) { registers.a = sbc_n(registers.d); }

// 0x9B
void sbc_a_e(void) { registers.a = sbc_n(registers.e); }

// 0x9C
void sbc_a_h(void) { registers.a = sbc_n(registers.h); }

// 0x9D
void sbc_a_l(void) { registers.a = sbc_n(registers.l); }

// 0x9E
void sbc_a_hl_v(void) { registers.a = sbc_n(readByte(registers.hl)); }

// 0x9F
void sbc_a_a(void) { registers.a = sbc_n(registers.a); }

// 0xA0
void and_a_b(void) { registers.a = and_n(registers.b); }

// 0xA1
void and_a_c(void) { registers.a = and_n(registers.c); }

// 0xA2
void and_a_d(void) { registers.a = and_n(registers.d); }

// 0xA3
void and_a_e(void) { registers.a = and_n(registers.e); }

// 0xA4
void and_a_h(void) { registers.a = and_n(registers.h); }

// 0xA5
void and_a_l(void) { registers.a = and_n(registers.l); }

// 0xA6
void and_a_hl_v(void) { registers.a = and_n(readByte(registers.hl)); }

// 0xA7
void and_a_a(void) { registers.a = and_n(registers.a); }

// 0xA8
void xor_a_b(void) { registers.a = xor_n(registers.b); }

// 0xA9
void xor_a_c(void) { registers.a = xor_n(registers.c); }

// 0xAA
void xor_a_d(void) { registers.a = xor_n(registers.d); }

// 0xAB
void xor_a_e(void) { registers.a = xor_n(registers.e); }

// 0xAC
void xor_a_h(void) { registers.a = xor_n(registers.h); }

// 0xAD
void xor_a_l(void) { registers.a = xor_n(registers.l); }

// 0xAE
void xor_a_hl_v(void) { registers.a = xor_n(readByte(registers.hl)); }

// 0xAF
void xor_a_a(void) { registers.a = xor_n(registers.a); }

// 0xB0
void or_a_b(void) { registers.a = or_n(registers.b); }

// 0xB1
void or_a_c(void) { registers.a = or_n(registers.c); }

// 0xB2
void or_a_d(void) { registers.a = or_n(registers.d); }

// 0xB3
void or_a_e(void) { registers.a = or_n(registers.e); }

// 0xB4
void or_a_h(void) { registers.a = or_n(registers.h); }

// 0xB5
void or_a_l(void) { registers.a = or_n(registers.l); }

// 0xB6
void or_a_hl_v(void) { registers.a = or_n(readByte(registers.hl)); }

// 0xB7
void or_a_a(void) { registers.a = or_n(registers.a); }

// 0xB8
void cp_a_b(void) { sub_n(registers.b); }

// 0xB9
void cp_a_c(void) { sub_n(registers.c); }

// 0xBA
void cp_a_d(void) { sub_n(registers.d); }

// 0xBB
void cp_a_e(void) { sub_n(registers.e); }

// 0xBC
void cp_a_h(void) { sub_n(registers.h); }

// 0xBD
void cp_a_l(void) { sub_n(registers.l); }

// 0xBE
void cp_a_hl_v(void) { sub_n(readByte(registers.hl)); }

// 0xBF
void cp_a_a(void) { sub_n(registers.a); }

// 0xC0
void ret_nz(void)
{
    if (~flagIsSet(FLAG_Z))
    {
        registers.pc = popStack();
        cycles += 12;
    }
}

// 0xC1
void pop_bc(void) { registers.bc = popStack(); }

// 0xC2
void jp_nz_nn(unsigned short nn)
{
    if (~flagIsSet(FLAG_Z))
    {
        registers.pc = nn;
        cycles += 4;
    }
}

// 0xC3
void jp_nn(unsigned short nn)
{
    registers.pc = nn;
}

// 0xC4
void call_nz_nn(unsigned short nn)
{
    if (~flagIsSet(FLAG_Z))
    {
        pushStack(registers.pc);
        registers.pc = nn;
        cycles += 12;
    }
}

// 0xC5
void push_bc(void) { pushStack(registers.bc); }

// 0xC6
void add_a_n(unsigned char n) { registers.a = add_n_n(registers.a, n); }

// 0xC7
void rst_0(void)
{
    pushStack(registers.pc);
    registers.pc = 0;
}

// 0xC8
void ret_z(void)
{
    if (flagIsSet(FLAG_Z))
    {
        registers.pc = popStack();
        cycles += 12;
    }
}

// 0xC9
void ret(void)
{
    registers.pc = popStack();
}

// 0xCA
void jp_z_nn(unsigned short nn)
{
    if (flagIsSet(FLAG_Z))
    {
        registers.pc = nn;
        cycles += 4;
    }
}

// 0xCB
void ext_ops(void)
{
    // TODO implement this
}

// 0xCC
void call_z_nn(unsigned short nn)
{
    if (flagIsSet(FLAG_Z))
    {
        pushStack(registers.pc);
        registers.pc = nn;
        cycles += 12;
    }
}

// 0xCD
void call_nn(unsigned short nn)
{
    pushStack(registers.pc);
    registers.pc = nn;
}

// 0xCE
void adc_a_n(unsigned char n) { registers.a = adc_n(n); }

// 0xCF
void rst_8(void)
{
    pushStack(registers.pc);
    registers.pc = 0x8;
}

// 0xD0
void ret_nc(void)
{
    if (~flagIsSet(FLAG_C))
    {
        registers.pc = popStack();
        cycles += 12;
    }
}

// 0xD1
void pop_de(void) { registers.de = popStack(); }

// 0xD2
void jp_nc_nn(unsigned short nn)
{
    if (~flagIsSet(FLAG_C))
    {
        registers.pc = nn;
        cycles += 4;
    }
}

// 0xD4
void call_nc_nn(unsigned short nn)
{
    if (~flagIsSet(FLAG_C))
    {
        pushStack(registers.pc);
        registers.pc = nn;
        cycles += 12;
    }
}

// 0xD5
void push_de(void) { pushStack(registers.de); }

// 0xD6
void sub_a_n(unsigned char n) { registers.a = sub_n(n); }

// 0xD7
void rst_10(void)
{
    pushStack(registers.pc);
    registers.pc = 0x10;
}

// 0xD8
void ret_c(void)
{
    if (flagIsSet(FLAG_C))
    {
        registers.pc = popStack();
        cycles += 12;
    }
}

// 0xD9
void reti(void)
{
    registers.pc = popStack();
    // TODO interrupt stuff
}

// 0xDA
void jp_c_nn(unsigned short nn)
{
    if (flagIsSet(FLAG_C))
    {
        registers.pc = nn;
        cycles += 4;
    }
}

// 0xDC
void call_c_nn(unsigned short nn)
{
    if (flagIsSet(FLAG_C))
    {
        pushStack(registers.pc);
        registers.pc = nn;
        cycles += 12;
    }
}

// 0xDE
void sbc_a_n(unsigned char n) { registers.a = sbc_n(n); }

// 0xDF
void rst_18(void)
{
    pushStack(registers.pc);
    registers.pc = 0x18;
}

// 0xE0
void ldh_n_a(unsigned char n) { writeByte((unsigned short) 0xFF00 + n, registers.a); }

// 0xE1
void pop_hl(void) { registers.hl = popStack(); }

// 0xE2
void ldh_c_a(void) { writeByte((unsigned short) 0xFF00 + registers.c, registers.a); }

// 0xE5
void push_hl(void) { pushStack(registers.hl); }

// 0xE6
void and_a_n(unsigned char n) { registers.a = and_n(n); }

// 0xE7
void rst_20(void)
{
    pushStack(registers.pc);
    registers.pc = 0x20;
}

// 0xE8
void add_sp_d(char d) { registers.sp += d; }

// 0xE9
void jp_hl(void)
{
    registers.pc = registers.hl;
    cycles += 4;
}

// 0xEA
void ld_nn_a(unsigned short nn) { writeByte(nn, registers.a); }

// 0xEE
void xor_a_n(unsigned char n) { registers.a = xor_n(n); }

// 0xEF
void rst_28(void)
{
    pushStack(registers.pc);
    registers.pc = 0x28;
}

// 0xF0
void ldh_a_n(unsigned char n) { registers.a = readByte((unsigned short) 0xFF00 + n); }

// 0xF1
void pop_af(void) { registers.af = popStack(); }

// 0xF3
void di(void)
{
    interrupts.ime = 0;
}

// 0xF5
void push_af(void) { pushStack(registers.af); }

// 0xF6
void or_a_n(unsigned char n) { registers.a = or_n(n); }

// 0xF7
void rst_30(void)
{
    pushStack(registers.pc);
    registers.pc = 0x30;
}

// 0xF8
void ldhl_sp_d(char d)
{
    unsigned long res = registers.sp + d;
    flagSet(FLAG_N, 0);
    flagSet(FLAG_H, ((registers.sp & 0xFFF) + (d & 0xFFF)) > 0xFFF);
    flagSet(FLAG_C, res & 0xFFFF0000);
    registers.hl = (unsigned short) res;
}

// 0xF9
void ld_sp_hl(void) { registers.sp = registers.hl; }

// 0xFA
void ld_a_nn_v(unsigned short nn) { registers.a = readByte(nn); }

// 0xFB
void ei(void)
{
    interrupts.ime = 1;
}

// 0xFE
void cp_a_n(unsigned char n) { sub_n(n); }

// 0xFF
void rst_38(void)
{
    pushStack(registers.pc);
    registers.pc = 0x38;
}


// Extended instructions

// 0x00
void rlc_b(void) { registers.b = rlc_n(registers.b); }

// 0x01
void rlc_c(void) { registers.c = rlc_n(registers.c); }

// 0x02
void rlc_d(void) { registers.d = rlc_n(registers.d); }

// 0x03
void rlc_e(void) { registers.e = rlc_n(registers.e); }

// 0x04
void rlc_h(void) { registers.h = rlc_n(registers.h); }

// 0x05
void rlc_l(void) { registers.l = rlc_n(registers.l); }

// 0x06
void rlc_hl_v(void) { writeByte(registers.hl, rlc_n(readByte(registers.hl))); }

// 0x08
void rrc_b(void) { registers.b = rrc_n(registers.b); }

// 0x09
void rrc_c(void) { registers.c = rrc_n(registers.c); }

// 0x0A
void rrc_d(void) { registers.d = rrc_n(registers.d); }

// 0x0B
void rrc_e(void) { registers.e = rrc_n(registers.e); }

// 0x0C
void rrc_h(void) { registers.h = rrc_n(registers.h); }

// 0x0D
void rrc_l(void) { registers.l = rrc_n(registers.l); }

// 0x0E
void rrc_hl_v(void) { writeByte(registers.hl, rrc_n(readByte(registers.hl))); }

// 0x10
void rl_b(void) { registers.b = rl_n(registers.b); }

// 0x11
void rl_c(void) { registers.c = rl_n(registers.c); }

// 0x12
void rl_d(void) { registers.d = rl_n(registers.d); }

// 0x13
void rl_e(void) { registers.e = rl_n(registers.e); }

// 0x14
void rl_h(void) { registers.h = rl_n(registers.h); }

// 0x15
void rl_l(void) { registers.l = rl_n(registers.l); }

// 0x16
void rl_hl_v(void) { writeByte(registers.hl, rl_n(readByte(registers.hl))); }

// 0x18
void rr_b(void) { registers.b = rr_n(registers.b); }

// 0x19
void rr_c(void) { registers.c = rr_n(registers.c); }

// 0x1A
void rr_d(void) { registers.d = rr_n(registers.d); }

// 0x1B
void rr_e(void) { registers.e = rr_n(registers.e); }

// 0x1C
void rr_h(void) { registers.h = rr_n(registers.h); }

// 0x1D
void rr_l(void) { registers.l = rr_n(registers.l); }

// 0x1E
void rr_hl_v(void) { writeByte(registers.hl, rr_n(readByte(registers.hl))); }

// 0x20
void sla_b(void) { registers.b = sla_n(registers.b); }

// 0x21
void sla_c(void) { registers.c = sla_n(registers.c); }

// 0x22
void sla_d(void) { registers.d = sla_n(registers.d); }

// 0x23
void sla_e(void) { registers.e = sla_n(registers.e); }

// 0x24
void sla_h(void) { registers.h = sla_n(registers.h); }

// 0x25
void sla_l(void) { registers.l = sla_n(registers.l); }

// 0x26
void sla_hl_v(void) { writeByte(registers.hl, sla_n(readByte(registers.hl))); }

// 0x27
void sla_a(void) { registers.a = sla_n(registers.a); }

// 0x28
void sra_b(void) { registers.b = sra_n(registers.b); }

// 0x29
void sra_c(void) { registers.c = sra_n(registers.c); }

// 0x2A
void sra_d(void) { registers.d = sra_n(registers.d); }

// 0x2B
void sra_e(void) { registers.e = sra_n(registers.e); }

// 0x2C
void sra_h(void) { registers.h = sra_n(registers.h); }

// 0x2D
void sra_l(void) { registers.l = sra_n(registers.l); }

// 0x2E
void sra_hl_v(void) { writeByte(registers.hl, sra_n(readByte(registers.hl))); }

// 0x2F
void sra_a(void) { registers.a = sra_n(registers.a); }

// 0x30
void swap_b(void) { registers.b = swap_n(registers.b); }

// 0x31
void swap_c(void) { registers.c = swap_n(registers.c); }

// 0x32
void swap_d(void) { registers.d = swap_n(registers.d); }

// 0x33
void swap_e(void) { registers.e = swap_n(registers.e); }

// 0x34
void swap_h(void) { registers.h = swap_n(registers.h); }

// 0x35
void swap_l(void) { registers.l = swap_n(registers.l); }

// 0x36
void swap_hl_v(void) { writeByte(registers.hl, swap_n(readByte(registers.hl))); }

// 0x37
void swap_a(void) { registers.a = swap_n(registers.a); }

// 0x38
void srl_b(void) { registers.b = srl_n(registers.b); }

// 0x39
void srl_c(void) { registers.c = srl_n(registers.c); }

// 0x3A
void srl_d(void) { registers.d = srl_n(registers.d); }

// 0x3B
void srl_e(void) { registers.e = srl_n(registers.e); }

// 0x3C
void srl_h(void) { registers.h = srl_n(registers.h); }

// 0x3D
void srl_l(void) { registers.l = srl_n(registers.l); }

// 0x3E
void srl_hl_v(void) { writeByte(registers.hl, srl_n(readByte(registers.hl))); }

// 0x3F
void srl_a(void) { registers.a = srl_n(registers.a); }

// 0x40
void bit_0_b(void) { bit_n(registers.b, 0); }

// 0x41
void bit_0_c(void) { bit_n(registers.c, 0); }

// 0x42
void bit_0_d(void) { bit_n(registers.d, 0); }

// 0x43
void bit_0_e(void) { bit_n(registers.e, 0); }

// 0x44
void bit_0_h(void) { bit_n(registers.h, 0); }

// 0x45
void bit_0_l(void) { bit_n(registers.l, 0); }

// 0x46
void bit_0_hl_v(void) { bit_n(readByte(registers.hl), 0); }

// 0x47
void bit_0_a(void) { bit_n(registers.a, 0); }

// 0x48
void bit_1_b(void) { bit_n(registers.b, 1); }

// 0x49
void bit_1_c(void) { bit_n(registers.c, 1); }

// 0x4A
void bit_1_d(void) { bit_n(registers.d, 1); }

// 0x4B
void bit_1_e(void) { bit_n(registers.e, 1); }

// 0x4C
void bit_1_h(void) { bit_n(registers.h, 1); }

// 0x4D
void bit_1_l(void) { bit_n(registers.l, 1); }

// 0x4E
void bit_1_hl_v(void) { bit_n(readByte(registers.hl), 1); }

// 0x4F
void bit_1_a(void) { bit_n(registers.a, 1); }

// 0x50
void bit_2_b(void) { bit_n(registers.b, 2); }

// 0x51
void bit_2_c(void) { bit_n(registers.c, 2); }

// 0x52
void bit_2_d(void) { bit_n(registers.d, 2); }

// 0x53
void bit_2_e(void) { bit_n(registers.e, 2); }

// 0x54
void bit_2_h(void) { bit_n(registers.h, 2); }

// 0x55
void bit_2_l(void) { bit_n(registers.l, 2); }

// 0x56
void bit_2_hl_v(void) { bit_n(readByte(registers.hl), 2); }

// 0x57
void bit_2_a(void) { bit_n(registers.a, 2); }

// 0x58
void bit_3_b(void) { bit_n(registers.b, 3); }

// 0x59
void bit_3_c(void) { bit_n(registers.c, 3); }

// 0x5A
void bit_3_d(void) { bit_n(registers.d, 3); }

// 0x5B
void bit_3_e(void) { bit_n(registers.e, 3); }

// 0x5C
void bit_3_h(void) { bit_n(registers.h, 3); }

// 0x5D
void bit_3_l(void) { bit_n(registers.l, 3); }

// 0x5E
void bit_3_hl_v(void) { bit_n(readByte(registers.hl), 3); }

// 0x5F
void bit_3_a(void) { bit_n(registers.a, 3); }

// 0x60
void bit_4_b(void) { bit_n(registers.b, 4); }

// 0x61
void bit_4_c(void) { bit_n(registers.c, 4); }

// 0x62
void bit_4_d(void) { bit_n(registers.d, 4); }

// 0x63
void bit_4_e(void) { bit_n(registers.e, 4); }

// 0x64
void bit_4_h(void) { bit_n(registers.h, 4); }

// 0x65
void bit_4_l(void) { bit_n(registers.l, 4); }

// 0x66
void bit_4_hl_v(void) { bit_n(readByte(registers.hl), 4); }

// 0x67
void bit_4_a(void) { bit_n(registers.a, 4); }

// 0x68
void bit_5_b(void) { bit_n(registers.b, 5); }

// 0x69
void bit_5_c(void) { bit_n(registers.c, 5); }

// 0x6A
void bit_5_d(void) { bit_n(registers.d, 5); }

// 0x6B
void bit_5_e(void) { bit_n(registers.e, 5); }

// 0x6C
void bit_5_h(void) { bit_n(registers.h, 5); }

// 0x6D
void bit_5_l(void) { bit_n(registers.l, 5); }

// 0x6E
void bit_5_hl_v(void) { bit_n(readByte(registers.hl), 5); }

// 0x6F
void bit_5_a(void) { bit_n(registers.a, 5); }

// 0x70
void bit_6_b(void) { bit_n(registers.b, 6); }

// 0x71
void bit_6_c(void) { bit_n(registers.c, 6); }

// 0x72
void bit_6_d(void) { bit_n(registers.d, 6); }

// 0x73
void bit_6_e(void) { bit_n(registers.e, 6); }

// 0x74
void bit_6_h(void) { bit_n(registers.h, 6); }

// 0x75
void bit_6_l(void) { bit_n(registers.l, 6); }

// 0x76
void bit_6_hl_v(void) { bit_n(readByte(registers.hl), 6); }

// 0x77
void bit_6_a(void) { bit_n(registers.a, 6); }

// 0x78
void bit_7_b(void) { bit_n(registers.b, 7); }

// 0x79
void bit_7_c(void) { bit_n(registers.c, 7); }

// 0x7A
void bit_7_d(void) { bit_n(registers.d, 7); }

// 0x7B
void bit_7_e(void) { bit_n(registers.e, 7); }

// 0x7C
void bit_7_h(void) { bit_n(registers.h, 7); }

// 0x7D
void bit_7_l(void) { bit_n(registers.l, 7); }

// 0x7E
void bit_7_hl_v(void) { bit_n(readByte(registers.hl), 7); }

// 0x7F
void bit_7_a(void) { bit_n(registers.a, 7); }
