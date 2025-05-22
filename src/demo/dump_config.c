// SPDX-License-Identifier: MIT

#include <json-c/json_object.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "samconf/json_backend.h"
#include "samconf/samconf.h"
#include "samconf/samconf_types.h"

static void _printHelp(const char *const name) {
    printf("Load a config files and dump and it as JSON or a tree\n");
    printf("\n");
    printf("Usage: %s [OPTIONS] <CONFIG>\n", name);
    printf("\n");
    printf("Options:\n");
    printf("  -j, --json                Dump the config as JSON\n");
    printf("  -t, --tree                Dump the config tree\n");
    printf("  -s, --enforceSignature    Enforce the signature for the config\n");
    printf("  -h, --help                Print this help message\n");
    printf("\n");
}

int main(int argc, char **argv) {
    samconfConfig_t *config = NULL;

    bool jsonDump = false;
    bool treeDump = false;
    bool enforceSignature = false;
    const char *location = NULL;

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            _printHelp(argv[0]);
            exit(0);
        } else if (strcmp(arg, "-j") == 0 || strcmp(arg, "--json") == 0) {
            jsonDump = true;
        } else if (strcmp(arg, "-t") == 0 || strcmp(arg, "--tree") == 0) {
            treeDump = true;
        } else if (strcmp(arg, "-s") == 0 || strcmp(arg, "--enforceSignature") == 0) {
            enforceSignature = true;
        } else {
            if (location == NULL) {
                location = argv[1];
            } else {
                fprintf(stderr, "only one config can be dumped\n");
                exit(2);
            }
        }
    }
    if (!jsonDump && !treeDump) {
        jsonDump = true;
    }

    if (location == NULL) {
        fprintf(stderr, "mising config to load\n");
        exit(1);
    }
    samconfConfigStatusE_t result = samconfLoad(location, enforceSignature, &config);
    if (result != SAMCONF_CONFIG_OK) {
        fprintf(stderr, "ERROR reading config\n");
    }
    if (jsonDump) {
        struct json_object *jconf = samconfDumpJsonConfig(config);
        printf("%s\n", json_object_to_json_string_ext(jconf, JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_SPACED));
        json_object_put(jconf);
    }
    if (treeDump) {
        samconfDumpConfigTree(config);
    }
    result = samconfConfigDelete(config);
    if (result != SAMCONF_CONFIG_OK) {
        fprintf(stderr, "ERROR freeing the config object\n");
    }

    return 0;
}
