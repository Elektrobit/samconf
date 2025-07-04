// SPDX-License-Identifier: MIT

#ifndef __MOCK_SAMCONF_SAMCONF_H__
#define __MOCK_SAMCONF_SAMCONF_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_SAMCONF_USE_WRAP
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "samconf/config_backend.h"
#include "samconf/crypto_utils.h"
#include "samconf/json_backend.h"
#include "samconf/samconf.h"
#include "samconf/uri.h"

MOCK_FUNC_PROTOTYPE(samconfGetBackendOps, const samconfConfigBackendOps_t *, samconfBackendOptions_t idx)
MOCK_FUNC_PROTOTYPE(samconfLookupBackend, samconfConfigStatusE_t, const char *location,
                    samconfConfigBackend_t **backend)
MOCK_FUNC_PROTOTYPE(samconfVerifySignature, samconfConfigStatusE_t, const char *location)
MOCK_FUNC_PROTOTYPE(samconfConfigBackendNew, samconfConfigStatusE_t, samconfConfigBackend_t **backend,
                    const samconfConfigBackendOps_t *ops)
MOCK_FUNC_PROTOTYPE(samconfConfigBackendInit, samconfConfigStatusE_t, samconfConfigBackend_t *backend,
                    const samconfConfigBackendOps_t *ops)
MOCK_FUNC_PROTOTYPE(samconfConfigBackendDelete, samconfConfigStatusE_t, samconfConfigBackend_t *backend)
MOCK_FUNC_PROTOTYPE(samconfConfigGet, samconfConfigStatusE_t, const samconfConfig_t *root, const char *path,
                    const samconfConfig_t **result)
MOCK_FUNC_PROTOTYPE(samconfConfigGetBool, samconfConfigStatusE_t, const samconfConfig_t *root, const char *path,
                    bool *result)
MOCK_FUNC_PROTOTYPE(samconfConfigGetInt32, samconfConfigStatusE_t, const samconfConfig_t *root, const char *path,
                    int32_t *result)
MOCK_FUNC_PROTOTYPE(samconfConfigGetReal, samconfConfigStatusE_t, const samconfConfig_t *root, const char *path,
                    double *result)
MOCK_FUNC_PROTOTYPE(samconfConfigGetString, samconfConfigStatusE_t, const samconfConfig_t *root, const char *path,
                    const char **result)
MOCK_FUNC_PROTOTYPE(samconfConfigDelete, samconfConfigStatusE_t, samconfConfig_t *config)
MOCK_FUNC_PROTOTYPE(samconfConfigDeleteMembers, samconfConfigStatusE_t, samconfConfig_t *config)
MOCK_FUNC_PROTOTYPE(samconfConfigNew, samconfConfigStatusE_t, samconfConfig_t **config)
MOCK_FUNC_PROTOTYPE(samconfConfigAdd, samconfConfigStatusE_t, samconfConfig_t *parent, samconfConfig_t *child)
MOCK_FUNC_PROTOTYPE(samconfJsonBackendSupports, samconfConfigStatusE_t, const char *location, bool *isSupported)
MOCK_FUNC_PROTOTYPE(samconfJsonBackendOpen, samconfConfigStatusE_t, const char *location,
                    samconfConfigBackend_t *backend)
MOCK_FUNC_PROTOTYPE(samconfJsonBackendLoad, samconfConfigStatusE_t, samconfConfigBackend_t *backend, bool isSigned,
                    samconfConfig_t **config)
MOCK_FUNC_PROTOTYPE(samconfJsonBackendClose, samconfConfigStatusE_t, samconfConfigBackend_t *backend)

MOCK_FUNC_PROTOTYPE(samconfEnvBackendSupports, samconfConfigStatusE_t, const char *location, bool *isSupported)
MOCK_FUNC_PROTOTYPE(samconfEnvBackendOpen, samconfConfigStatusE_t, const char *location,
                    samconfConfigBackend_t *backend)
MOCK_FUNC_PROTOTYPE(samconfEnvBackendLoad, samconfConfigStatusE_t, samconfConfigBackend_t *backend, bool isSigned,
                    samconfConfig_t **config)
MOCK_FUNC_PROTOTYPE(samconfEnvBackendClose, samconfConfigStatusE_t, samconfConfigBackend_t *backend)

