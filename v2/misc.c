#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"



int fsize(char *filename) {
	FILE *fp;
	int fsize = 0;
	//fopen_s(&fp, filename, "r");
	fp = fopen(filename,"r");
	if (fp != NULL) {
		fseek(fp, 0, 2);
		fsize = ftell(fp);
		fclose(fp);
		return fsize;
	}
	return -1;
}

/*
	Encoder / Decoder
*/




/*
	Helpers
*/

//Converts char to int
int get_int(char *input) {
	int i = 0;
	sscanf(input, "%4d", &i);
	return i;
}

int get_immediate(char *str) {
	if (strchr(str, '#') == 0 || strchr(str, '@') == 0) {
		*str++;
		int result = atoi(str);
		return result;
	}
	else {
		return -1;
	}
}

//filters invalid registers
int filter_register(int reg) {
	return reg <= 3 ? reg : R0;
}

char *get_register_name(int reg) {
	switch (reg)
	{
	case R0:
		return strdup("R0");
		break;
	case R1:
		return strdup("R1");
		break;
	case R2:
		return strdup("R2");
		break;
	case R3:
		return strdup("R3");
		break;
	default:
		return strdup("INVALID");
		break;
	}
}

char *get_instruction_name(int inst) {
	switch (inst)
	{
	case INSTR_LOADI:
		return strdup("loadi");
		break;
	case INSTR_LOADR:
		return strdup("loadr");
		break;
	case INSTR_ADD:
		return strdup("add");
		break;
	case INSTR_ADDR:
		return strdup("addr");
		break;
	case INSTR_SUB:
		return strdup("sub");
		break;
	case INSTR_SUBR:
		return strdup("subr");
		break;
	case INSTR_PUSHI:
		return strdup("pushi");
		break;
	case INSTR_PUSHR:
		return strdup("pushr");
		break;
	case INSTR_INC:
		return strdup("inc");
		break;
	case INSTR_DEC:
		return strdup("dec");
		break;
	case INSTR_CMP:
		return strdup("cmp");
		break;
	case INSTR_JNZ:
		return strdup("jnz");
		break;
	case INSTR_JMP:
		return strdup("jmp");
		break;
	case INSTR_CMPR:
		return strdup("cmpr");
		break;
	case INSTR_HALT:
		return strdup("halt");
		break;

	default:
		return strdup("UNKNOWN");
		break;
	}
}


void input(char *prompt, char *buffer)
{
	printf(prompt);
	fgets(buffer, 1024, stdin);
}

int get_register(char *rname) {
	if (strstr(rname, "R0") || strstr(rname, "r0")) {
		return R0;
	}
	else if (strstr(rname, "R1") || strstr(rname, "r1")) {
		return R1;
	}
	else if (strstr(rname, "R2") || strstr(rname, "r2")) {
		return R2;
	}
	else if (strstr(rname, "R3") || strstr(rname, "r3")) {
		return R3;
	}
	else if (strstr(rname, "sp") || strstr(rname, "SP")) {
		return SP;
	}
	else {
		return 0;
	}
}

//Returns the value in the register
int get_register_value(machine *m, int reg) {
	switch (reg)
	{
	case R0:
		return m->r0;
		break;
	case R1:
		return m->r1;
		break;
	case R2:
		return m->r2;
		break;
	case R3:
		return m->r3;
		break;
	case SP:
		return m->sp;
		break;
	case PC:
		return m->pc;
		break;
	default:
		return -1;
		break;
	}
}

//char *remove_newline(char *str) {
//	for (char *p; *p != NULL; *p++) {
//		char c = *p;
//
//	}
//}