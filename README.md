# Wi-Fi Driver Realtime Hook
use debugfs/procfs + mmap to access ieee80211_ops within extremely small latency from userspace.


### Brief Introduction
The common tools to communicate with wireless NIC driver are `iwconfig` and `iw`.
* `iwconfig` is based on `ioctl` which is time-consuming and deprecated.
* `iw` is based on `nl80211` interface implemented via system socket, and its latency would be extremely large when the system is under heavy burden.

So, this project is aiming at providing short and reliable latency (<<1ms) control of `ieee80211_ops` in `mac80211` module. This toolset is consisted of *kernel-space* hook module and *user-space* program, which are connected via direct memory access mounted on `procfs`.
The memory r/w is currently implemented in synchrounized block writing/reading (to be lockless ring-buffer impl).


As a demo, this project currently only provides access to `conf_tx` function in `mac80211` kernel module.

### Installation
1. `make build-krn` to build kernel module `wlsops_hook.ko` (in './');
2. `make build-usr` to build userspace program `wlsctrl` (in './wlsctrl');
3. (Optional) `make install` will put kernel module to default system folder, and userspace program to `/usr/bin` (previlege required)

### How to Use
0. Make sure that you have one wireless NIC and have it enabled;
1. Fisrt of all, `make insmod` or `modprobe wlsops_hook` (if you run `make install` before) to install the kernel hook module, and it will hook onto the first entry of wireless NIC;
2. Run `./wlsctrl/wlsctrl`;
3. You will see the time printed on console, which is the time elapsed with 1000 times communication to set the prioprity of channel access to the fastest (AIFS with 1 timeslot and No Contention Window).

### What's Next
If you are inspired by this project, please feel free to fork it!

If you have any problems, create the issue!