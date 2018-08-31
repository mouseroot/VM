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
	m->r0 = 0;
	m->r1 = 0;
	m->r2 = 0;
	m->r3 = 0;
	m->pc = 0;
	m->sp = 0;
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
	printf("SP: %04X\t\n", m->sp);
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


int main(int argc, char *argv[]) {
	if (argc == 1) {
		print_banner();
		return 1;
	}
	else {
		machine my_vm;

		//Setup the VM
		machine_init(&my_vm, 15); //15 instructions

		//Small inputs test
		//test_loadi(&my_vm, argc, argv);
		test_instruction(&my_vm, argc, argv);

		//vm_loadi(&my_vm, R1, 0x01);
		//vm_loadi(&my_vm, R2, 0x02);
		//vm_add(&my_vm, R0, 0x1);
		//vm_cmp(&my_vm, R1, R2);

		//Stdout the registers
		machine_display_registers(&my_vm);

		//for (int i = 0; i < my_vm.code_size; i++) {
		//	printf("Instruction #%d: %1X ->",i, my_vm.code[i].inst);
		//	printf("%2X %2X %2X %04X\n", my_vm.code[i].op1, my_vm.code[i].op2, my_vm.code[i].op3, my_vm.code[i].imm);
		//}
	}

}
