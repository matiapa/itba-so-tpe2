// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*---------------------------------------------------------------------------------------------------
|   SYSCALLS_OTHER.C    |                                                                           |
|------------------------                                                                           |
| These functions implement miscellaneous syscalls.           	                                    |
---------------------------------------------------------------------------------------------------*/

#include <rtc_driver.h>
#include <stdint.h>

/* ---------------------------------------------------------------------------------------------------------------------------
                                                    GETTIME
--------------------------------------------------------------------------------------------------------------------------- */

typedef struct Time{
    int hours, minutes, seconds;
} Time;


int sysGetTime(Time * time){

    time->hours = getHours();
    time->minutes = getMinutes();
    time->seconds = getSeconds();

    return 0;

}

/* ---------------------------------------------------------------------------------------------------------------------------
                                                    GETREGBKP
--------------------------------------------------------------------------------------------------------------------------- */

typedef struct RegBkp{
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
} RegBkp;

void dumpState(RegBkp *dest);

int sysGetRegBkp(RegBkp *dest){

    dumpState(dest);

    return 0;

}