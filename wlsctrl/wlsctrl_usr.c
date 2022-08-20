#include <stdio.h>
#include <assert.h>
#include "wlsctrl.h"
#include "timing_usr.h"

int main(int argc, char const *argv[])
{
    int ret = 0;
    timing_unit tt;

    if (w_init() < 0)
    {
        return -1;
    }

    timing_start(tt);
    ret = set_tx_params(1, 1, 1, 1);
    timing_stop(tt);

    printf("%d, %ld.%06ld\n", ret, tt.result.tv_sec, tt.result.tv_usec);
    
    w_fini();
    return 0;
}