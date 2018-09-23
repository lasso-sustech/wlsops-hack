#ifndef __HACK_MMAP_H__
#define __HACK_MMAP_H__

#include <linux/kthread.h>
#include "WLSINC.h"

#ifndef VM_RESERVED
#define VM_RESERVED   (VM_DONTEXPAND | VM_DONTDUMP)
#endif

#define DBGFS_FILE "wlsctrl"

extern struct dentry  *file;
extern struct task_struct *kThread;

int hack_mmap_init(void);
void hack_mmap_fini(void);
inline info_blk* mmap_access(void);
inline int mmap_isNotLocked(void);
inline void mmap_setLocked(void);

#endif
