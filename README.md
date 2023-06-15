# Wi-Fi Driver Real-time Hack
> Target for Linux Kernel v5.15 LTS.

An alternative to `iw` which uses `nl80211` interface.

Use `procfs + mmap` to access `ieee80211_ops` within ***extremely low latency*** from userspace.

<!--
The memory r/w is currently implemented in synchronized block writing/reading (to be lockless ring-buffer impl).
-->

### Build
- build the kernel module
    > We use a custom `iwlmvm` build in this repo. You can disable it with `cmake -DBUILD_IWLMVM=OFF ..` or activate DVM with `-DBUILD_IWLMVM=OFF -DBUILD_IWLDVM=TRUE`
    ```bash
    mkdir build
    cd build && cmake ..
    make
    ```

- build and install the python controller package
    > It is recommended to install the package globally which could be found by root user.
    ```bash
    cd wlsctrl
    python3 setup.py bdist_wheel
    sudo pip3 install ./dist/*.whl
    ```

### How to Use
> Make sure that you have at least one wireless NIC enabled.

1. Run `sudo insmod build/wlsops/wlsops_hack.ko` to install the built kernel modules, where `wlsops_hack` will use the first wireless NIC found in the system;

2. Run the test in `wlsctrl/tests` with root permission.
