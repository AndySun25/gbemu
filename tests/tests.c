#include <check.h>
#include "memory.h"
#include "cpu.h"
#include "registers.h"

void setUpInstruction(unsigned char instruction, void *value)
{
    reset();
    debugWriteByte(0x100, instruction);

    if (instructions[instruction].operand_length == 1)
    {
        debugWriteByte(0x101, *((unsigned char *) value));
    }
    else if (instructions[instruction].operand_length == 2)
    {
        debugWriteShort(0x101, *((unsigned short *) value));
    }
}

START_TEST(test_nop)
    {
        setUpInstruction(0x0, 0);

        cycle();
        ck_assert(flagIsSet(FLAG_Z) == 0);
        ck_assert(flagIsSet(FLAG_N) == 0);
        ck_assert(flagIsSet(FLAG_H) == 0);
        ck_assert(flagIsSet(FLAG_C) == 0);
        ck_assert(registers.pc == 0x101);
    }
END_TEST

START_TEST(test_ld_bc_nn)
    {
        unsigned short val = 0xFFFF;
        setUpInstruction(0x1, &val);

        cycle();
        ck_assert(flagIsSet(FLAG_Z) == 0);
        ck_assert(flagIsSet(FLAG_N) == 0);
        ck_assert(flagIsSet(FLAG_H) == 0);
        ck_assert(flagIsSet(FLAG_C) == 0);
        ck_assert(registers.bc == val);
        ck_assert(registers.pc == 0x103);
    }
END_TEST

START_TEST(test_ld_bc_v_a)
    {
        unsigned char val = 0xFF;
        unsigned short address = 0x8000;
        setUpInstruction(0x2, 0);

        registers.a = val;
        registers.bc = address;
        cycle();
        ck_assert(flagIsSet(FLAG_Z) == 0);
        ck_assert(flagIsSet(FLAG_N) == 0);
        ck_assert(flagIsSet(FLAG_H) == 0);
        ck_assert(flagIsSet(FLAG_C) == 0);
        ck_assert(readByte(address) == val);
        ck_assert(registers.pc == 0x101);
    }
END_TEST

START_TEST(test_inc_bc)
    {
        unsigned short val = 0x1;
        setUpInstruction(0x3, 0);

        registers.bc = val;
        cycle();
        ck_assert(flagIsSet(FLAG_Z) == 0);
        ck_assert(flagIsSet(FLAG_N) == 0);
        ck_assert(flagIsSet(FLAG_H) == 0);
        ck_assert(flagIsSet(FLAG_C) == 0);
        ck_assert(registers.bc == val + 1);
        ck_assert(registers.pc == 0x101);
    }
END_TEST


Suite * cpu_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("CPU");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_nop);
    tcase_add_test(tc_core, test_ld_bc_nn);
    tcase_add_test(tc_core, test_ld_bc_v_a);
    tcase_add_test(tc_core, test_inc_bc);
    suite_add_tcase(s, tc_core);
    return s;
}


int main()
{
    int number_failed;
    Suite *suite = cpu_suite();
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_VERBOSE);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return number_failed;
}
