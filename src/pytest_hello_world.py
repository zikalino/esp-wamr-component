# SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: CC0-1.0

from typing import Callable

import pytest
from pytest_embedded_idf.dut import IdfDut
from pytest_embedded_qemu.dut import QemuDut

@pytest.mark.esp32
@pytest.mark.host_test
@pytest.mark.qemu
def test_hello_world_host(dut: QemuDut) -> None:
    dut.expect('Hello world!')
