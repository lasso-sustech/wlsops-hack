#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "wlsctrl.h"

static int fd;
static info_blk *w_blk;
//<little-edian> ac-16, cwMin-16, cwMax-16, txop-16, aifs-8
static const char tx_prior[9] = {0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00}; //0,1,1,0,0
static const char tx_last[9]  = {0x00, 0x00, 0xFF, 0x03, 0xFF, 0x07, 0x00, 0x00, 0x0F}; //0,1023,2047,0,15

int setTxPrior(void)
{
    return 0;
}

int setTxNormal(void)
{
    return 0; 
}

int setTxLast(void)
{
    return 0;
}

int w_init()
{
    if( (fd=open(DBGFS_FILE, O_RDWR|O_SYNC)) <= 0 )
    {
        perror("Open call failed");
        return -1;
    }

    if( (w_blk=(info_blk *)mmap(NULL, sizeof(info_blk), PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0)) \
                == MAP_FAILED )
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
    return 0;
}

int main(int argc, char const *argv[])
{
    if (w_init() < 0)
    {
        return -1;
    }

    // memcpy(w_blk, tx_last, sizeof(tx_last));

    w_fini();
    
    return 0;
}