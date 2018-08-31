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

void vm_cmp(machine *m, int r_source, int val) {
	int ival = 0;
	switch (r_source) {
	case 0:
		ival = (val - m->r0);
		break;
	case 1:
		ival = (val - m->r1);
		break;
	case 2:
		ival = (val - m->r2);
		break;
	case 3:
		ival = (val - m->r3);
		break;
	}
	printf("CMP %d\n", ival);
}

void vm_nop(machine *m) {
	m->pc++;
}

void vm_jmp(machine *m, int jmpto) {
	int imm = (jumpto & 0xFF);
	m->pc = imm;
}

void vm_inc(machine *m, int r_index) {
	switch (r_index) {
		case 0:
			m->r0++;
			break;
		case 1:
			m->r1++;
			break;
		case 2:
			m->r2++;
			break;
		case 3:
			m->r3++;
			break;
	}
}

void vm_dec(machine *m, int r_index) {
	switch (r_index) {
		case 0:
			m->r0--;
			break;
		case 1:
			m->r1--;
			break;
		case 2:
			m->r2--;
			break;
		case 3:
			m->r3--;
			break;
	}
}

void vm_add(machine *m, int r_dest, int value) {
	int imm = (value & 0xFF);
	switch (r_dest) {
	case 0:
		m->r0 = (m->r0 + imm);
		break;
	case 1:
		m->r1 = (m->r1 + imm);
		break;
	case 2:
		m->r2 = (m->r2 + imm);
		break;
	case 3:
		m->r3 = (m->r3 + imm);
		break;
	}
}

void vm_sub(machine *m, int r_dest, int value) {
	int imm = (value & 0xFF);
	switch (r_dest) {
	case 0:
		m->r0 = (m->r0 - imm);
		break;
	case 1:
		m->r1 = (m->r1 - imm);
		break;
	case 2:
		m->r2 = (m->r2 - imm);
		break;
	case 3:
		m->r3 = (m->r3 - imm);
		break;
	}
}

void vm_loadr(machine *m, int r_source, int r_dest) {

	int rval = 0;
	switch (r_source) {

		case 0:
			rval = m->r0;
		break;

		case 1:
			rval = m->r1;
		break;

		case 2:
			rval = m->r2;
		break;

		case 3:
			rval = m->r3;
		break;

		default:
			return;
		break;
	}
	switch (r_dest) {
		case 0:
			m->r0 = rval;
			break;
		case 1:
			m->r1 = rval;
			break;
		case 2:
			m->r2 = rval;
			break;
		case 3:
			m->r3 = rval;
			break;
	}
}

void vm_loadi(machine *m, int r_index, int value) {
	int imm = (value & 0xFF);
	switch (r_index) {
		case 0:
			m->r0 = imm;
		break;

		case 1:
			m->r1 = imm;
		break;

		case 2:
			m->r2 = imm;
		break;

		case 3:
			m->r3 = imm;
		break;

		default:
			printf("Invalid Register r%d", r_index);
		break;
	}
}

void machine_init(machine *m, int codesize) {
	m->r0 = 0;
	m->r1 = 1;
	m->r2 = 2;
	m->r3 = 3;
	m->pc = 0;
	m->sp = 0;
	m->code_size = codesize;
	m->stack_size = 255;
	m->code = malloc(sizeof(instruction) * m->code_size);
	m->stack = malloc(sizeof(short) * m->stack_size);
	
}

void machine_display_registers(machine *m) {
	printf("R0: %04X\t",m->r0);
	printf("R1: %04X\t", m->r1);
	printf("R2: %04X\t", m->r2);
	printf("R3: %04X\n", m->r3);

	printf("PC: %04X\t", m->pc);
	printf("SP: %04X\t\n", m->sp);
}


int main(int argc, char *argv[]) {
	machine my_vm;

	//Setup the VM
	machine_init(&my_vm, 15); //15 instructions

	vm_loadi(&my_vm, R1, 0x01);
	vm_loadi(&my_vm, R2, 0x02);
	vm_add(&my_vm, R0, 0x1);
	vm_cmp(&my_vm, R2, R1);

	//Stdout the registers
	machine_display_registers(&my_vm);

	//for (int i = 0; i < my_vm.code_size; i++) {
	//	printf("Instruction #%d: %1X ->",i, my_vm.code[i].inst);
	//	printf("%2X %2X %2X %04X\n", my_vm.code[i].op1, my_vm.code[i].op2, my_vm.code[i].op3, my_vm.code[i].imm);
	//}

}
