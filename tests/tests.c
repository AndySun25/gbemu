#include <check.h>
#include <stdio.h>
#include "memory.h"
#include "cpu.h"
#include "registers.h"

void setUp(unsigned char instruction)
{
    reset();
    debugWriteByte(0x100, instruction);
}

START_TEST(test_nop)
    {
        setUp(0x0);
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
        setUp(0x1);
        unsigned short address = 0x101;
        unsigned short val = 0xFF;
        debugWriteShort(address, val);
        cycle();
        ck_assert(flagIsSet(FLAG_Z) == 0);
        ck_assert(flagIsSet(FLAG_N) == 0);
        ck_assert(flagIsSet(FLAG_H) == 0);
        ck_assert(flagIsSet(FLAG_C) == 0);
        ck_assert(readShort(address) == 0xFF);
        ck_assert(registers.bc == 0xFF);
        ck_assert(registers.pc == 0x103);
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
