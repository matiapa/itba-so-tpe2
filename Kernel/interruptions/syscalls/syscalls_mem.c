// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*---------------------------------------------------------------------------------------------------
|   SYSCALLS_MEM.C    |                                                                             |
|--------------------                                                                               |
| These functions implement syscalls related to the memory.   	                                    |
---------------------------------------------------------------------------------------------------*/

#include <lib.h>
#include <mem_manager.h>

/* ---------------------------------------------------------------------------------------------------------------------------
                                                    MEMDUMP
--------------------------------------------------------------------------------------------------------------------------- */

int sysMemDump(void *src, void *dest){

    memcpy(dest, src, 32);

    return 0;

}

/* ---------------------------------------------------------------------------------------------------------------------------
                                                MEMORY ALLOCATION
--------------------------------------------------------------------------------------------------------------------------- */

void * sysMalloc (int size){
    return malloc(size);
}


void sysFree (void * ptr){
    free(ptr);
}


void sysMemStatus(int * memory_size, int * free_space, int * occupied_space){
    mem_status(memory_size,free_space,occupied_space);

}
