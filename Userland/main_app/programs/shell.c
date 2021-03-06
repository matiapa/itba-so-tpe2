// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//-V::576

/*---------------------------------------------------------------------------------------------------
|   SHELL.C    |                                                                             	    |
|---------------                                                                                    |
| This windows will display the shell program.														|
| Press ENTER to execute a command.																	|
---------------------------------------------------------------------------------------------------*/

#include <shell_cmds.h>
#include <syscalls.h>
#include <std_lib.h>
#include <std_io.h>
#include <asm_lib.h>

/* --------------------------------------------------------------------------------------------------------------------------
                                        		SHELL DEFINITIONS
------------------------------------------------------------------------------------------------------------------------- */

#define NULL ((void *) 0)

#define MAX_TOKENS 10
#define MAX_TOKEN_LEN 20
#define MAX_CMD_LEN (MAX_TOKENS*MAX_TOKEN_LEN)

void parseCommand(char *cmdBuff);

void dummy(int argc, char **argv);


/* --------------------------------------------------------------------------------------------------------------------------
                                        	SHELL METHODS
------------------------------------------------------------------------------------------------------------------------- */

void shell(){
    startProcess(dummy, 0, (void *) 0, "dummy", 0); //Necesario en ciertos casos

	forcePipe(0); //Creamos el pipe que comunica fd 0 con teclado
	forcePipe(1); //Creamos el pipe que comunica fd 1 con salida estandar

	char cmdBuff[MAX_CMD_LEN] = {0};
	int buffPos = 0;

	printf("\f", 0);

	while (1) {
		char c = getChar();

		// Handle the chars that are not CR

		if (c == '\b' && buffPos > 0)
			cmdBuff[--buffPos] = 0;
		
		if (c >= 32 && buffPos < MAX_CMD_LEN)
			cmdBuff[buffPos++] = c;

		putChar(c);

        // Handle the CR char, parse command

		if(c=='\r'){
			if(buffPos==0){
				putChar('\n');
				continue;
			}


			putChar('\n');

			cmdBuff[buffPos] = 0;
			parseCommand(cmdBuff);

			putChar('\n');
			putChar('\n');


            buffPos = 0;
        }
    }

 }
    


/* --------------------------------------------------------------------------------------------------------------------------
                                COMMAND-CHECK METHODS
------------------------------------------------------------------------------------------------------------------------- */

typedef struct ProcessExec{
	void (*main)(int,char**);
	int argc;
	char *argv[MAX_TOKENS];
	int foreground;
} ProcessExec;

int init=0;
ProcessExec lp, rp;


void parseSimpleCommand(char *cmdBuff, ProcessExec *exec);

void parseCommand(char *cmdBuff) {

	if(!init){
		for(int i=0; i<MAX_TOKENS; i++){
			lp.argv[i] = malloc(MAX_TOKEN_LEN);
			rp.argv[i] = malloc(MAX_TOKEN_LEN);
		}
		init = 1;
	}

	char *leftCommand = cmdBuff;
	char *rightCommand = NULL;

	for(int i=0; cmdBuff[i]; i++){
		if (cmdBuff[i] == '|'){
			cmdBuff[i-1] = 0;	// Trim previous space
			rightCommand = cmdBuff+i+2; // Skip next space
		}
	}

	parseSimpleCommand(leftCommand, &lp);

	if(rightCommand == NULL){

		startProcess(lp.main, lp.argc, lp.argv, lp.argv[0], lp.foreground);

	}else{	
		
		parseSimpleCommand(rightCommand, &rp);

		int fds[2];
		pipe(fds);

		// Both pipeLeftProcMediator and pipeRightProcMediator will receive the following args:
		// argv[0]: process name - argv[1]...argv[argc-1]: process arguments
		// argv[argc]: read fd - argv[argc+1]: write fd - argv[argc+2]: process main

		lp.argv[lp.argc] = (char *) lp.main;
		itoa(fds[0], lp.argv[lp.argc+1], 10, -1);
    	itoa(fds[1], lp.argv[lp.argc+2], 10, -1);

		rp.argv[rp.argc] = (char *) rp.main;
		itoa(fds[0], rp.argv[rp.argc+1], 10, -1);
    	itoa(fds[1], rp.argv[rp.argc+2], 10, -1);
		
		startProcess(pipeLeftProcMediator, lp.argc, lp.argv, "left_proc", 1);
		startProcess(pipeRightProcMediator, rp.argc, rp.argv, "right_proc", 0);

		close(fds[0]);
		close(fds[1]);

	}
	
}


void createProcessExec(void (*main)(int,char**), int argc, char *name, char **argv, int fg, ProcessExec *p){

	p->main = main;
	p->argc = argc;
	p->foreground = fg;

	// For the name we will use the given memory zone because the PCB stores a pointer to the name, not a copy
	// so we cant always use the same location. By passing a pointer to a literal, the problem is solved.
	p->argv[0] = name;

	for(int i=1; i<argc; i++)
		memcpy(p->argv[i], argv[i], MAX_TOKEN_LEN);

}


