#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm2.h"


instruction *machine_decode(int inst) {
	instruction *i = malloc(sizeof(instruction));
	i->inst = (inst & 0xF000) >> 12;
	i->op1 = (inst & 0xF00) >> 8;
	i->op2 = (inst & 0xF0) >> 4;
	i->op3 = (inst & 0xF);
	i->imm = (inst & 0xFF);
	return i;
}

void machine_fill_instruction(machine *m, int inst) {
	instruction in = *machine_decode(inst);
	for (int i = 0; i < m->code_size; i++) {
		m->code[i] = in;
	}
}

void machine_init(machine *m, int codesize) {
	//General Purpose
	m->r0 = 255;
	m->r1 = 1;
	m->r2 = 2;
	m->r3 = 3;
	//Reserved
	m->pc = 0;
	m->sp = 0;
	//Flags
	m->cflag = 0;
	m->zflag = 0;
	m->oflag = 0;

	m->code_size = codesize;
	m->stack_size = 255;
	m->code = malloc(sizeof(instruction) * m->code_size);
	m->stack = malloc(sizeof(short) * m->stack_size);
	
}

void machine_display_registers(machine *m) {
	printf("R0: %04X(%d)\t",m->r0, m->r0);
	printf("R1: %04X(%d)\t", m->r1, m->r1);
	printf("R2: %04X(%d)\t", m->r2, m->r2);
	printf("R3: %04X(%d)\n", m->r3, m->r3);

	printf("PC: %04X\t", m->pc);
	printf("SP: %04X\t", m->sp);
	printf("FLAGS: \nCarry: %d\nZero: %d\nOverflow: %d\n", m->cflag, m->zflag, m->oflag);
}

//Converts char to int
int get_int(char *input) {
	int i = 0;
	sscanf(input, "%4d", &i);
	return i;
}

//filters invalid registers
int filter_register(int reg) {
	return reg <= 3 ? reg : R0;
}


void print_banner() {
	printf("VM2 <Register: 0-3> <value: 0-65535>\n");
	printf("VM2 -test <instruction> <operand> <operand>\n");

}

void handle_cmdline(char *argv[]) {
	
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
	else {
		return 0;
	}
}


int main(int argc, char *argv[]) {
	int count = (argc - 1);

	//No parameters
	if (count == 0) {
		print_banner();
		return 1;
	}

	//More then one parameter
	else if (count >= 1) {
		char *option = argv[1];

		if (strstr(option, "-a") || strstr(option, "--assemble")) {
			printf("Live Assembler Mode\n");
		}
		else if (strstr(option, "-t") || strstr(option, "--test")) {
			machine my_vm;
			machine_init(&my_vm, 15);
			if (count >= 3) {
				test_instruction(&my_vm, argc, argv);
				machine_display_registers(&my_vm);
			}
		}
	}


}
