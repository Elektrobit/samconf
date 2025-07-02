// SPDX-License-Identifier: MIT

#include <bits/time.h>
#include <json-c/json_object.h>
#include <safu/log.h>
#include <safu/vector.h>
#include <safu/vector_types.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "samconf/json_backend.h"
#include "samconf/samconf.h"
#include "samconf/samconf_types.h"

static void _printHelp(const char *const name) {
    printf("Merge all the configurations in the provided directories or files and dump them as JSON or a tree\n");
    printf("\n");
    printf("Usage: %s [OPTIONS] <DIRECTORIES|CONFIGS>\n", name);
    printf("\n");
    printf("Options:\n");
    printf("  -j, --json                Dump the merged config as JSON\n");
    printf("  -t, --tree                Dump the merged config tree\n");
    printf("  -b, --benchmark           Benchmark the time taken to load and merge provided configs\n");
    printf("  -s, --enforceSignature    Enforce the signature for all configs after this option\n");
    printf("  -u, --allowUnsigned       Don't enforce the signature for any configs following this option\n");
    printf("  -h, --help                Print this help message\n");
    printf("\n");
}

int main(int argc, char **argv) {
    samconfConfig_t *config = NULL;
    safuLogSetStreamLevel(SAFU_LOG_LEVEL_ALL);

    bool jsonDump = false;
    bool treeDump = false;
    bool enforceSignature = false;
    bool benchmark = false;
    struct timespec startTime = {0};
    struct timespec endTime = {0};
    int ret = 0;

    safuVec_t confDirs;
    int vecRes = safuVecCreate(&confDirs, 100, sizeof(samconfConfigLocation_t));
    if (vecRes != SAMCONF_CONFIG_OK) {
        fprintf(stderr, "failed to allocate directory list\n");
    }

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            _printHelp(argv[0]);
            exit(0);
        } else if (strcmp(arg, "-j") == 0 || strcmp(arg, "--json") == 0) {
            jsonDump = true;
        } else if (strcmp(arg, "-t") == 0 || strcmp(arg, "--tree") == 0) {
            treeDump = true;
        } else if (strcmp(arg, "-b") == 0 || strcmp(arg, "--benchmark") == 0) {
            benchmark = true;
        } else if (strcmp(arg, "-s") == 0 || strcmp(arg, "--enforceSignature") == 0) {
            enforceSignature = true;
        } else if (strcmp(arg, "-u") == 0 || strcmp(arg, "--allowUnsigned") == 0) {
            enforceSignature = false;
        } else {
            samconfConfigLocation_t location = CONF_PATH(arg, enforceSignature);
            safuVecPush(&confDirs, &location);
        }
    }
    if (!jsonDump && !treeDump) {
        jsonDump = true;
    }

    if (confDirs.elementCount == 0) {
        fprintf(stderr, "ERROR: missing config paths to load\n\n");
        _printHelp(argv[0]);
        exit(1);
    }

    ret = clock_gettime(CLOCK_MONOTONIC, &startTime);
    samconfConfigStatusE_t result = samconfLoadAndMerge(confDirs.data, confDirs.elementCount, &config);
    ret = ret | clock_gettime(CLOCK_MONOTONIC, &endTime);
    if (result != SAMCONF_CONFIG_OK) {
        fprintf(stderr, "ERROR reading config (%d)\n", result);
    }
    if (ret == 0 && benchmark) {
        long nsec = endTime.tv_nsec - startTime.tv_nsec;
        printf("Load and Merge Time :  %ld s %ld ns\n", (endTime.tv_sec - startTime.tv_sec), (nsec > 0 ? nsec : 0));
    }
    safuVecFree(&confDirs);

    if (jsonDump) {
        struct json_object *jconf = samconfDumpJsonConfig(config);
        int format = JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_SPACED;
        printf("%s\n", json_object_to_json_string_ext(jconf, format));
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