void parseSimpleCommand(char *cmdBuff, ProcessExec *processExec){

	// Separate on tokens

	char *tokens[MAX_TOKENS] = {0};
	
	tokens[0] = cmdBuff;

	int j=1;
	for (int i=0; cmdBuff[i]; i++){
		if (cmdBuff[i] == ' ' || cmdBuff[i] == '\t' || cmdBuff[i] == '\n' || cmdBuff[i] == '\r'){
			cmdBuff[i] = 0;
			tokens[j++] = cmdBuff+i+1;
		}
	}

	int fg = 1;
	if(strncmp(tokens[j-1], "&", 2) == 0){
		fg = 0;
		j -= 1;
	}

	// Miscellaneous

	if (strncmp(tokens[0], "help", 5) == 0 && j==1)
		createProcessExec(printHelp, 1, "help", tokens, fg, processExec);

	else if (strncmp(tokens[0], "divzero", 8) == 0 && j==1)
		createProcessExec(divZeroException, 1, "divzero", tokens, fg, processExec);

	else if (strncmp(tokens[0], "invopcode", 10) == 0 && j==1)
		createProcessExec(invOpcodeException, 1, "invopcode", tokens, fg, processExec);

	else if (strncmp(tokens[0], "time", 5) == 0 && j==1)
		createProcessExec(printTime, 1, "time", tokens, fg, processExec);

	else if(strncmp(tokens[0], "display", 8) == 0 && j==2)
		createProcessExec(displayImage, 2, "display", tokens, fg, processExec);


	else if(strncmp(tokens[0], "filter", 7) == 0 && j == 1)
		createProcessExec(filter, 1, "filter", tokens, fg, processExec);

	else if(strncmp(tokens[0], "cat", 4) == 0 && j==1)
		createProcessExec(cat, 1, "cat", tokens, fg, processExec);
	
	else if(strncmp(tokens[0], "wc", 3) == 0 && j==1)
		createProcessExec(wc, 1, "wc", tokens, fg, processExec);


	// CPU management

	else if (strncmp(tokens[0], "cputemp", 8) == 0 && j==1)
		createProcessExec(printCPUTemp, 1, "cputemp", tokens, fg, processExec);

	else if (strncmp(tokens[0], "cpuinfo", 8) == 0 && j==1)
		createProcessExec(printCPUInfo, 1, "cpuinfo", tokens, fg, processExec);

	else if (strncmp(tokens[0], "inforeg", 8) == 0 && j==1)
		createProcessExec(printInfoReg, 1, "inforeg", tokens, fg, processExec);

	else if (strncmp(tokens[0], "storedreg", 10) == 0 && j==1)
		createProcessExec(printStoredReg, 1, "storedreg", tokens, fg, processExec);


	// Memory management

	else if (strncmp(tokens[0], "memdump", 8) == 0 && j == 2)
		createProcessExec(printMemDump, 2, "memdump", tokens, fg, processExec);

	else if (strncmp(tokens[0], "mem", 4) == 0 && j==1)
		createProcessExec(printMemStatus, 1, "mem", tokens, fg, processExec);


	// Process management

	else if (strncmp(tokens[0], "kill", 5) == 0 && j == 2)
		createProcessExec(killProcess, 2, "kill", tokens, fg, processExec);

	else if (strncmp(tokens[0], "block", 6) == 0 && j == 2)
		createProcessExec(blockProcess, 2, "block", tokens, fg, processExec);

	else if (strncmp(tokens[0], "nice", 5) == 0 && j == 3)
		createProcessExec(niceProcess, 3, "nice", tokens, fg, processExec);

	else if(strncmp(tokens[0], "ps", 3)==0 && j==1)
		createProcessExec(printProcData, 2, "ps", tokens, fg, processExec);


	// New processes

	else if(strncmp(tokens[0], "line", 5) == 0 && j==1)	
		createProcessExec(line, 1, "line", tokens, fg, processExec);

	else if (strncmp(tokens[0], "loop", 5) == 0 && j==1)
		createProcessExec(loop, 1, "loop", tokens, fg, processExec);

	else if(strncmp(tokens[0], "phyloproblem", 13)==0 && j==1)
		createProcessExec(phylo, 1, "phyloproblem", tokens, fg, processExec);


	//Sync

	else if (strncmp(*tokens, "sem", 4) == 0 && j==1)
		createProcessExec(printSemStatus, 1, "sem", tokens, fg, processExec);

	else if(strncmp(tokens[0], "pipe", 5) == 0 && j==1)
		createProcessExec(printPipeInfo, 1, "pipe", tokens, fg, processExec);


	//Testing

	else if(strncmp(tokens[0], "testmm", 7) == 0 && j==1)
		createProcessExec(test_mm, 1, "testmm", tokens, fg, processExec);

	else if(strncmp(tokens[0], "testproc", 9) == 0)
		createProcessExec(test_proc, 1, "testproc", tokens, fg, processExec);

	else if(strncmp(tokens[0], "testprio", 9) == 0 && j==1)
		createProcessExec(test_prio, 1, "testprio", tokens, fg, processExec);

	else if(strncmp(tokens[0], "testsync", 9) == 0 && j==1)
		createProcessExec(test_sync, 1, "testsync", tokens, fg, processExec);

	else if(strncmp(tokens[0], "testnosync", 11) == 0 && j==1)
		createProcessExec(test_no_sync, 1, "testnosync", tokens, fg, processExec);

	else if(strncmp(tokens[0], "clear", 6) == 0 && j==1)
		createProcessExec(clearScreen, 1, "clear", tokens, fg, processExec);

	else
		createProcessExec(printWarning, 1, "warning", tokens, fg, processExec);
	
}

    
/* --------------------------------------------------------------------------------------------------------------------------
                                        	OTHER METHODS
------------------------------------------------------------------------------------------------------------------------- */

void dummy(int argc, char **argv) {
    printf("Hola soy dummy", 0);
    while(1)
        halt();
}