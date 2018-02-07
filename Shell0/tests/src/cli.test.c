#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <stdio.h>
#include <string.h>

#include "../include/cmocka.h"
#include "../../include/cli.h"

const struct CMUnitTest open_i2c_tests[] = {

};

int main(void)
{
    return cmocka_run_group_tests(open_i2c_tests, NULL, NULL);
}
