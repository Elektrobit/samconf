Benchmark Tests
===============

Description:

The benchmark test provides data points to help in the checking of application
performance and establishes whether changes to code base, improved or deteriorated
the performance.
 
The CI system is responsible of running the benchmark tests regularly.

Details:

- Can run against any build type
- Executed by CI-pipeline
- Written in pure shell to run on targets with very limited environment

To run the benchmark test from within the source (Developer):
-------------------------------------------------------------

First ensure the build is present that shall be tested. `Debug` is assumed as
default.

.. code:: shell

   ./ci/build.sh [Release]

.. program-output:: ./ci/run_benchmark.sh -h
   :shell:

Run on a Target (Integrator):
-----------------------------

To run the benchmark test install all content in ``tests/smoketest`` to a
custom location on the target system. Currently it is not packaged and
installed by elos build system itself.

Prerequisites :
~~~~~~~~~~~~~~~

-  Elos is installed on target system

-  Elos benchmark test package is installed

-  At least a writeable directory to store results and for intermediate
   files

Running the Test Suite:
~~~~~~~~~~~~~~~~~~~~~~~

To execute the benchmark tests run the installed bench_test.sh script. Here is an
example script to for benchmark test integration.

.. code:: shell

    #!/bin/sh -eu

    export PREFIX_PATH=/usr/local/
    export BENCHMARK_RESULT_DIR=/tmp/test-elos-benchmark
    /usr/local/lib/test/elos/benchmark/bench_test.sh

Extended configuration
~~~~~~~~~~~~~~~~~~~~~~

Depending on the installation and configuration it could be necessary to
adjust some settings. The following Environment variables can be used:

.. program-output:: cat ./test/benchmark/bench_test_env.sh
   :shell:
