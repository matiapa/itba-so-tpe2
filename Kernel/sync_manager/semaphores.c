// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//-V::675

#include <mem_manager.h>
#include <process_manager.h>
#include <interrupts.h>
#include <lib.h>
#include <semaphores.h>

Semaphore * semaphores = NULL;


void semStatus(void * buffer, int * sem_count){

    Semaphore * semaphore = semaphores;

    int count=0;
    int size= sizeof(Semaphore);

    while (semaphore!=NULL) {
        memcpy(((char *) buffer) + size * count++, semaphore, size);
        semaphore = semaphore->next;
    }

    *sem_count=count;

}


Semaphore * findSemaphore(int id, int prev){
    
    Semaphore *semaphore = semaphores;
    if(!prev)
        while(semaphore!=NULL && semaphore->id != id)
            semaphore = semaphore->next;
    else
        while(semaphore!=NULL && semaphore->next!=NULL && semaphore->next->id != id)
            semaphore = semaphore->next;

    return semaphore;

}


int openSemaphore(int id, int initValue){

    Semaphore *sem = findSemaphore(id, 0);
    if(sem == NULL){

        sem = malloc(sizeof(Semaphore));
        if(sem == NULL)
            return -1;

        sem->id = id;
        sem->value = initValue;
        sem->blockedPIDsSize = 0;
        sem->lock = 0;
        sem->listeners = 0;
        sem->next = NULL;

        Semaphore *lastSem = semaphores;
        if(lastSem == NULL){
            semaphores = lastSem = sem;
        }else{
            while(lastSem->next != NULL)
                lastSem = lastSem->next;
            lastSem->next = sem;
        }

    }

    sem->listeners += 1;

    return 0;

}


int waitSemaphore(int id) {

    Semaphore *semaphore = findSemaphore(id, 0);
    if(semaphore == NULL)
        return -1;
    
    acquire(&(semaphore->lock));

        if(semaphore->value > 0){

        semaphore->value -= 1;
        release(&(semaphore->lock));

    }else{
        
        if(semaphore->value==0)
            semaphore->value--;

        int currPid = getPID();

        semaphore->blockedPIDs[semaphore->blockedPIDsSize++] = currPid;
        
        release(&(semaphore->lock));

        blockProcess(currPid,1);

      //  acquire(&(semaphore->lock));

     //   semaphore->value -= 1;
    } 

 /*   if(semaphore->value > 0){
        semaphore->value -= 1;
    }else{
        int currPid = getPID();

        semaphore->blockedPIDs[semaphore->blockedPIDsSize++] = currPid;
        
        release(&(semaphore->lock));

        blockProcess(currPid,1);

        acquire(&(semaphore->lock));

        semaphore->value -= 1;
    } */


    //release(&(semaphore->lock));

    return 0;

}


int postSemaphore(int id){

    Semaphore * semaphore = findSemaphore(id, 0);
    if(semaphore == NULL)
        return -1;

    acquire(&(semaphore->lock));

    semaphore->value += 1;

    if(semaphore->blockedPIDsSize > 0){
       // int unblockPID = semaphore->blockedPIDs[--semaphore->blockedPIDsSize];
        int unblockPID = semaphore->blockedPIDs[0];
        for(int i=0; i<semaphore->blockedPIDsSize-1;i++){
            semaphore->blockedPIDs[i] = semaphore->blockedPIDs[i+1];
        }
        semaphore->blockedPIDsSize--;
        blockProcess(unblockPID, 0);
    }

    release(&(semaphore->lock));

    return 0;

}


void getBlockedProc(char * buffer, int id) {

    buffer[0] = 0; //Inicia vacío
    Semaphore * semaphore = findSemaphore(id, 0);
    if(semaphore == NULL)
        return;

    acquire(&(semaphore->lock));

    for(int i=0; i < semaphore->blockedPIDsSize; i++) {
        getProcName(semaphore->blockedPIDs[i], buffer + strlen(buffer));
        strcat(", ", buffer);
    }

    release(&(semaphore->lock));

    if(semaphore->blockedPIDsSize > 0)
        buffer[strlen(buffer)-2] = 0; //Quitamos el ultimo ", "

}


int closeSemaphore(int id){

    Semaphore *semaphore = findSemaphore(id,0);

    if(semaphore==NULL)
        return -1;

    if(semaphore->listeners > 0)
        semaphore->listeners -= 1;

    if(semaphore->listeners==0){

        if(semaphore->blockedPIDsSize>0){
            //Puede ser que hagamos close de un proceso que estaba bloqueado pero ya lo matamos, pero acá va a seguir en la 
            for(int i=0;i<semaphore->blockedPIDsSize;i++)
                if(findProc(semaphore->blockedPIDs[i])!=NULL)
                    return -1;
        }

        Semaphore *prevSemaphore = findSemaphore(id, 1);

        if(prevSemaphore == NULL)
            semaphores = semaphore->next;
        else
            prevSemaphore->next=semaphore->next;

        free(semaphore);

    }

    return 0;

}

