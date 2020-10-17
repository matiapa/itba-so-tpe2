#include <syscalls.h>
#include <asm_lib.h>
#include <windows_lib.h>
#include <std_lib.h>
#include <shell_builtins.h>
#include <stdint.h>

/* ----------------------------------------------------------------------------------------------------
						                MISCELLANEOUS
----------------------------------------------------------------------------------------------------- */

void printHelp(void){
	newLine();
	printLine("---------------------------------------------------");
	newLine();

	printLine("- help              to go to the Help Manual");
	printLine("- clear             to clear the screen");
	newLine();
	
	printLine("- time              to get the system time");
	printLine("- divzero           to execute a Div by Zero");
	printLine("- invopcode         to exectue an invalid opcode");
	newLine();
	
	printLine("- display anon      to show an image of Anonymous");
	printLine("- display matrix    to show an image of Matrix");
	newLine();

	printLine("- cputemp           to get the CPU Temp");
	printLine("- cpuinfo           to get the CPU Brand Data");
	printLine("- inforeg           to get the Register's Values");
	printLine("- storedreg         to get the Stored Register's Values");
	newLine();
	
	printLine("- memdump n         to print memory starting at n");
	printLine("- mem          to print heap status");
	newLine();

	printLine("- ps                to list active processes");
	printLine("- kill pid          to kill a process");
	printLine("- block pid         to block a process");
	printLine("- nice pid priority to change priority (100 - 139)");
    printLine("- line              to draw a line (for testing)");
    printLine("- loop              to start loop (for testing)");
	newLine();
	
	printLine("To scroll the window up, press F3");
	printLine("To store registers value, press TAB");
	newLine();
	
	printLine("---------------------------------------------------");
	newLine();
}


void printTime(void){
	Time t;
	getTime(&t);

	printf("\\nTime now: %2d:%2d:%2d\\n", 3, t.hours, t.minutes, t.seconds);
}


void divZeroException(void){
	int a;
	a = 2 / 0;
}


void invalidOpcode();

void invOpcodeException(void){
	invalidOpcode();
}


void printWarning(){
	printf("\\n >> Error: ", 0);
	printLine("Command not found");
	printLine("If you want to see the command manual type 'help'.");
}


/* ----------------------------------------------------------------------------------------------------
						                CPU MANAGEMENT
----------------------------------------------------------------------------------------------------- */

void printCPUTemp(void){
	int temp = cpuTemp();
	printf("\\n Computer's Temperature: %d\\n", 1, temp);
}


void printCPUInfo(void){
	CPUInfo info;

	char brandName[50], brandDesc[70];
	info.brandName = (char *)&brandName;
	info.brandDesc = (char *)&brandDesc;

	cpuInfo(&info);

	printf("\\nBrand name: %s", 1, info.brandName);
	printf("\\nBrand description: %s\\n", 1, info.brandDesc);
}


void printInfoReg(void){
	RegDump reg;
	regDump(&reg);

	printLine("Register's values:");
	printLine("--- --- --- --- --- --- --- --- --- --- --- --- ---");
	printf("\\n - rax - %x", 1, reg.rax);
	printf("\\n - rbx - %x", 1, reg.rbx);
	printf("\\n - rcx - %x", 1, reg.rcx);
	printf("\\n - rdx - %x", 1, reg.rdx);
	printf("\\n - rsi - %x", 1, reg.rsi);
	printf("\\n - rdi - %x", 1, reg.rdi);
	printf("\\n - rbp - %x", 1, reg.rbp);
	printf("\\n - rsp - %x", 1, reg.rsp);

	printf("\\n - r8 - %x", 1, reg.r8);
	printf("\\n - r9 - %x", 1, reg.r9);
	printf("\\n - r10 - %x", 1, reg.r10);
	printf("\\n - r11 - %x", 1, reg.r11);
	printf("\\n - r12 - %x", 1, reg.r12);
	printf("\\n - r13 - %x", 1, reg.r13);
	printf("\\n - r14 - %x", 1, reg.r14);
	printf("\\n - r15 - %x\\n", 1, reg.r15);

	printLine("--- --- --- --- --- --- --- --- --- --- --- --- ---");
}


void printStoredReg(void){
	RegBkp reg;
	getRegBkp(&reg);

	printLine("Stored register's values:");
	printLine("--- --- --- --- --- --- --- --- --- --- --- --- ---");
	printf("\\n - rax - %x", 1, reg.rax);
	printf("\\n - rbx - %x", 1, reg.rbx);
	printf("\\n - rcx - %x", 1, reg.rcx);
	printf("\\n - rdx - %x", 1, reg.rdx);
	printf("\\n - rsi - %x", 1, reg.rsi);
	printf("\\n - rdi - %x", 1, reg.rdi);
	printf("\\n - rbp - %x", 1, reg.rbp);

	printf("\\n - r8 - %x", 1, reg.r8);
	printf("\\n - r9 - %x", 1, reg.r9);
	printf("\\n - r10 - %x", 1, reg.r10);
	printf("\\n - r11 - %x", 1, reg.r11);
	printf("\\n - r12 - %x", 1, reg.r12);
	printf("\\n - r13 - %x", 1, reg.r13);
	printf("\\n - r14 - %x", 1, reg.r14);
	printf("\\n - r15 - %x\\n", 1, reg.r15);

	printLine("--- --- --- --- --- --- --- --- --- --- --- --- ---");
}


/* ----------------------------------------------------------------------------------------------------
						                MEMORY MANAGEMENT
----------------------------------------------------------------------------------------------------- */

void printMemDump(char *sourceStr){

	uint64_t sourceHex = 0;
	for (int i = 2; sourceStr[i]; i++){
		char c = '0';
		if (isDigit(sourceStr[i]))
			c = '0';
		else if (isLower(sourceStr[i]))
			c = 'a';
		else
			c = 'A';
		sourceHex = sourceHex * 16 + sourceStr[i] - c;
	}

	if (sourceHex < 0){
		printLine("Parameter not allowed");
		return;
	}

	char *src = (void *) 0;
	src = (char *) sourceHex;
	char *dst = src + 32;

	memDump((void *)src, (void *)dst);

	newLine();
	for (int i = 0; i < 33; i += 8){
		printf("%2x: %2x %2x %2x %2x %2x %2x %2x %2x\\n", 9, src + i, src[i], src[i + 1], src[i + 2], src[i + 3],
			   src[i + 4], src[i + 5], src[i + 6], src[i + 7]);
	}
	
}


void printMemStatus(){
	print_mem_status();
}


/* ----------------------------------------------------------------------------------------------------
						                PROCESS MANAGEMENT
----------------------------------------------------------------------------------------------------- */

void printProcData(){
	char  buffer[90 * 40];
    if(buffer == (void *) 0)
		return;

    ps(buffer);    
    printf(buffer, 0);  


}



void killProcess(char * pid){
	int _pid = atoi(pid);

	if(_pid==-1){
		printLine("Argument must be a pid. Use ps to see processes");
		return;
	}
    else if(_pid==0){
        printLine("Cannot kill Shell process");
        return;
    }

	kill(_pid);
}


void blockProcess(char * pid){
	int _pid= atoi(pid);

	if(_pid==-1){
		printLine("Argument must be a pid. Use ps to see processes");
		return;
	}

	block(_pid);
}

void niceProcess(char * pid, char * priority) {
	int _pid = atoi(pid);
	int _prio = atoi(priority);

	if(_pid==-1){
		printLine("Argument must be a pid. Use ps to see processes");
		return;
	}

	nice(_pid, _prio);
}