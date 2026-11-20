# adas-bsp-lab

Embedded Linux BSP lab for an AArch64 ADAS/autonomous-driving compute board, runnable on a laptop with QEMU. Virtual board only — no real silicon.

## Design

```
                 +---------------------- QEMU aarch64 virt ----------------------+
                 |  U-Boot (env) -> Linux Image + adas-virt.dtb                  |
                 |                                                                |
  sensors (mock) |  I2C IMU --(iio)--> drivers/imu-i2c/adas_imu.c                |
                 |  SPI CAN-FD -(SocketCAN)-> drivers/canfd-spi/adas_canfd.c     |
                 |  GPIO cam trigger -(IRQ/sysfs)-> drivers/cam-trigger/         |
                 |                                                                |
                 |  gateway (C++17, SocketCAN/vcan) <-> signals.dbc decoding     |
                 |  timesync (gPTP-style UDP) -> camera/radar/IMU timestamps     |
                 +------------------------------+-------------------------------+
                                                |
  host: Yocto meta-adas-board | bootlog/analyze.py | pytest + C++ tests | run-qemu.sh
```

| Module | Path | What it does | Status |
|---|---|---|---|
| Yocto layer | `meta-adas-board/` | machine conf, image, kernel/u-boot bbappends | recipes only, not built here |
| Device tree + U-Boot | `board/dts/adas-virt.dts`, `board/u-boot/` | I2C IMU, SPI CAN-FD, GPIO triggers, Ethernet MAC | DTS not compiled (no dtc in env) |
| Kernel drivers | `drivers/` | IIO IMU, SocketCAN CAN-FD skeleton (mock loopback), GPIO trigger IRQ+sysfs | need Linux headers to build |
| CAN-FD gateway | `gateway/` | routing table, rate limiting, DBC-like signal decode | tested on host |
| Time sync | `timesync/timesync.py` | simplified 802.1AS offset/delay, frame timestamps, skew | simulated, tested |
| Boot analysis | `bootlog/` | time to "camera pipeline ready", before/after logs | tested |
| QEMU | `scripts/run-qemu.sh` | boots kernel+DTB when images exist, else skips | skips cleanly here |

## Run / test / bench

```sh
python3 -m pytest tests -q          # 5 passed
make -C gateway test                # C++ unit tests
make -C gateway gateway && printf '256 8 100 0 16 39 0 0 0 0\n' | ./gateway/gateway --simulate
python3 timesync/timesync.py --simulate --frames 20
python3 bootlog/analyze.py bootlog/before.log
python3 bootlog/analyze.py bootlog/after.log
./scripts/run-qemu.sh               # SKIP if no qemu/images
./scripts/test-in-qemu.sh           # SKIP if no qemu
# drivers (Linux only): make -C drivers/imu-i2c KDIR=/lib/modules/$(uname -r)/build
# vcan (Linux only): sudo ip link add dev vcan0 type vcan && sudo ip link set vcan0 up
```

Sanity runs (this machine, macOS, scaled-down simulation — not hardware):

```
$ python3 -m pytest tests -q
5 passed in 0.82s
$ make -C gateway test
test_gateway: PASS
$ python3 timesync/timesync.py --simulate --frames 20
samples=20 estimated_offset_ms=5.012 true_offset_ms=5.000 skew_ms=0.317
sensor_skew_ms=2.000
$ python3 bootlog/analyze.py bootlog/before.log   # 5.480s
$ python3 bootlog/analyze.py bootlog/after.log    # 1.150s (parallel init, deferred modules, trimmed DT)
$ ./scripts/run-qemu.sh
SKIP: qemu-system-aarch64 not installed
```

## Limitations (honest)

- Virtual board only: QEMU `virt`, mock transports (IMU returns synthesized values, CAN-FD loops TX→RX), no real silicon, PHY, or timing.
- Kernel modules and Yocto image are not built in this environment (no kernel headers / bitbake / dtc / QEMU); they build/skip as documented.
- Time sync is a simplified 802.1AS estimator over simulated timestamps, not a full gPTP daemon; gateway SocketCAN loop is Linux-only, host testing uses `--simulate` and vcan conventions.
- Boot logs are example logs, not measured from this board.

## License

MIT — see `LICENSE`.
