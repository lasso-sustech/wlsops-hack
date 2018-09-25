#include <linux/sched.h>
#include <asm/uaccess.h> /* copy_from_user */
#include <linux/fs.h>
// #include <linux/debugfs.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include "hack_mmap.h"

static struct dentry *__file;

inline info_blk* mmap_access()
{
    return info->blk;
}

inline int mmap_isNotWriting()
{
    return info->blk->byte_ptr[15] & 0x80;
}

inline void mmap_setWritable()
{
    info->blk->byte_ptr[15] &= 0x00;
}

void mmap_open(struct vm_area_struct *vma)
{
    struct mmap_info *info = (struct mmap_info *)vma->vm_private_data;
    info->reference++;
}
 
void mmap_close(struct vm_area_struct *vma)
{
    struct mmap_info *info = (struct mmap_info *)vma->vm_private_data;
    info->reference--;
}
 
static int mmap_fault(struct vm_fault *vmf)
{
    struct page *page;
    struct mmap_info *info;

    info = (struct mmap_info *)vmf->vma->vm_private_data;
     
    page = virt_to_page(info->blk);
     
    get_page(page);
    vmf->page = page;
     
    return 0;
}

struct vm_operations_struct mmap_vm_ops =
{
    .open = mmap_open,
    .close = mmap_close,
    .fault = mmap_fault
};
 
int mmap_ops_mount(struct file *fd, struct vm_area_struct *vma)
{
    vma->vm_ops = &mmap_vm_ops;
    vma->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP;//VM_RESERVED;
    vma->vm_private_data = fd->private_data;
    mmap_open(vma);
    return 0;
}

static int fop_open_mmap(struct inode *inode, struct file *fd)
{
    fd->private_data = info;    // attach memory to debugfs fd
    kThread = kthread_create(read_loop, NULL, "wls_hack");
    wake_up_process(kThread);
    return 0;
}

static int fop_close_mmap(struct inode *inode, struct file *fd)
{
    // struct mmap_info *info = fd->private_data;
    kthread_stop(kThread);
    fd->private_data = NULL;
    return 0;
}

static ssize_t default_read_file(struct file *file, char __user *buf,
				 size_t count, loff_t *ppos)
{
	return 0;
}

static ssize_t default_write_file(struct file *file, const char __user *buf,
				   size_t count, loff_t *ppos)
{
	return count;
}

static const struct file_operations mmap_fops = {
    .mmap = mmap_ops_mount,
    .open = fop_open_mmap,
    .release = fop_close_mmap,
    .read = default_read_file,
    .write = default_write_file,
};

int hack_mmap_init()
{
    // __file = debugfs_create_file(DBGFS_FILE, 0644, NULL, NULL, &mmap_fops);
    proc_create(DBGFS_FILE, 0, NULL, &mmap_fops);
    return 0;
}

void hack_mmap_fini()
{
    // debugfs_remove(__file);
    remove_proc_entry(DBGFS_FILE, NULL);
}