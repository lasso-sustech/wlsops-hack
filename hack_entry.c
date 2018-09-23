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

struct task_struct *kThread;

int read_loop(void *data)
{
    info_blk *blk = mmap_access();

    while(1)
    {
        set_current_state(TASK_UNINTERRUPTIBLE);
        if (kthread_should_stop())
        {
            break;
        }
        schedule();
        if (mmap_isNotLocked())
        {
            wls_conf_tx(blk->word_ptr[0], blk->word_ptr[1], blk->word_ptr[2], \
                        blk->word_ptr[3], blk->byte_ptr[9]);
            printh("Accessed Once\n");
            mmap_setLocked();
        }
    }
    return 0;
}

static int __init wlsops_init(void)
{
    int ret;

    ret = wls_hacker_init();
    ret = hack_mmap_init();

    kThread = kthread_create(read_loop, NULL, "wls_hack");
    
    return 0;
}

static void __exit wlsops_fini(void)
{
    hack_mmap_fini();
    printh("Now exit~\n");
}

module_init(wlsops_init);
module_exit(wlsops_fini);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lab1112-VLC");