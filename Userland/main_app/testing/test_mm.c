// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//-V::576

#include <test_util.h>
#include <syscalls.h>
#include <std_lib.h>
#define NULL (void*) 0
extern void printProcData();

#define MAX_BLOCKS 128
#define MAX_MEMORY 100000 //Should be around 80% of memory managed by the MM

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void test_mm(){
  mm_rq mm_rqs[MAX_BLOCKS];

  //printf("ARRANCO A TESTEAR TU MM\n",0);
  while (1){
    uint8_t rq = 0;
    uint32_t total = 0;

    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address = malloc(mm_rqs[rq].size); // TODO: Port this call as required
                                                                                                        
      if(mm_rqs[rq].address==NULL);
  //      printf("NULL CON MALLOC\n",0);    
        
      total += mm_rqs[rq].size;
      rq++;
    }

    // Set
    uint32_t i;
 //   printf("VOY A ESCRIBIR\n",0);
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        memset(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required

    // Check
  // printf("VOY A CHECKEAR\n",0);
   // printProcData();
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
          printf("ERROR!\n",0); // TODO: Port this call as required

    // Free
 //   printf("ESTOY LIBERANDO\n",0);
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        free(mm_rqs[i].address);  // TODO: Port this call as required
  //  printf("SALIO TODO OK\n",0);
  } 

}