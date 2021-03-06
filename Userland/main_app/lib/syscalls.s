/*---------------------------------------------------------------------------------------------------
|   SYSCALLS.S    |                                                                                 |
|----------------                                                                                   |
| This libary implements callers for all the syscalls provided by the kernel                        |
---------------------------------------------------------------------------------------------------*/

.intel_syntax noprefix
.global valueSet

.macro syscall name, code
.global \name
\name:
    mov eax, \code
    int 0x80
    ret
.endm

syscall read, 0
syscall write, 1

syscall draw, 2
syscall getRes, 3
syscall getTime, 4

syscall cpuInfo, 5
syscall cpuTemp, 6
syscall getRegBkp, 7

syscall memDump, 8
syscall memStatus, 9
syscall malloc, 10
syscall free, 11

syscall startProcess, 12
syscall getPid, 13
syscall ps, 14
syscall block, 15
syscall nice, 16
syscall kill, 17

syscall openSem, 18
syscall waitSem, 19
syscall postSem, 20
syscall closeSem, 21
syscall pipe, 22
syscall close, 23
syscall forcePipe, 24
syscall pipeRead, 25
syscall semStatus,26
syscall pipeInfo, 27
syscall dup, 28
