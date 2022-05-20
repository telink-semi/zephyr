# Copyright (c) 2021, Telink Semiconductor
#
# SPDX-License-Identifier: Apache-2.0

import re
import os
import time
import subprocess
import platform

from runners.core import ZephyrBinaryRunner, RunnerCaps, BuildConfiguration

class SpiBurnBinaryRunner(ZephyrBinaryRunner):
    '''Runner front-end for SPI_burn.'''

    def __init__(self, cfg, addr, spiburn, iceman, timeout, gdb_port, gdb_ex, erase=False):
        super().__init__(cfg)

        self.spiburn = spiburn
        self.iceman = iceman
        self.addr = addr
        self.timeout = int(timeout)
        self.erase = bool(erase)
        self.gdb_port = gdb_port
        self.gdb_ex = gdb_ex

    @classmethod
    def name(cls):
        return 'spi_burn'

    @classmethod
    def capabilities(cls):
        return RunnerCaps(commands={'flash', 'debug'}, erase=True, flash_addr=True)

    @classmethod
    def do_add_parser(cls, parser):
        parser.add_argument('--addr', default='0x0',
                            help='start flash address to write')
        parser.add_argument('--timeout', default=10,
                            help='ICEman connection establishing timeout in seconds')
        parser.add_argument('--telink-tools-path', help='path to Telink flash tools')
        parser.add_argument('--gdb-port', default='1111', help='Port to connect for gdb-client')
        parser.add_argument('--gdb-ex', default='', nargs='?', help='Additional gdb commands to run')

    @classmethod
    def do_create(cls, cfg, args):

        if args.telink_tools_path:
            spiburn = f'{args.telink_tools_path}/flash/bin/SPI_burn'
            iceman = f'{args.telink_tools_path}/ice/ICEman'
        else:
            # If telink_tools_path arg is not specified then pass to tools shall be specified in PATH
            spiburn = 'SPI_burn'
            iceman  = 'ICEman'

        # Get flash address offset
        if args.dt_flash:
            build_conf = BuildConfiguration(cfg.build_dir)
            address = hex(cls.get_flash_address(args, build_conf) - build_conf['CONFIG_FLASH_BASE_ADDRESS'])
        else:
            address = args.addr

        return SpiBurnBinaryRunner(cfg, address, spiburn, iceman, args.timeout, args.gdb_port, args.gdb_ex, args.erase)

    def do_run(self, command, **kwargs):

        self.require(self.spiburn)

        # Find path to ICEman with require call
        self.iceman_path = self.require(self.iceman)

        if command == "flash":
            self._flash()

        elif command == "debug":
            self._debug()

        else:
            self.logger.error(f'{command} not supported!')

    def start_iceman(self):

        # ICEman tool from Andestech is required to be run from its root dir,
        # due to the tool looking for its config files in currently active dir.
        # In attempt to run it from other dirs it returns with an error that
        # required config file is missing
        origin_dir = os.getcwd()
        iceman_dir = os.path.dirname(self.iceman_path)

        # Go into ICEman dir
        os.chdir(iceman_dir)

        # Start ICEman as background process
        cmd_ice_run = ["./ICEman", '-Z', 'v5', '-l', 'aice_sdp.cfg']

        # Following part of code is necessary to ignore INT signal.
        # which is necessary for correct working of GDB.
        # The part is copied from popen_ignore_int. It is not possible
        # to use popen_ignore_int directly due to abscense of possibility
        # to pass stdout=subprocess.PIPE argument in Popen call
        cflags = 0
        preexec = None
        system = platform.system()

        if system == 'Windows':
            # We can't type check this line on Unix operating systems:
            # mypy thinks the subprocess module has no such attribute.
            cflags |= subprocess.CREATE_NEW_PROCESS_GROUP  # type: ignore
        elif system in {'Linux', 'Darwin'}:
            # We can't type check this on Windows for the same reason.
            preexec = os.setsid # type: ignore

        # Run ICEman
        self.ice_process = subprocess.Popen(cmd_ice_run, stdout=subprocess.PIPE,
                                                         creationflags=cflags,
                                                         preexec_fn=preexec)

        # Wait till it ready or exit by timeout
        start = time.time()
        while True:
            out = self.ice_process.stdout.readline()
            if b'ICEman is ready to use.' in out:
                break
            if time.time() - start > self.timeout:
                raise RuntimeError("TIMEOUT: ICEman is not ready")

        # Restore origin dir
        os.chdir(origin_dir)

    def stop_iceman(self):
        # Kill ICEman subprocess
        self.ice_process.terminate()

    def _flash(self):

        try:

            # Start ICEman
            self.start_iceman()

            # Compose flash command
            cmd_flash = [self.spiburn, '--addr', str(self.addr), '--image', self.cfg.bin_file]

            if self.erase:
                cmd_flash += ["--erase-all"]

            # Run SPI burn flash tool
            self.check_call(cmd_flash)

        finally:
            self.stop_iceman()

    def _debug(self):

        try:

            # Start ICEman
            self.start_iceman()

            # format -ex commands
            gdb_ex = re.split("(-ex) ", self.gdb_ex)[1::]

            # Compose gdb command
            client_cmd = [self.cfg.gdb, self.cfg.elf_file, '-ex', f'target remote :{self.gdb_port}'] + gdb_ex

            # Run gdb
            self.run_client(client_cmd)

        finally:
            self.stop_iceman()