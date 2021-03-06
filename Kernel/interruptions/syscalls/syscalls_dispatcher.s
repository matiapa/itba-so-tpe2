/*---------------------------------------------------------------------------------------------------
|   SYSCALLS_DISPATCHER.S    |                                                          			|
|-------------------------------                                                          			|
| This file provides exception handling interruption 0x80 attention routine.		                |
---------------------------------------------------------------------------------------------------*/

.globl _syscallDispatcher

.extern sysRead
.extern sysWrite

.extern sysDraw
.extern sysGetRes
.extern sysGetTime

.extern sysCpuInfo
.extern sysCPUTemp
.extern sysGetRegBkp

.extern sysMemDump
.extern sysMemStatus
.extern sysMalloc 
.extern sysFree

.extern sysStartProcess
.extern sysGetPid
.extern sysPS
.extern sysBlock
.extern sysNice
.extern sysKill

.extern sysCreateSemaphore
.extern sysWaitSemaphore
.extern sysPostSemaphore
.extern sysDeleteSemaphore
.extern sysPipe
.extern sysPipeInfo
.extern sysForcePipe
.extern sysClosefd
.extern sysDup
.extern sysPipeWrite
.extern sysPipeRead
.extern sysSemStatus

.extern sysCreatePipe
.extern sysReadPipe 
.intel_syntax noprefix

.section .text

.include "./interruptions/macros.s"


_syscallDispatcher:
    cli
    pushStateNoRax

    cmp rax, 0
    je _read

    cmp rax, 1
    je _write

    cmp rax, 2
    je _draw

    cmp rax, 3
    je _getRes

    cmp rax, 4
    je _getTime

    cmp rax, 5
    je _cpuInfo

    cmp rax, 6
    je _cpuTemp

    cmp rax, 7
    je _getRegBkp

    cmp rax, 8
    je _memDump

    cmp rax, 9
    je _memStatus

    cmp rax, 10
    je _malloc

    cmp rax, 11
    je _free 

    cmp rax, 12
    je _startProcess

    cmp rax, 13
    je _get_pid

    cmp rax, 14
    je _ps

    cmp rax, 15
    je _block

    cmp rax, 16
    je _nice

    cmp rax, 17
    je _kill

    cmp rax, 18
    je _openSemaphore

    cmp rax, 19
    je _waitSemaphore

    cmp rax, 20
    je _postSemaphore

    cmp rax, 21
    je _closeSemaphore

    cmp rax, 22
    je _pipe

    cmp rax, 23
    je _closefd

    cmp rax, 24
    je _forcepipe

    cmp rax, 25
    je _pipe_read

    cmp rax,26
    je _semStatus

    cmp rax, 27
    je _pipeInfo

    cmp rax, 28
    je _dup

    jmp endOfInt


endOfInt:
	push rax
    mov al, 0x20
	out 0x20, al
    pop rax

    popStateNoRax
    
    sti
	iretq


_read:
    call sysRead
    jmp endOfInt

_write:
    call sysWrite
    jmp endOfInt


_draw:
    call sysDraw
    jmp endOfInt
    
_getRes:
    call sysGetRes
    jmp endOfInt

_getTime:
    call sysGetTime
    jmp endOfInt


_cpuInfo:
    call sysCpuInfo
    jmp endOfInt

_cpuTemp:
    call sysCPUTemp
    jmp endOfInt

_getRegBkp:
    call sysGetRegBkp
    jmp endOfInt


_memDump:
    call sysMemDump
    jmp endOfInt

_memStatus:
    call sysMemStatus 
    jmp endOfInt

_malloc:
    call sysMalloc 
    jmp endOfInt

_free:
    call sysFree
    jmp endOfInt


_startProcess:
    call sysStartProcess 
    jmp endOfInt

_get_pid:
    call sysGetPid
    jmp endOfInt

_ps:
    call sysPS
    jmp endOfInt

_block:
    call sysBlock
    jmp endOfInt

_nice:
    call sysNice
    jmp endOfInt

_kill:
    call sysKill
    jmp endOfInt


_openSemaphore:
    call sysCreateSemaphore
    jmp endOfInt

_waitSemaphore:
    call sysWaitSemaphore
    jmp endOfInt

_postSemaphore:
    call sysPostSemaphore
    jmp endOfInt

_closeSemaphore:
    call sysDeleteSemaphore
    jmp endOfInt

_pipe:
    call sysPipe
    jmp endOfInt

_pipeInfo:
    call sysPipeInfo
    jmp endOfInt

_forcepipe:
    call sysForcePipe
    jmp endOfInt

_closefd:
    call sysClosefd
    jmp endOfInt

_dup:
    call sysDup
    jmp endOfInt

_pipe_read:
    call sysPipeRead
    jmp endOfInt

_semStatus:
    call sysSemStatus
    jmp endOfInt
