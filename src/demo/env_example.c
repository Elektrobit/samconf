// SPDX-License-Identifier: MIT
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "samconf/samconf.h"

static void _set_env_variables() {
    setenv("SAMCONF_VARIABLE_STRING", "utest string", 1);
    setenv("SAMCONF_VARIABLE_INT", "42", 1);
    setenv("SAMCONF_VARIABLE_REAL", "1.9865", 1);
    setenv("SAMCONF_VARIABLE_BOOL", "true", 1);
    setenv("SAMCONF_SAMCONF_VARIABLE_INT", "42", 1);
}

static void _unset_env_variables() {
    unsetenv("SAMCONF_VARIABLE_STRING");
    unsetenv("SAMCONF_VARIABLE_INT");
    unsetenv("SAMCONF_VARIABLE_REAL");
    unsetenv("SAMCONF_VARIABLE_BOOL");
    unsetenv("SAMCONF_SAMCONF_VARIABLE_INT");
}

int main(UNUSED int argc, UNUSED char **argv) {
    samconfConfig_t *config = NULL;
    const samconfConfig_t *node = NULL;
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    const char *location = "env://SAMCONF";

    const char *paths[] = {"SAMCONF/VARIABLE/STRING", "SAMCONF/VARIABLE/INT", "SAMCONF/VARIABLE/REAL",
                           "SAMCONF/VARIABLE/BOOL", "SAMCONF/SAMCONF/VARIABLE/INT"};

    _set_env_variables();
    result = samconfLoad(location, false, &config);

    fprintf(stdout, "%-30s\t%-30s\t%-30s\t%-30s\n", "PATH", "KEY", "VALUE", "TYPE");
    for (size_t i = 0; i < 5; i++) {
        result = samconfConfigGet(config, paths[i], &node);
        switch (node->type) {
            case SAMCONF_CONFIG_VALUE_STRING:
                fprintf(stdout, "%-30s\t%-30s\t%-30s\t%-30d\n", paths[i], node->key, node->value.string, node->type);
                break;
            case SAMCONF_CONFIG_VALUE_INT:
                fprintf(stdout, "%-30s\t%-30s\t%-30ld\t%-30d\n", paths[i], node->key, node->value.integer, node->type);
                break;
            case SAMCONF_CONFIG_VALUE_BOOLEAN:
                fprintf(stdout, "%-30s\t%-30s\t%-30d\t%-30d\n", paths[i], node->key, node->value.boolean, node->type);
                break;
            case SAMCONF_CONFIG_VALUE_REAL:
                fprintf(stdout, "%-30s\t%-30s\t%-30f\t%-30d\n", paths[i], node->key, node->value.real, node->type);
                break;
            case SAMCONF_CONFIG_VALUE_ARRAY:
                fprintf(stdout, "%-30s\t%-30s\t%-30s\t%-30d\n", paths[i], node->key, "None", node->type);
                break;
            case SAMCONF_CONFIG_VALUE_OBJECT:
                fprintf(stdout, "%-30s\t%-30s\t%-30s\t%-30d\n", paths[i], node->key, "None", node->type);
                break;
            default:
                fprintf(stdout, "%-30s\t%-30s\t%-30s\t%-30d\n", paths[i], node->key, "None", node->type);
                break;
        }
    }

    result = samconfConfigDelete(config);

    _unset_env_variables();

    return result;
}
