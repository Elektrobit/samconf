#!/usr/bin/python3

import os
import json
import argparse
import sys
from faker import Faker

from config_provider import ConfigProvider
from faker.providers import file


def main():
    fake = Faker()
    fake.add_provider(ConfigProvider)
    fake.add_provider(file)
    count = 0
    parser = argparse.ArgumentParser(
        prog='Config Generator',
        description='Random config generator for elos',
        epilog="if no options are set, random config will be added in /tmp",
        add_help=True)

    parser.add_argument('-n', help='number of configs to be genrated')
    parser.add_argument(
        '-l',
        nargs='+',
        help='list of locations where configs are to be stored')

    args = parser.parse_args()

    if args.n:
        count = int(args.n)
    else:
        count = 1

    if args.l:
        conf_dir = args.l
    else:
        conf_dir = ["/tmp"]

    if (len(conf_dir) == 1) and conf_dir[0]:
        for i in range(count):
            config_filename = conf_dir[0] + "/" + str(
                fake.random_int(min=1, max=256)) + "_" + fake.word() + ".json"
            config = json.dumps(fake.create_config(), indent=4)
            with open(config_filename, "w") as config_file:
                config_file.write(config)
            print(config_filename)
    else:
        for j in range(count):
            config_filename = fake.random_element(
                elements=conf_dir) + "/" + str(fake.random_int(
                    min=1, max=110)) + "_" + fake.word() + ".json"
            config = json.dumps(fake.create_config(), indent=4)
            with open(config_filename, "w") as config_file:
                config_file.write(config)
            print(config_filename)


if __name__ == "__main__":
    main()
