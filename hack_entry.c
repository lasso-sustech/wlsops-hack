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
struct mmap_info *info;

int read_loop(void *data)
{
    info_blk *blk = mmap_access();

    set_current_state(TASK_INTERRUPTIBLE);
    while(!kthread_should_stop())
    {
        schedule();
        set_current_state(TASK_INTERRUPTIBLE);
        
        if (mmap_isNotWriting())
        {
            /* printh("ac-%d, min-%d, max-%d, txop-%d, aifs-%d\n", \
                    blk->word_ptr[0], blk->word_ptr[1], blk->word_ptr[2], \
                    blk->word_ptr[3], blk->byte_ptr[8]); */
            wls_conf_tx(blk->word_ptr[0], blk->word_ptr[1], blk->word_ptr[2], \
                        blk->word_ptr[3], blk->byte_ptr[8]);
            mmap_setWritable();
        }
    }

    // printh("Reading Loop Actually Exit\n");
    return 0;
}

static int __init wlsops_init(void)
{
    int ret;

    info = kmalloc(sizeof(struct mmap_info), GFP_KERNEL);
    info->blk = (info_blk *)get_zeroed_page(GFP_KERNEL);

    ret = wls_hacker_init();
    ret = hack_mmap_init();

    return 0;
}

static void __exit wlsops_fini(void)
{
    free_page((unsigned long)info->blk);
    kfree(info);
    hack_mmap_fini();
    
    printh("Now exit~\n");
    printh("\n");
}

module_init(wlsops_init);
module_exit(wlsops_fini);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lab1112-VLC");