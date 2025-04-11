// SPDX-License-Identifier: MIT
#pragma once

#include <stdlib.h>

#include "samconf/samconf.h"

/*******************************************************************
 * Convert given path in to array of path segments and return array
 * element count.
 *
 * Parameters:
 *     path : path to be converted to an array
 *     patharray : array containing the path segments.
 *     count : array element count.
 *
 * Returns:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 ******************************************************************/
samconfConfigStatusE_t samconfPathCreateArray(const char *path, char ***patharray, int *count);

/*******************************************************************
 * Delete given path array.
 *
 * Parameters:
 *     patharray : array containing the path segments.
 *     count : array element count.
 *
 * Returns:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 ******************************************************************/
void samconfPathDeleteArray(char **patharray, int count);

/*******************************************************************
 * Create a path string from segments from the given patharray till
 * the given index.
 *
 * Parameters:
 *     patharray : array containing the path segments.
 *     index : element index up to which segments for path are taken.
 *     newPath : path string created with segments till given index.
 *
 * Returns:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 ******************************************************************/
samconfConfigStatusE_t samconfPathGetPathUntil(char **patharray, int index, char **newPath);
