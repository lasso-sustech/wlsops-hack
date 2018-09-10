#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/version.h>

// #include <linux/proc_fs.h>
// #include <asm/uaccess.h>
#include "wlsops.h"

#define WLSDRV_SPEC "iwl"

struct ieee80211_local *local;

static int __init wlsops_init(void)
{
    struct net_device *dev;

    for_each_netdev(&init_net, dev)
    {
        if( (strcmp(WLSDRV_SPEC, dev->name)<=0) && dev->ieee80211_ptr ) // <net/cfg80211.h>
        {
            local = (struct ieee80211_local *)wdev_priv(dev->ieee80211_ptr);
            printk("find one driver: %s\n", dev->name);
            break;
        }
    }
    return 0;
}

static void __exit wlsops_fini(void)
{
    printk("Now exit~");
}

module_init(wlsops_init);
module_exit(wlsops_fini);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lab1112-VLC");