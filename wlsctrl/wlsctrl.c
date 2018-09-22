#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "wlsctrl.h"

static info_blk *w_blk;
//ac-16, cwMin-16, cwMax-16, txop-16, aifs-8
static const char tx_prior[9] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00}; //0,1,1,0,0
static const char tx_last[9]  = {0x00, 0x00, 0x03, 0xFF, 0x07, 0xFF, 0x00, 0x00, 0x0F}; //0,1023,2047,0,15

static inline int isNotLocked()
{
    return (w_blk->byte_ptr[15]&0xFF) == 0x00;
}

static inline void setLocked()
{
    w_blk->byte_ptr[15] |= 0x80;
}

int w_init()
{
    int fd;

    fd = open("/sys/kernel/debug/" DBGFS_FILE, O_RDWR);
    if(fd < 0)
    {
        perror("Open call failed");
        return -1;
    }

    w_blk = (info_blk *)mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (w_blk == MAP_FAILED)
    {
        perror("mmap operation failed");
        return -1;
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    int fd;

    fd = w_init();
    if (fd < 0)
    {
        return -1;
    }
    
    memcpy(w_blk, tx_prior, sizeof(tx_prior));
    setLocked();
    sleep(1);
    memcpy(w_blk, tx_prior, sizeof(tx_last));
    setLocked();

    close(fd);
    return 0;
}