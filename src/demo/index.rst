Demos
=====

Samconf comes with a set of demo applications to show certain aspects of samconf.

samprobe – test and query config files
---------------------------------------

The tool was primarily developed to test the samconf library. It is also useful
to check and query config files.

To verify a config file :

.. command-output:: samprobe test/smoketest/config.json


env_example – test and query configs created from environment variables
-----------------------------------------------------------------------

This is an example application to check configurations created from environment
variables. In this example some environment variables are set with a prefix **SAMCONF**.
A string is passed with `env://SAMCONF` as location for the `samconfLoad` function. The
`env://` indicates that the config shall be loaded from environment variables and not a file. The
prefix .i.e **SAMCONF** indicates that not all environment variables are to be loaded
into a configuration but only those with the given prefix are to be loaded.

To use this demo :

.. command-output:: env_example


dump_merged_config - merge multiple configs and dump the result
---------------------------------------------------------------

This is an example application to dump the result of merging multiple configurations.
The result can be dumped as JSON, a tree view or both.

To use this demo:

.. command-output:: dump_merged_config --help

.. command-output:: dump_merged_config --tree --json test/smoketest/

dump_config - load a single config and dump it to stdout
--------------------------------------------------------

This is an example application to load and dump a configurations.
The output can be as JSON, a tree view or both.

To use this demo:

.. command-output:: dump_config --help

.. command-output:: dump_config --tree --json test/smoketest/config.json
