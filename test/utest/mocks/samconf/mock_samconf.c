// SPDX-License-Identifier: MIT

#include "mock_samconf.h"

#include <cmocka.h>
#include <cmocka_extensions/mock_extensions.h>
#include <samconf/samconf.h>
#include <samconf/samconf_types.h>
#include <stdlib.h>

MOCK_FUNC_BODY(samconfGetBackendOps, const samconfConfigBackendOps_t *, samconfBackendOptions_t idx) {
    if (MOCK_IS_ACTIVE(samconfGetBackendOps)) {
        check_expected(idx);
        return mock_type(const samconfConfigBackendOps_t *);
    }
    return MOCK_FUNC_REAL(samconfGetBackendOps)(idx);
}

MOCK_FUNC_BODY(samconfLookupBackend, samconfConfigStatusE_t, const char *location, samconfConfigBackend_t **backend) {
    if (MOCK_IS_ACTIVE(samconfLookupBackend)) {
        check_expected_ptr(location);
        check_expected_ptr(backend);
        if (backend != NULL) {
            *backend = mock_ptr_type(samconfConfigBackend_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfLookupBackend)(location, backend);
}

MOCK_FUNC_BODY(samconfVerifySignature, samconfConfigStatusE_t, const char *location) {
    if (MOCK_IS_ACTIVE(samconfVerifySignature)) {
        check_expected_ptr(location);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfVerifySignature)(location);
}

MOCK_FUNC_BODY(samconfConfigBackendNew, samconfConfigStatusE_t, samconfConfigBackend_t **backend,
               const samconfConfigBackendOps_t *ops) {
    if (MOCK_IS_ACTIVE(samconfConfigBackendNew)) {
        check_expected_ptr(backend);
        check_expected_ptr(ops);
        if (backend != NULL) {
            *backend = mock_ptr_type(samconfConfigBackend_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigBackendNew)(backend, ops);
}

MOCK_FUNC_BODY(samconfConfigBackendInit, samconfConfigStatusE_t, samconfConfigBackend_t *backend,
               const samconfConfigBackendOps_t *ops) {
    if (MOCK_IS_ACTIVE(samconfConfigBackendInit)) {
        check_expected_ptr(backend);
        check_expected_ptr(ops);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigBackendInit)(backend, ops);
}

MOCK_FUNC_BODY(samconfConfigBackendDelete, samconfConfigStatusE_t, samconfConfigBackend_t *backend) {
    if (MOCK_IS_ACTIVE(samconfConfigBackendDelete)) {
        check_expected_ptr(backend);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigBackendDelete)(backend);
}

MOCK_FUNC_BODY(samconfConfigGet, samconfConfigStatusE_t, const samconfConfig_t *root, const char *path,
               const samconfConfig_t **result) {
    if (MOCK_IS_ACTIVE(samconfConfigGet)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(result);
        if (result != NULL) {
            *result = mock_ptr_type(const samconfConfig_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigGet)(root, path, result);
}

MOCK_FUNC_BODY(samconfConfigGetBool, samconfConfigStatusE_t, const samconfConfig_t *root, const char *path,
               bool *result) {
    if (MOCK_IS_ACTIVE(samconfConfigGetBool)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(result);
        if (result != NULL) {
            *result = mock_type(bool);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigGetBool)(root, path, result);
}

MOCK_FUNC_BODY(samconfConfigGetInt32, samconfConfigStatusE_t, const samconfConfig_t *root, const char *path,
               int32_t *result) {
    if (MOCK_IS_ACTIVE(samconfConfigGetInt32)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(result);
        if (result != NULL) {
            *result = mock_type(int32_t);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigGetInt32)(root, path, result);
}

MOCK_FUNC_BODY(samconfConfigGetReal, samconfConfigStatusE_t, const samconfConfig_t *root, const char *path,
               double *result) {
    if (MOCK_IS_ACTIVE(samconfConfigGetReal)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(result);
        if (result != NULL) {
            *result = mock_type(double);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigGetReal)(root, path, result);
}

MOCK_FUNC_BODY(samconfConfigGetString, samconfConfigStatusE_t, const samconfConfig_t *root, const char *path,
               const char **result) {
    if (MOCK_IS_ACTIVE(samconfConfigGetString)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(result);
        if (result != NULL) {
            *result = mock_ptr_type(const char *);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigGetString)(root, path, result);
}

MOCK_FUNC_BODY(samconfConfigDelete, samconfConfigStatusE_t, samconfConfig_t *config) {
    if (MOCK_IS_ACTIVE(samconfConfigDelete)) {
        check_expected_ptr(config);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigDelete)(config);
}

MOCK_FUNC_BODY(samconfConfigDeleteMembers, samconfConfigStatusE_t, samconfConfig_t *config) {
    if (MOCK_IS_ACTIVE(samconfConfigDeleteMembers)) {
        check_expected_ptr(config);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigDeleteMembers)(config);
}

MOCK_FUNC_BODY(samconfConfigNew, samconfConfigStatusE_t, samconfConfig_t **config) {
    if (MOCK_IS_ACTIVE(samconfConfigNew)) {
        check_expected(config);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigNew)(config);
}

MOCK_FUNC_BODY(samconfConfigAdd, samconfConfigStatusE_t, samconfConfig_t *parent, samconfConfig_t *child) {
    if (MOCK_IS_ACTIVE(samconfConfigAdd)) {
        check_expected(parent);
        check_expected(child);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigAdd)(parent, child);
}

MOCK_FUNC_BODY(samconfJsonBackendSupports, samconfConfigStatusE_t, const char *location, bool *isSupported) {
    if (MOCK_IS_ACTIVE(samconfJsonBackendSupports)) {
        check_expected_ptr(location);
        check_expected_ptr(isSupported);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfJsonBackendSupports)(location, isSupported);
}

MOCK_FUNC_BODY(samconfJsonBackendOpen, samconfConfigStatusE_t, const char *location, samconfConfigBackend_t *backend) {
    if (MOCK_IS_ACTIVE(samconfJsonBackendOpen)) {
        check_expected_ptr(location);
        check_expected_ptr(backend);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfJsonBackendOpen)(location, backend);
}

MOCK_FUNC_BODY(samconfJsonBackendLoad, samconfConfigStatusE_t, samconfConfigBackend_t *backend, bool isSigned,
               samconfConfig_t **config) {
    if (MOCK_IS_ACTIVE(samconfJsonBackendLoad)) {
        check_expected_ptr(backend);
        check_expected(isSigned);
        check_expected_ptr(config);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfJsonBackendLoad)(backend, isSigned, config);
}

MOCK_FUNC_BODY(samconfJsonBackendClose, samconfConfigStatusE_t, samconfConfigBackend_t *backend) {
    if (MOCK_IS_ACTIVE(samconfJsonBackendClose)) {
        check_expected_ptr(backend);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfJsonBackendClose)(backend);
}

MOCK_FUNC_BODY(samconfEnvBackendSupports, samconfConfigStatusE_t, const char *location, bool *isSupported) {
    if (MOCK_IS_ACTIVE(samconfEnvBackendSupports)) {
        check_expected_ptr(location);
        check_expected_ptr(isSupported);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfEnvBackendSupports)(location, isSupported);
}

MOCK_FUNC_BODY(samconfEnvBackendOpen, samconfConfigStatusE_t, const char *location, samconfConfigBackend_t *backend) {
    if (MOCK_IS_ACTIVE(samconfEnvBackendOpen)) {
        check_expected_ptr(location);
        check_expected_ptr(backend);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfEnvBackendOpen)(location, backend);
}

MOCK_FUNC_BODY(samconfEnvBackendLoad, samconfConfigStatusE_t, samconfConfigBackend_t *backend, bool isSigned,
               samconfConfig_t **config) {
    if (MOCK_IS_ACTIVE(samconfEnvBackendLoad)) {
        check_expected_ptr(backend);
        check_expected(isSigned);
        check_expected_ptr(config);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfEnvBackendLoad)(backend, isSigned, config);
}

MOCK_FUNC_BODY(samconfEnvBackendClose, samconfConfigStatusE_t, samconfConfigBackend_t *backend) {
    if (MOCK_IS_ACTIVE(samconfEnvBackendClose)) {
        check_expected_ptr(backend);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfEnvBackendClose)(backend);
}

MOCK_FUNC_BODY(samconfUriNew, samconfConfigStatusE_t, samconfUri_t **uri, const char *uriString) {
    if (MOCK_IS_ACTIVE(samconfUriNew)) {
        check_expected_ptr(uri);
        check_expected(uriString);
        if (uri != NULL) {
            *uri = mock_ptr_type(samconfUri_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfUriNew)(uri, uriString);
}

MOCK_FUNC_BODY(samconfUriInit, samconfConfigStatusE_t, samconfUri_t *uri, const char *uriString) {
    if (MOCK_IS_ACTIVE(samconfUriInit)) {
        check_expected_ptr(uri);
        check_expected(uriString);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfUriInit)(uri, uriString);
}

MOCK_FUNC_BODY(samconfUriDelete, samconfConfigStatusE_t, samconfUri_t *uri) {
    if (MOCK_IS_ACTIVE(samconfUriDelete)) {
        check_expected_ptr(uri);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfUriDelete)(uri);
}

MOCK_FUNC_BODY(samconfUriDeleteMembers, samconfConfigStatusE_t, samconfUri_t *uri) {
    if (MOCK_IS_ACTIVE(samconfUriDeleteMembers)) {
        check_expected_ptr(uri);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfUriDeleteMembers)(uri);
}

MOCK_FUNC_BODY(samconfUriPattern, samconfConfigStatusE_t, regex_t **pattern) {
    if (MOCK_IS_ACTIVE(samconfUriPattern)) {
        check_expected_ptr(pattern);
        if (pattern != NULL) {
            *pattern = mock_ptr_type(regex_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfUriPattern)(pattern);
}

MOCK_FUNC_BODY(samconfLoadPublicKey, samconfConfigStatusE_t, EVP_PKEY **pKey) {
    if (MOCK_IS_ACTIVE(samconfLoadPublicKey)) {
        check_expected_ptr(pKey);
        if (pKey != NULL) {
            *pKey = mock_ptr_type(EVP_PKEY *);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfLoadPublicKey)(pKey);
}

MOCK_FUNC_VAR_NEW(samconfLoad);
samconfConfigStatusE_t MOCK_FUNC_WRAP(samconfLoad)(const char *location, bool enforceSignature,
                                                   samconfConfig_t **const config) {
    samconfConfigStatusE_t result;
    switch (MOCK_GET_TYPE(samconfLoad)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(samconfLoad)(location, enforceSignature, config);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(location);
            check_expected(enforceSignature);
            check_expected_ptr(config);
            if (config != NULL) {
                *config = mock_ptr_type(samconfConfig_t *const);
            }
            result = mock_type(samconfConfigStatusE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(samconfLoad)(location, enforceSignature, config);
    }

    return result;
}

MOCK_FUNC_VAR_NEW(samconfLoadAndMerge);
samconfConfigStatusE_t MOCK_FUNC_WRAP(samconfLoadAndMerge)(const samconfConfigLocation_t *locations,
                                                           size_t locationsSize, samconfConfig_t **const config) {
    samconfConfigStatusE_t result;
    switch (MOCK_GET_TYPE(samconfLoadAndMerge)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(samconfLoadAndMerge)(locations, locationsSize, config);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(locations);
            check_expected(locationsSize);
            check_expected_ptr(config);
            if (config != NULL) {
                *config = mock_ptr_type(samconfConfig_t *const);
            }
            result = mock_type(samconfConfigStatusE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(samconfLoadAndMerge)(locations, locationsSize, config);
            break;
    }
    return result;
}

MOCK_FUNC_BODY(samconfBase64Decode, samconfConfigStatusE_t, const char *base64, uint8_t **buf, size_t *bufLen) {
    if (MOCK_IS_ACTIVE(samconfBase64Decode)) {
        check_expected(base64);
        check_expected_ptr(buf);
        check_expected_ptr(bufLen);
        if (buf != NULL) {
            *buf = mock_ptr_type(uint8_t *);
        }
        if (bufLen != NULL) {
            *bufLen = mock_type(size_t);
        }
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfBase64Decode)(base64, buf, bufLen);
}

MOCK_FUNC_BODY(samconfCryptoUtilsVerify, samconfConfigStatusE_t, const char *data, size_t length,
               const char *signature) {
    if (MOCK_IS_ACTIVE(samconfCryptoUtilsVerify)) {
        check_expected_ptr(data);
        check_expected(length);
        check_expected_ptr(signature);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfCryptoUtilsVerify)(data, length, signature);
}

MOCK_FUNC_BODY(samconfConfigGetStringOr, const char *, const samconfConfig_t *root, const char *path,
               const char *defaultValue) {
    if (MOCK_IS_ACTIVE(samconfConfigGetStringOr)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(defaultValue);
        return mock_ptr_type(const char *);
    }
    return MOCK_FUNC_REAL(samconfConfigGetStringOr)(root, path, defaultValue);
}
MOCK_FUNC_BODY(samconfConfigGetBoolOr, bool, const samconfConfig_t *root, const char *path, bool defaultValue) {
    if (MOCK_IS_ACTIVE(samconfConfigGetBoolOr)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(defaultValue);
        return mock_type(bool);
    }
    return MOCK_FUNC_REAL(samconfConfigGetBoolOr)(root, path, defaultValue);
}
MOCK_FUNC_BODY(samconfConfigGetInt32Or, int32_t, const samconfConfig_t *root, const char *path, int32_t defaultValue) {
    if (MOCK_IS_ACTIVE(samconfConfigGetInt32Or)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(defaultValue);
        return mock_type(int32_t);
    }
    return MOCK_FUNC_REAL(samconfConfigGetInt32Or)(root, path, defaultValue);
}
MOCK_FUNC_BODY(samconfConfigGetRealOr, double, const samconfConfig_t *root, const char *path, double defaultValue) {
    if (MOCK_IS_ACTIVE(samconfConfigGetRealOr)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(defaultValue);
        return mock_type(double);
    }
    return MOCK_FUNC_REAL(samconfConfigGetRealOr)(root, path, defaultValue);
}

MOCK_FUNC_BODY(samconfConfigSetValueFromString, samconfConfigStatusE_t, samconfConfig_t *config, const char *value) {
    if (MOCK_IS_ACTIVE(samconfConfigSetValueFromString)) {
        check_expected_ptr(config);
        check_expected_ptr(value);
        return mock_type(samconfConfigStatusE_t);
    }
    return MOCK_FUNC_REAL(samconfConfigSetValueFromString)(config, value);
}

MOCK_FUNC_BODY(samconfInsertAt, samconfConfigStatusE_t, samconfConfig_t **root, const char *path,
               samconfConfig_t *config) {
    if (MOCK_IS_ACTIVE(samconfInsertAt)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(config);

        if (root != NULL) {
            *root = mock_ptr_type(samconfConfig_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }

    return MOCK_FUNC_REAL(samconfInsertAt)(root, path, config);
}

MOCK_FUNC_VAR_NEW(samconfConfigNext);
samconfConfigStatusE_t MOCK_FUNC_WRAP(samconfConfigNext)(const samconfConfig_t *root,
                                                         const samconfConfig_t *configToFind,
                                                         const samconfConfig_t **nextConfig) {
    samconfConfigStatusE_t result;
    switch (MOCK_GET_TYPE(samconfConfigNext)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(samconfConfigNext)(root, configToFind, nextConfig);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(root);
            check_expected_ptr(configToFind);
            check_expected_ptr(nextConfig);
            if (nextConfig != NULL) {
                *nextConfig = mock_ptr_type(const samconfConfig_t *);
            }
            result = mock_type(samconfConfigStatusE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(samconfConfigNext)(root, configToFind, nextConfig);
            break;
    }
    return result;
}

MOCK_FUNC_VAR_NEW(samconfConfigMergeConfig);
samconfConfigStatusE_t MOCK_FUNC_WRAP(samconfConfigMergeConfig)(samconfConfig_t **mergedConfig,
                                                                samconfConfig_t *configToMerge) {
    samconfConfigStatusE_t result;
    switch (MOCK_GET_TYPE(samconfConfigMergeConfig)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(samconfConfigMergeConfig)(mergedConfig, configToMerge);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(mergedConfig);
            check_expected_ptr(configToMerge);
            if (mergedConfig != NULL) {
                *mergedConfig = mock_ptr_type(samconfConfig_t *);
            }
            result = mock_type(samconfConfigStatusE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(samconfConfigMergeConfig)(mergedConfig, configToMerge);
            break;
    }
    return result;
}

MOCK_FUNC_VAR_NEW(samconfConfigMergeConfigs);
samconfConfigStatusE_t MOCK_FUNC_WRAP(samconfConfigMergeConfigs)(samconfConfig_t **mergedConfig,
                                                                 samconfConfig_t **configsToMerge,
                                                                 size_t configsCount) {
    samconfConfigStatusE_t result;
    switch (MOCK_GET_TYPE(samconfConfigMergeConfigs)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(samconfConfigMergeConfigs)(mergedConfig, configsToMerge, configsCount);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(mergedConfig);
            check_expected_ptr(configsToMerge);
            check_expected(configsCount);
            if (mergedConfig != NULL) {
                *mergedConfig = mock_ptr_type(samconfConfig_t *);
            }
            result = mock_type(samconfConfigStatusE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(samconfConfigMergeConfigs)(mergedConfig, configsToMerge, configsCount);
            break;
    }
    return result;
}

MOCK_FUNC_BODY(samconfCreateIntAt, samconfConfigStatusE_t, samconfConfig_t **root, const char *path, int64_t value) {
    if (MOCK_IS_ACTIVE(samconfCreateIntAt)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected(value);

        if (root != NULL) {
            *root = mock_ptr_type(samconfConfig_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }

    return MOCK_FUNC_REAL(samconfCreateIntAt)(root, path, value);
}

MOCK_FUNC_BODY(samconfCreateRealAt, samconfConfigStatusE_t, samconfConfig_t **root, const char *path, double value) {
    if (MOCK_IS_ACTIVE(samconfCreateRealAt)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected(value);

        if (root != NULL) {
            *root = mock_ptr_type(samconfConfig_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }

    return MOCK_FUNC_REAL(samconfCreateRealAt)(root, path, value);
}

MOCK_FUNC_BODY(samconfCreateBoolAt, samconfConfigStatusE_t, samconfConfig_t **root, const char *path, bool value) {
    if (MOCK_IS_ACTIVE(samconfCreateBoolAt)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected(value);

        if (root != NULL) {
            *root = mock_ptr_type(samconfConfig_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }

    return MOCK_FUNC_REAL(samconfCreateBoolAt)(root, path, value);
}

MOCK_FUNC_BODY(samconfCreateFromStringAt, samconfConfigStatusE_t, samconfConfig_t **root, const char *path,
               char *value) {
    if (MOCK_IS_ACTIVE(samconfCreateFromStringAt)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(value);

        if (root != NULL) {
            *root = mock_ptr_type(samconfConfig_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }

    return MOCK_FUNC_REAL(samconfCreateFromStringAt)(root, path, value);
}

MOCK_FUNC_BODY(samconfCreateStringAt, samconfConfigStatusE_t, samconfConfig_t **root, const char *path,
               const char *value) {
    if (MOCK_IS_ACTIVE(samconfCreateStringAt)) {
        check_expected_ptr(root);
        check_expected_ptr(path);
        check_expected_ptr(value);

        if (root != NULL) {
            *root = mock_ptr_type(samconfConfig_t *);
        }
        return mock_type(samconfConfigStatusE_t);
    }

    return MOCK_FUNC_REAL(samconfCreateStringAt)(root, path, value);
}

MOCK_FUNC_BODY(samconfCopyConfigValue, samconfConfigStatusE_t, const samconfConfig_t *from, samconfConfig_t *to) {
    if (MOCK_IS_ACTIVE(samconfCopyConfigValue)) {
        check_expected_ptr(from);
        check_expected_ptr(to);

        return mock_type(samconfConfigStatusE_t);
    }

    return MOCK_FUNC_REAL(samconfCopyConfigValue)(from, to);
}

MOCK_FUNC_BODY(samconfConfigCopyConfig, samconfConfigStatusE_t, const samconfConfig_t *from, samconfConfig_t *to) {
    if (MOCK_IS_ACTIVE(samconfConfigCopyConfig)) {
        check_expected_ptr(from);
        check_expected_ptr(to);

        return mock_type(samconfConfigStatusE_t);
    }

    return MOCK_FUNC_REAL(samconfConfigCopyConfig)(from, to);
}
