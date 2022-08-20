# Wi-Fi Driver Real-time Hack
> Target for Linux Kernel v5.15 LTS.

use procfs + mmap to access ieee80211_ops within ***extremely low latency*** from userspace.

<!--
### Brief Introduction
The common tools used to communicate with wireless NIC driver are `iwconfig` and `iw`.
* `iwconfig` is based on `ioctl` which is time-consuming and deprecated.
* `iw` is based on `nl80211` interface implemented via system socket, and its latency would be extremely large when the system is under heavy burden.

So, this project is aiming at providing very small and reliable latency (<<1ms) control on `ieee80211_ops` in `mac80211` module. This toolkit is consisted of *kernel-space* hack module and *user-space* program, which are connected via ***direct memory access*** mounted on `procfs`.
The memory r/w is currently implemented in synchronized block writing/reading (to be lockless ring-buffer impl).


As a demo, this project currently only provides access to `conf_tx` function in `mac80211` kernel module, which could alter the wireless channel access priority.s
-->

### Build
```bash
mkdir build
cd build && cmake ..
make
```

> We use a custom `iwlmvm` build together with the repo. You can disable it with `cmake -DBUILD_IWLMVM=OFF ..`

### How to Use
> Make sure that you have at least one wireless NIC enabled;

1. Run `sudo insmod wlsops/wlsops_hack.ko` to install the built kernel modules, where `wlsops_hack` will use the first wireless NIC registered in the system;

2. Run `sudo ./wlsctrl/wlsctrl.py` to apply the actions.
> You can use `chmod u+s ./wlsctrl/wlsctrl.py` to avoid tedious usage of `sudo`.
