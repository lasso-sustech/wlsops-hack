#ifndef __HACK_MMAP_H__
#define __HACK_MMAP_H__

#include "WLSINC.h"

// #ifndef VM_RESERVED
// #define VM_RESERVED   (VM_DONTEXPAND | VM_DONTDUMP)
// #endif

// #define DBGFS_FILE "wlsctrl"

int hack_mmap_init(void);
void hack_mmap_fini(void);

#endif
