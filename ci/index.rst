
.. default-role:: code

=================================
Different options on how to build
=================================

- `ci/build.sh [Debug|Release]`
- `cmake && make|ninja`

Build Configurations
====================

Make sure to install all dependencies

`cmocka_extensions`, `cmocka_mocks` & `safu`

`cmake & make` without any options (default)
--------------------------------------------

.. code-block::

   cmake -B build/ .
   make -C build/

Builds everything.
Everything is build in Debug mode.

.. code-block::

   make -C build/ install

Can be configured with the options described in section :ref:`cmake-options`.

Alternatively use the CI hooks for developer setup

./ci/install_deps.py
./ci/build.sh [Release|Debug]

`ci/install_deps.py`
--------------------

Intended to easier install the dependencies `cmocka_extensions`, `cmocka_mocks` & `safu`

.. code-block::

   ci/install_deps.py

By default it installs from the main branch of the GitHub repositories.
The behavior can be modified by specifying one of the following files in this order:

* file specified by `SAMCONF_DEPENDENCY_CONFIG` environment variable

.. code-block::

   export SAMCONF_DEPENDENCY_CONFIG=".myconfig/my_dependencies.json"

* `dependencies.json` in the root of the project

* `ci/dependencies_default.json` in the root of the project

Each resource can be defined by the `[url|path][commit|branch|tag][cmake_opts]`
options. This could look like:

.. code-block:: json

   {
      "safu": {
         "path": "../safu"
      },
      "cmocka_mocks": {
         "url": "https://github.com/Elektrobit/cmocka_mocks.git",
         "commit": "2206cfbcf315b3c7381f9b9f4aa29238b18c1a6b",
         "cmake_opts": ["-D", "UNIT_TESTS=off"]
      },
      "cmocka_extensions": {
         "tag": "cmocka_extensions-0.50.0"
      }
   }

If no URL or path is provided the default is used.
A path is used as is without checking the branch, tag or commit and includes
local changes that aren't committed jet

The `url` property supports the evaluation of environment variables like
`SOURCES_URI` so it is possible to define template configurations like:

.. code-block:: json

   {
      "safu": {
           "url": "${SOURCES_URI}/safu.git",
           "branch": "integration"
       },
       "cmocka_mocks": {
           "url": "${SOURCES_URI}/cmocka_mocks.git",
           "branch": "integration"
       },
       "cmocka_extensions": {
           "url": "${SOURCES_URI}/cmocka_extensions.git",
           "branch": "integration"
       }
   }

and call `install_deps.py` like :

.. code-block:: bash

   SOURCES_URI=https://${GIT_USER_TOKEN}@github.com/Elektrobit/

These dependencies get installed into `build/deps/` and if provided with an URL the sources can be found in `build/deps/src`.
To use binaries installed from dependencies add `build/deps/bin` to the `PATH` environment variable.

`ci/build.sh`
-------------

Intended for easier building of different configurations while developing and in CI.

.. code-block::

   ci/build.sh [BUILD_TARGET]

Builds everything.
By default the BUILD_TARGET is Debug.
The build folder is set to `build/BUILD_TARGET/cmake` and everything is also directly installed into `build/BUILD_TARGET/dist/`.

Additional configurations are described in :ref:`buildsh-options`.

Output
======

.. code-block::

   usr/
   └── local/
      ├── bin/
      │  ├── samprobe
      │  └── signature.sh
      ├── include/
      │  └── samconf/
      │     ├── config_backend.h
      │     ├── crypto_utils.h
      │     ├── json_backend.h
      │     ├── mock_samconf.h
      │     ├── samconf.h
      │     ├── samconf_types.h
      │     ├── signature.h
      │     └── uri.h
      └── lib/
         ├── cmake/
         │  └── samconf/
         │     ├── mock_samconfConfig.cmake
         │     ├── mock_samconfConfigVersion.cmake
         │     ├── samconfConfig.cmake
         │     ├── samconfConfigVersion.cmake
         │     ├── samconfTargets-release.cmake
         │     └── samconfTargets.cmake
         ├── libmock_samconf.so -> libmock_samconf.so.0
         ├── libmock_samconf.so.0 -> libmock_samconf.so.0.52.0
         ├── libmock_samconf.so.0.52.0
         ├── libsamconf.so -> libsamconf.so.0
         ├── libsamconf.so.0 -> libsamconf.so.0.52.0
         ├── libsamconf.so.0.52.0
         ├── pkgconfig/
         │  └── samconf.pc
         └── test/
            └── samconf unit tests


Options
=======

.. _cmake-options:

cmake options
-------------
- CMAKE_BUILD_TYPE: (default "Debug")
- ENABLE_CI: (default OFF) adds `-Werror` to the compile options
- UNIT_TESTS: (default ON) Build the unit tests
- INSTALL_UNIT_TESTS: (default ON) install the unit tests

.. _buildsh-options:

ci/build.sh options
-------------------
- [BUILD_TYPE] (default "Debug") sets the build type for cmake
- `--ci` enables `-DENABLE_CI` for cmake and implicitly sets `--clean --verbose`
- `--clean|-c` deletes the build directory before the build for a fresh start
- `--verbose|-v` adds `-v` to the parameters for ninja
