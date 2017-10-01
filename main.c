#include <check.h>
#include <stdlib.h>
#include "memory.h"
#include "cpu.h"

START_TEST(test_nop)
    {
        reset();
        writeByte(0x10, 0x0);
        cycle();
    }
END_TEST


Suite * cpu_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("CPU");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_nop);
    suite_add_tcase(s, tc_core);
    return s;
}


int main()
{
    int number_failed;
    Suite *suite = cpu_suite();
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return number_failed;
}
