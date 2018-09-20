#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/version.h>

#include <linux/sched.h>
#include <linux/kthread.h>
#include "hack_mmap.h"
#include "wlsops.h"
#include "WLSINC.h"

static struct task_struct *kThread;

int read_loop(void *data)
{
    while(1)
    {
        set_current_state(TASK_UNINTERRUPTIBLE);
        if (kthread_should_stop())
        {
            break;
        }
        schedule();
        //TODO: read memory here
    }
    return 0;
}

static int __init wlsops_init(void)
{
    int ret;

    ret = wls_hacker_init();
    ret = hack_mmap_init();

    kThread = kthread_create(read_loop, NULL, "wls_hack");
    //TODO: promise no error when create
    wake_up_process(kThread);
    
    return 0;
}

static void __exit wlsops_fini(void)
{
    kthread_stop(kThread);
    hack_mmap_fini();
    printk("Now exit~\n");
}

module_init(wlsops_init);
module_exit(wlsops_fini);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lab1112-VLC");