MOCK_FUNC_PROTOTYPE(samconfUriPattern, samconfConfigStatusE_t, regex_t **pattern)
MOCK_FUNC_PROTOTYPE(samconfUriNew, samconfConfigStatusE_t, samconfUri_t **uri, const char *uriString)
MOCK_FUNC_PROTOTYPE(samconfUriInit, samconfConfigStatusE_t, samconfUri_t *uri, const char *uriString)
MOCK_FUNC_PROTOTYPE(samconfUriDelete, samconfConfigStatusE_t, samconfUri_t *uri)
MOCK_FUNC_PROTOTYPE(samconfUriDeleteMembers, samconfConfigStatusE_t, samconfUri_t *uri)

MOCK_FUNC_PROTOTYPE(samconfLoadPublicKey, samconfConfigStatusE_t, EVP_PKEY **pKey)
MOCK_FUNC_PROTOTYPE(samconfLoad, samconfConfigStatusE_t, const char *location, bool enforceSignature,
                    samconfConfig_t **const config)
MOCK_FUNC_PROTOTYPE(samconfLoadAndMerge, samconfConfigStatusE_t, const samconfConfigLocation_t locations[],
                    size_t locationsSize, samconfConfig_t **const config)
MOCK_FUNC_PROTOTYPE(samconfBase64Decode, samconfConfigStatusE_t, const char *base64, uint8_t **buf, size_t *bufLen)
MOCK_FUNC_PROTOTYPE(samconfCryptoUtilsVerify, samconfConfigStatusE_t, const char *data, size_t length,
                    const char *signature)

MOCK_FUNC_PROTOTYPE(samconfConfigGetStringOr, const char *, const samconfConfig_t *root, const char *path,
                    const char *defaultValue)
MOCK_FUNC_PROTOTYPE(samconfConfigGetBoolOr, bool, const samconfConfig_t *root, const char *path, bool defaultValue)
MOCK_FUNC_PROTOTYPE(samconfConfigGetInt32Or, int32_t, const samconfConfig_t *root, const char *path,
                    int32_t defaultValue)
MOCK_FUNC_PROTOTYPE(samconfConfigGetRealOr, double, const samconfConfig_t *root, const char *path, double defaultValue)

MOCK_FUNC_PROTOTYPE(samconfConfigSetValueFromString, samconfConfigStatusE_t, samconfConfig_t *config, const char *value)

MOCK_FUNC_PROTOTYPE(samconfConfigNext, samconfConfigStatusE_t, const samconfConfig_t *root,
                    const samconfConfig_t *configToFind, const samconfConfig_t **nextConfig)

MOCK_FUNC_PROTOTYPE(samconfConfigMergeConfig, samconfConfigStatusE_t, samconfConfig_t **mergedConfig,
                    samconfConfig_t *configToMerge)

MOCK_FUNC_PROTOTYPE(samconfConfigMergeConfigs, samconfConfigStatusE_t, samconfConfig_t **mergedConfig,
                    samconfConfig_t **configsToMerge, size_t configsCount)

MOCK_FUNC_PROTOTYPE(samconfInsertAt, samconfConfigStatusE_t, samconfConfig_t **root, const char *path,
                    samconfConfig_t *config)
MOCK_FUNC_PROTOTYPE(samconfCreateIntAt, samconfConfigStatusE_t, samconfConfig_t **root, const char *path, int64_t value)
MOCK_FUNC_PROTOTYPE(samconfCreateRealAt, samconfConfigStatusE_t, samconfConfig_t **root, const char *path, double value)
MOCK_FUNC_PROTOTYPE(samconfCreateBoolAt, samconfConfigStatusE_t, samconfConfig_t **root, const char *path, bool value)
MOCK_FUNC_PROTOTYPE(samconfCreateFromStringAt, samconfConfigStatusE_t, samconfConfig_t **root, const char *path,
                    char *value)
MOCK_FUNC_PROTOTYPE(samconfCreateStringAt, samconfConfigStatusE_t, samconfConfig_t **root, const char *path,
                    const char *value)
MOCK_FUNC_PROTOTYPE(samconfCopyConfigValue, samconfConfigStatusE_t, const samconfConfig_t *from, samconfConfig_t *to)
MOCK_FUNC_PROTOTYPE(samconfConfigCopyConfig, samconfConfigStatusE_t, const samconfConfig_t *from, samconfConfig_t *to)
#endif /* __MOCK_SAMCONF_SAMCONF_H__ */
