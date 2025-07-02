from random import choice, randint, sample
from collections import OrderedDict
from faker import Faker
from faker.providers import BaseProvider

from config_data import ConfigDataProvider

class ConfigProvider(BaseProvider):

    def __init__(self, generator):
        super().__init__(generator)
        self.data = ConfigDataProvider()

    def empty_config(self) -> dict:
        return {}

    def local_tcp_plugin(self) -> dict:
        local_tcp_plugin = {"LocalTcpClient": {}}
        config = {}
        config.update({"ConnectionLimit": self.data.connection_limit()})
        config.update({"Port": self.data.port_val()})
        config.update({"Interface": self.data.private_interface()})
        config.update({"EventBlackLiist": self.data.event_filter()})
        config.update({"authorizedProcesses": self.data.process_filter(2, 3)})
        local_tcp_plugin["LocalTcpClient"].update({"File": self.data.client_files(ext='so')})
        local_tcp_plugin["LocalTcpClient"].update({"Run": self.data.run_val()})
        local_tcp_plugin["LocalTcpClient"].update({"Config": config})

        return local_tcp_plugin

    def public_tcp_plugin(self) -> dict:
        public_tcp_plugin = {"PublicTcpClient": {}}
        config = {}
        config.update({"ConnectionLimit": self.data.connection_limit()})
        config.update({"Port": self.data.port_val()})
        config.update({"Interface": self.data.public_interface()})
        config.update({"EventBlackLiist": self.data.event_filter()})
        config.update({"authorizedProcesses": self.data.process_filter(2, 3)})
        public_tcp_plugin["PublicTcpClient"].update(
            {"File": self.data.client_files(ext='so')})
        public_tcp_plugin["PublicTcpClient"].update({"Run": self.data.run_val()})
        public_tcp_plugin["PublicTcpClient"].update({"Config": config})

        return public_tcp_plugin

    def unix_plugin(self) -> dict:
        unix_plugin = {"unixClient": {}}
        config = {}
        config.update({"ConnectionLimit": self.data.connection_limit()})
        config.update({"path": self.data.path_to_files(dpth=2, ext='socket')})
        config.update({"EventBlackLiist": self.data.event_filter()})
        config.update({"authorizedProcesses": self.data.process_filter(2, 3)})
        unix_plugin["unixClient"].update({"File": self.data.client_files(ext='so')})
        unix_plugin["unixClient"].update({"Run": self.data.run_val()})
        unix_plugin["unixClient"].update({"Config": config})

        return unix_plugin

    def fetchapi_plugin(self) -> dict:
        fetchapi_plugin = {"fetchapi": {}}
        config = {}
        config.update({"BufferSize": self.random_int(min=10, max=150)})
        fetchapi_plugin["fetchapi"].update({"File": self.data.client_files(ext='so')})
        fetchapi_plugin["fetchapi"].update({"Run": self.data.run_val()})
        fetchapi_plugin["fetchapi"].update({"Filter": [self.data.event_filter()]})
        fetchapi_plugin["fetchapi"].update({"Config": config})

        return fetchapi_plugin

    def json_plugin(self) -> dict:
        json_plugin = {"JsonBackend": {}}
        config = {}
        config.update({"StoragePath": self.data.path_to_files(dpth=1, ext='log')})
        config.update({"MaxSize": self.random_int(min=10, max=60000)})
        config.update({"Flags": ["O_SYNC"]})
        json_plugin["JsonBackend"].update({"File": self.data.client_files(ext='so')})
        json_plugin["JsonBackend"].update({"Run": self.data.run_val()})
        json_plugin["JsonBackend"].update({"Filter": [self.data.event_filter()]})
        json_plugin["JsonBackend"].update({"Config": config})

        return json_plugin

    def sql_plugin(self) -> dict:
        sql_plugin = {"SQLBackend": {}}
        config = {}
        config.update({"ConnectionPath": self.data.path_to_files(dpth=1, ext='sqlite')})
        sql_plugin["SQLBackend"].update({"File": self.data.client_files(ext='so')})
        sql_plugin["SQLBackend"].update({"Run": self.data.run_val()})
        sql_plugin["SQLBackend"].update({"Filter": [self.data.event_filter()]})
        sql_plugin["SQLBackend"].update({"Config": config})

        return sql_plugin

    def dlt_plugin(self) -> dict:
        dlt_plugin = {"DLT": {}}
        config = {}
        config.update({"Connection": "/tmp/dlt"})
        config.update({"EcuId": "ECU1"})
        config.update({"AppId": "ELOS"})
        dlt_plugin["DLT"].update({"File": self.data.client_files(ext='so')})
        dlt_plugin["DLT"].update({"Run": self.data.run_val()})
        dlt_plugin["DLT"].update({"Filter": [self.data.event_filter()]})
        dlt_plugin["DLT"].update({"Config": config})

        return dlt_plugin

    def oomkiller_plugin(self) -> dict:
        oomkiller_plugin = {"OomKiller": {}}
        oomkiller_plugin["OomKiller"].update({"File": self.data.client_files(ext='so')})
        oomkiller_plugin["OomKiller"].update({"Run": self.data.run_val()})

        return oomkiller_plugin

    def syslog_plugin(self) -> dict:
        syslog_plugin = {"SyslogScanner": {}}
        mapping_rules = {"MappingRules": {}}
        message_codes = {"MessageCodes": {}}
        message_codes["MessageCodes"].update({
            str(self.random_element(elements=self.data.messageCodes)):
            self.data.event_filter(3)
        })
        mapping_rules["MappingRules"].update(message_codes)
        config = {}
        config.update({"SyslogPath": "/dev/log"})
        config.update(mapping_rules)
        syslog_plugin["SyslogScanner"].update({"File": self.data.client_files(ext='so')})
        syslog_plugin["SyslogScanner"].update({"Run": self.data.run_val()})
        syslog_plugin["SyslogScanner"].update({"Config": config})

        return syslog_plugin

    def kmsg_plugin(self) -> dict:
        kmsg_plugin = {"KmsgScanner": {}}
        config = {}
        config.update({"KmsgFile": "/dev/kmsg"})
        kmsg_plugin["KmsgScanner"].update({"File": self.data.client_files(ext='so')})
        kmsg_plugin["KmsgScanner"].update({"Run": self.data.run_val()})
        kmsg_plugin["KmsgScanner"].update({"Config": config})

        return kmsg_plugin

    def shmem_plugin(self) -> dict:
        shmem_plugin = {"Shmem": {}}
        config = {}
        config.update({"ShmemFile": "scanner_shmem"})
        config.update({"ShmemCreate": self.data.use_env()})
        config.update({"ShmemLogEntries": self.random_int(min=1, max=256)})
        config.update({"ShmemOffset": self.random_int(min=0, max=100)})
        config.update({"SemFile": "scanner_shmem_sem"})
        config.update({"SemCreate": self.data.use_env()})
        shmem_plugin["Shmem"].update({"File": self.data.client_files(ext='so')})
        shmem_plugin["Shmem"].update({"Run": self.data.run_val()})
        shmem_plugin["Shmem"].update({"Config": config})

        return shmem_plugin

    def client_input_node(self) -> dict:
        client_inputs = {"ClientInputs": {}}
        plugins = {"Plugins": {}}
        plugin_members = self.random_elements(
            elements=(self.local_tcp_plugin(), self.public_tcp_plugin(),
                      self.unix_plugin()))
        for member in plugin_members:
            plugins["Plugins"].update(member)
        client_inputs["ClientInputs"].update(plugins)
        return client_inputs

    def event_logging_node(self) -> dict:
        event_logging = {"EventLogging": {}}
        plugins = {"Plugins": {}}
        plugin_members = self.random_elements(elements=(self.fetchapi_plugin(),
                                                        self.json_plugin(),
                                                        self.sql_plugin(),
                                                        self.dlt_plugin()))
        for member in plugin_members:
            plugins["Plugins"].update(member)
        event_logging["EventLogging"].update(plugins)
        return event_logging

    def scanner_node(self) -> dict:
        scanner = {"Scanner": {}}
        plugins = {"Plugins": {}}
        plugin_members = self.random_elements(
            elements=(self.oomkiller_plugin(), self.syslog_plugin(),
                      self.kmsg_plugin(), self.shmem_plugin()))
        for member in plugin_members:
            plugins["Plugins"].update(member)
        scanner["Scanner"].update(plugins)
        return scanner

    def elos_node(self) -> dict:
        elos = {"elos": {}}
        elos["elos"].update({"UseEnv": self.data.use_env()})
        elos["elos"].update({"LogFilter": self.data.log_filter()})
        elos["elos"].update({"LogLevel": self.data.log_level()})
        return elos

    def root_node(self) -> dict:
        root = {"root": {}}
        return root

    def create_config(self) -> dict:
        root = self.root_node()
        elos = self.elos_node()

        elos_modules = self.random_elements(
            elements=(self.client_input_node(), self.event_logging_node(),
                      self.scanner_node()))
        for module in elos_modules:
            elos["elos"].update(module)

        root["root"] = elos

        return root
