# samconf

It stands for **s**igned and **m**ergable **conf**iguration. It is used by
[elos](https://github.com/Elektrobit/elos). The goal of the library is to
manage program configuration options from various sources including support for
verification of the configuration sources.
There is also a online documentation available at https://elektrobit.github.io/samconf/.

Dependencies:

* [cmocka_extensions](https://github.com/Elektrobit/cmocka_extensions)
* [cmocka_mocks](https://github.com/Elektrobit/cmocka_mocks)
* [safu](https://github.com/Elektrobit/safu).

## Building

samconf uses CMake for building:

```bash
cmake -B ./build ./samconf
make -C ./build all
make -C ./build install
```

or use the CI hooks

```bash
./samconf/ci/build.sh [Release]
```

### Workspace setup

1. Please run the following git commands after cloning the repository:

   ```bash
   git config core.hooksPath .githooks
   ```

   to set the commit message template and to setup the project specific git hooks.

2. Usually it is a good idea to create a symlink to the compilation database
   created by cmake in the build directory. Run therefore:

   ```
   ln -s build/compile_commands.json
   ```

## Folders

### ci

Scripts to be used by CI pipelines to build , run various tests and checks.

### documentation

More detailed documentation on the event logging and management system.

### debian.native

Contains Debian packaging scripts for building the current source tree as .deb
packages. This is primarily for CI pipelines to automatically verify if the
packages are buildable or if anything needs to be changed.

See `debian.native/README.md` for more information.

### src

Contains the actual productive sources.

#### src/samconf

Contains code.

### test

Contains all test related sources. See the documentation for details.

# License

MIT License

Copyright (c) [2023] [emlix GmbH, Elektrobit Automotive GmbH]

The full text of the license can be found in the [LICENSE](LICENSE) file in the repository root directory.

## Powered by EB

<img src="doc/source/_static/eb-logo.png" width=70 height=70 align="left">
elos is powered by elektrobit automotive gmbh.
Elektrobit is an automotive software company and developer of embedded software products for ECU, AUTOSAR, automated driving, connected vehicles and UX.
elos is an  integrated part of EB corbos Linux – built on Ubuntu is an open-source operating system for high-performance computing, leveraging the rich functionality of Linux while meeting security and industry regulations.


## Maintainers

* Wolfgang Gehrhardt wolfgang.gehrhardt@emlix.com [@gehwolf](https://github.com/gehwolf)
* Thomas Brinker thomas.brinker@emlix.com [@ThomasBrinker](https://github.com/ThomasBrinker)

## Credits

* Andreas Schickedanz
* Andreas Zdziarstek
* Anja Lehwess-Litzmann
* Annika Schmitt
* Anton Hillerband
* Benedikt Braunger
* Christian Steiger
* Daniel Glöckner
* Fabian Godehardt
* Friedrich Schwedler
* Isaac True
* Joerg Vehlow
* Maryniuk Bogdan
* Rainer Müller
* Sabrina Otto
* Thomas Brinker
* Vignesh Jayaraman
* Wolfgang Gehrhardt
