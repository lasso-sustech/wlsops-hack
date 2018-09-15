#include <linux/sched.h>
#include <asm/uaccess.h> /* copy_from_user */
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include "hack_mmap.h"

struct dentry *__file;

struct mmap_info
{
    char *data;
    int reference; //count accessing user
};

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
     
    page = virt_to_page(info->data);
     
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
    vma->vm_flags |= VM_RESERVED;
    vma->vm_private_data = fd->private_data;
    mmap_open(vma);
    return 0;
}

static int fop_open_mmap(struct inode *inode, struct file *fd)
{
    const unsigned char fd_info[] = "Hello World";//fd->f_path.dentry->d_name.name;

    struct mmap_info *info = kmalloc(sizeof(struct mmap_info), GFP_KERNEL);
    info->data = (char *)get_zeroed_page(GFP_KERNEL);
    
    memcpy(info->data, fd_info, strlen(fd_info));   // initialize the memory with "Hello World"
    fd->private_data = info;                        // attach memory to debugfs fd
    return 0;
}

static int fop_close_mmap(struct inode *inode, struct file *fd)
{
    struct mmap_info *info = fd->private_data;

    free_page((unsigned long)info->data);
    kfree(info);
    fd->private_data = NULL;
    return 0;
}

static const struct file_operations mmap_fops = {
    .open    = fop_open_mmap,
    .release = fop_close_mmap,
    .mmap    = mmap_ops_mount,
};

int hack_mmap_init()
{
    __file = debugfs_create_file(DBGFS_FILE, 0644, NULL, NULL, &mmap_fops);
    return 0;
}

void hack_mmap_fini()
{
    debugfs_remove(__file);
}