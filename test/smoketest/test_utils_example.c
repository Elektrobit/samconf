// SPDX-License-Identifier: MIT

#include <samconf/samconf.h>
#include <samconf/test_utils.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    samconfConfig_t config = {0};
    samconfUtilCreateMockConfigFromStr("{\"optionName\": 42}", false, &config);

    int32_t value = 0;
    samconfConfigGetInt32(&config, "/optionName", &value);

    printf("%s is %i\n", config.children[0]->key, value);

    samconfConfigDeleteMembers(&config);
    return EXIT_SUCCESS;
}
