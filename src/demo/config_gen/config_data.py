from enum import Enum
from collections import OrderedDict
from random import choice, randint, sample

from faker import Faker

from faker.providers import python
from faker.providers import file
from faker.providers import internet


class ConfigDataProvider(object):

    def __init__(self):
        self.fake = Faker()
        self.fake.add_provider(python)
        self.fake.add_provider(file)
        self.fake.add_provider(internet)

        self.messageCodes = (0, 200, 202, 400, 401, 501, 1000, 1101, 1102,
                             1103, 1111, 2000, 2001, 2002, 2003, 2004, 2005,
                             2006, 2007, 2008, 3000, 3001, 3003, 3004, 3005,
                             3006, 3023, 3422, 4000, 4001, 4002, 4005, 4006,
                             5000, 5001, 5002, 5003, 5004, 5005, 5006, 5020,
                             5100, 5101, 5102, 6000, 6001, 6002, 6003, 6004,
                             6005, 7000, 7001, 7002, 7003, 7004, 7005, 7125,
                             7126, 7127, 7200, 7201, 7202, 7203, 7204, 7205,
                             8000, 8001, 8002, 8003, 8004, 8005, 8006, 8007)

        self.classification = (0x00000000FFFFFFFF, 0x000000FF00000000,
                               0xFFFFFF0000000000, 0x0000000000000000,
                               0x0000000000000001, 0x0000000000000002,
                               0x0000000000000004, 0x0000000000000008,
                               0x0000000000000010, 0x0000000000000020,
                               0x0000000000000040, 0x0000000000000080,
                               0x0000000000000100, 0x0000000000000200,
                               0x0000000100000000, 0x0000000200000000,
                               0x0000000400000000, 0x0000000800000000,
                               0x0000001000000000, 0x0000002000000000,
                               0x0000004000000000, 0x0000008000000000)

        self.log_levels = ("NONE", "CRITICAL", "ERROR", "WARN", "INFO",
                           "DEBUG", "COUNT", "ALL", "INVALID")

        self.filter_event_idf = (".event.", ".ev.", ".e.")
        self.filter_event_member_idf = ("date.tv_sec", "date.tv_nsec",
                                        "source.pid", "source.appname",
                                        "source.filename", "severity",
                                        "hardwareid", "classification",
                                        "messageCode", "payload")

        self.filter_process_idf = (".process.", ".proc.", ".p.")
        self.filter_process_member_idf = ("uid", "gid", "pid", "exec")

        self.filter_operand_operator = ("EQ", "LT", "LE", "GT", "GE", "STRCMP")
        self.filter_term_operator = ("AND", "OR")

    def use_env(self):
        return self.fake.pybool()

    def log_filter(self):
        filenames = self.fake.file_name(extension="c")
        return self.fake.random_element(
            elements=OrderedDict([("", 0.9), (filenames, 0.1)]))

    def log_level(self):
        return self.fake.random_element(elements=self.log_levels)

    def run_val(self):
        return self.fake.random_element(
            elements=OrderedDict([("Always", 0.9), ("Never", 0.1)]))

    def port_val(self):
        return self.fake.port_number()

    def private_interface(self):
        return self.fake.ipv4_private()

    def public_interface(self):
        return self.fake.ipv4_public()

    def connection_limit(self):
        return self.fake.random_int(min=0, max=256)

    def client_files(self, ext=''):
        return self.fake.file_name(extension=ext)

    def path_to_files(self, dpth=0, ext=''):
        return self.fake.file_path(depth=dpth, extension=ext, absolute=True)

    def event_filter(self, term_count=1, filter_count=1):
        event = {
            "date.tv_sec":
            self.fake.random_number(digits=10),
            "date.tv_nsec":
            self.fake.random_number(digits=10),
            "source.pid":
            self.fake.random_int(min=0, max=4194304),
            "source.appname":
            self.fake.file_name(),
            "source.filename":
            "",
            "severity":
            self.fake.random_int(min=0, max=6),
            "hardwareid":
            self.fake.hexify(text="^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"),
            "classification":
            hex(self.fake.random_element(elements=self.classification)),
            "messageCode":
            self.fake.random_element(elements=self.messageCodes),
            "payload":
            self.fake.sentence(nb_words=6)
        }
        event["source.filename"] = self.fake.file_path(
        ) + event["source.appname"]
        event_member = self.fake.random_element(
            elements=self.filter_event_member_idf)

        term = self.fake.random_element(
            elements=self.filter_event_idf) + event_member + " " + str(
                event[event_member]) + " " + self.fake.random_element(
                    elements=self.filter_operand_operator) + " "

        if (term_count == 1) and (filter_count == 1):
            return term
        elif (term_count > 1) and (filter_count == 1):
            for i in range(term_count):
                newterm = self.fake.random_element(
                    elements=self.filter_event_idf) + event_member + " " + str(
                        event[event_member]) + " " + self.fake.random_element(
                            elements=self.filter_operand_operator) + " "
                term_op = self.fake.random_element(
                    elements=self.filter_term_operator) + " "
                term = term + term_op + newterm
            return term
        elif (term_count == 1) and (filter_count > 1):
            filter_list = [term]
            for i in range(filter_count):
                newterm = self.fake.random_element(
                    elements=self.filter_event_idf) + event_member + " " + str(
                        event[event_member]) + " " + self.fake.random_element(
                            elements=self.filter_operand_operator) + " "
                filter_list.append(newterm)
            return filter_list
        else:
            filter_list = []
            for i in range(filter_count):
                for j in range(term_count):
                    newterm = self.fake.random_element(
                        elements=self.filter_event_idf
                    ) + event_member + " " + str(
                        event[event_member]) + " " + self.fake.random_element(
                            elements=self.filter_operand_operator) + " "
                    term_op = self.fake.random_element(
                        elements=self.filter_term_operator) + " "
                    term = term + term_op + newterm
                filter_list.append(term)
            return filter_list

    def process_filter(self, term_count=1, filter_count=1):
        process = {
            "pid": self.fake.random_int(min=0, max=4194304),
            "gid": self.fake.random_int(min=1000, max=60000),
            "uid": self.fake.random_int(min=1000, max=60000),
            "exec": self.fake.file_path(depth=5, extension='', absolute=True),
        }
        process_member = self.fake.random_element(
            elements=self.filter_process_member_idf)

        term = self.fake.random_element(
            elements=self.filter_process_idf) + process_member + " " + str(
                process[process_member]) + " " + self.fake.random_element(
                    elements=self.filter_operand_operator) + " "

        if (term_count == 1) and (filter_count == 1):
            return term

        elif (term_count > 1) and (filter_count == 1):
            for i in range(term_count):
                newterm = self.fake.random_element(
                    elements=self.filter_process_idf
                ) + process_member + " " + str(
                    process[process_member]) + " " + self.fake.random_element(
                        elements=self.filter_operand_operator) + " "
                term_op = self.fake.random_element(
                    elements=self.filter_term_operator) + " "
                term = term + term_op + newterm
            return term

        elif (term_count == 1) and (filter_count > 1):
            filter_list = [term]
            for i in range(filter_count):
                newterm = self.fake.random_element(
                    elements=self.filter_process_idf
                ) + process_member + " " + str(
                    process[process_member]) + " " + self.fake.random_element(
                        elements=self.filter_operand_operator) + " "
                filter_list.append(newterm)
            return filter_list
        else:
            filter_list = []
            for i in range(filter_count):
                for j in range(term_count):
                    newterm = self.fake.random_element(
                        elements=self.filter_process_idf
                    ) + process_member + " " + str(
                        process[process_member]
                    ) + " " + self.fake.random_element(
                        elements=self.filter_operand_operator) + " "
                    term_op = self.fake.random_element(
                        elements=self.filter_term_operator) + " "
                    term = term + term_op + newterm
                filter_list.append(term)
            return filter_list
