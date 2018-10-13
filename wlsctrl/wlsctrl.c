#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "wlsctrl.h"
#include "timing_usr.h"

static int fd;
static info_blk *w_blk;
//<little-edian> ac-16, cwMin-16, cwMax-16, txop-16, aifs-8
const char tx_prior[9]   = {0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00}; //0,1,1,0,0
const char tx_normal[9]  = {0x00, 0x00, 0x03, 0x00, 0x07, 0x00, 0x00, 0x00, 0x02}; //0,3,7,0,2
const char tx_last[9]    = {0x00, 0x00, 0xFF, 0x03, 0xFF, 0x07, 0x00, 0x00, 0x0F}; //0,1023,2047,0,15

static inline int mmap_write(const char *src, size_t len)
{
    if ((w_blk->byte_ptr[15]&0xFF) == 0x00) //kernel read complete
    {
        memcpy(w_blk, src, len);
        w_blk->byte_ptr[15] |= 0x80; //set kernel readable
        return 1;
    }
    else
    {
        return 0;
    }
}

int w_writer(const char *ptr)
{
    int cnt = 0;
    while(mmap_write(ptr, 9)==0)
    {
        if(++cnt>MAX_TIMEOUT)
        {
            return -1;
        }
    }
    return 0;
}

int setTxPrior(void)
{
    return w_writer(tx_prior);
}

int setTxNormal(void)
{
    return w_writer(tx_normal);
}

int setTxLast(void)
{
    return w_writer(tx_last);
}

int w_init()
{
    if( (fd=open(DBGFS_FILE, O_RDWR|O_SYNC)) < 0 )
    {
        perror("open call failed");
        return -1;
    }

    if( (w_blk=(info_blk *)mmap(NULL, sizeof(info_blk), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED )
    {
        perror("mmap operation failed");
        return -1;
    }

    return 0;
}

void w_fini()
{
    munmap(w_blk, sizeof(w_blk));
    close(fd);
}