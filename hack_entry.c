#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/version.h>

#include "hack_mmap.h"
#include "wlsops.h"

static int __init wlsops_init(void)
{
    int ret;

    ret = wls_hacker_init();
    ret = hack_mmap_init();
    
    return 0;
}

static void __exit wlsops_fini(void)
{
    printk("Now exit~\n");
}

module_init(wlsops_init);
module_exit(wlsops_fini);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lab1112-VLC");