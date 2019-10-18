# Wi-Fi Driver Real-time Hook
use debugfs/procfs + mmap to access ieee80211_ops within ***extremely small latency*** from userspace.


### Brief Introduction
The common tools used to communicate with wireless NIC driver are `iwconfig` and `iw`.
* `iwconfig` is based on `ioctl` which is time-consuming and deprecated.
* `iw` is based on `nl80211` interface implemented via system socket, and its latency would be extremely large when the system is under heavy burden.

So, this project is aiming at providing very small and reliable latency (<<1ms) control on `ieee80211_ops` in `mac80211` module. This toolkit is consisted of *kernel-space* hook module and *user-space* program, which are connected via ***direct memory access*** mounted on `procfs`.
The memory r/w is currently implemented in synchronized block writing/reading (to be lockless ring-buffer impl).


As a demo, this project currently only provides access to `conf_tx` function in `mac80211` kernel module, which could alter the wireless channel access priority.

### Installation
1. `make build-krn` to build kernel module `wlsops_hook.ko` (in './');
2. `make build-usr` to build userspace program `wlsctrl` (in './wlsctrl');
3. (Optional, privilege required) `make install` will put kernel module into default system folder, and userspace program into `/usr/bin`.

### How to Use
0. Make sure that you have at least one wireless NIC enabled;
1. First of all, `make insmod` or `modprobe wlsops_hook` (if you have run `make install` before) to install the kernel hook module, and it will hook onto the first wireless NIC registered in the system;
2. Run `sudo ./wlsctrl/wlsctrl`;
3. You will see the time with unit of second printed in `%.6f` format on the console, which is the time elapsed after 1000 times trying to set the fastest priority of the channel access (AIFS with 1 timeslot and NO contention window).

### What's Next
If you are inspired by this project, please feel free to FORK it!

If you have any problems, create the issue!