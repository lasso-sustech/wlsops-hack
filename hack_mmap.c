#include <linux/sched.h>
#include <asm/uaccess.h> /* copy_from_user */
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include "hack_mmap.h"

static struct mmap_info *m_info;

/******************** STRUCT MMAP OPERATIONS ********************/
void mmap_open(struct vm_area_struct *vma)
{
    // info_blk *tmp_info = (mmap_info *)vma->vm_private_data
}
void mmap_close(struct vm_area_struct *vma)
{
    // info_blk *tmp_info = (mmap_info *)vma->vm_private_data;
}
static int mmap_fault(struct vm_fault *vmf)
{
    struct page *v_page;

    v_page = virt_to_page(m_info->blk);
    get_page(v_page);
    vmf->page = v_page;
     
    return 0;
}

struct vm_operations_struct mmap_vm_ops =
{
    .open = mmap_open,
    .close = mmap_close,
    .fault = mmap_fault
};

/******************** STRUCT FILE OPERATIONS ********************/
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
    fd->private_data = m_info;    // attach memory to procfs fd
    return 0;
}

static int fop_close_mmap(struct inode *inode, struct file *fd)
{
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
    return 0;
}

static const struct file_operations mmap_fops = {
    .mmap = mmap_ops_mount,
    .open = fop_open_mmap,
    .release = fop_close_mmap,
    .read = default_read_file,
    .write = default_write_file,
};

/******************** CUSTOMIZED FUNCTIONS ********************/
inline int mmap_read(char *ptr, size_t len)
{
    if (m_info->blk->byte_ptr[15] & 0x80) //user write complete
    {
        memcpy(ptr, m_info->blk, len);
        m_info->blk->byte_ptr[15] = 0x00; //set user writable
        return 1;
    }
    else
    {
        return 0;
    }
}

int hack_mmap_init()
{
    //NOTE: everytime the user access the procfs, the same dirty memory would be served.
    m_info = kmalloc(sizeof(struct mmap_info), GFP_KERNEL);
    m_info->blk = (info_blk *)get_zeroed_page(GFP_KERNEL);
    proc_create(DBGFS_FILE, 0, NULL, &mmap_fops);
    return 0;
}

void hack_mmap_fini()
{
    remove_proc_entry(DBGFS_FILE, NULL);
    free_page((unsigned long)m_info->blk);
    kfree(m_info);
}