#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"

//Converts int to Instruction
//TODO: rename these
instruction *machine_decode(int inst) {
	instruction *i = malloc(sizeof(instruction));
	i->inst = (inst & 0xF000) >> 12;
	i->op1 = (inst & 0xF00) >> 8;
	i->op2 = (inst & 0xF0) >> 4;
	i->imm = (inst & 0xFF);
	return i;
}

//Takes the integer values and creates an int 
int machine_encode(int instr, int reg1, int reg2, int imm) {
	int op = instr << 12;
	if (imm != 0) {
		op = op | (imm & 0xFF);
		op = op | reg1 << 8;
	}
	else {
		op = op | reg1 << 8;
		op = op | reg2 << 4;
	}
	//printf("Opcode: %04X(%d)\n",op,op);
	return op;
}

int machine_to_opcode(instruction *inp) {
	int op = machine_encode(inp->inst, inp->op1, inp->op2, inp->imm);
	return op;
}

/*
Machine methods
*/
//Floods the existing buffer with a specified instruction
//TODO: find a use for this/perhaps to NOP/ZERO on init?
void machine_fill_instruction(machine *m, int inst) {
	instruction in = *machine_decode(inst);
	for (int i = 0; i < m->code_size; i++) {
		m->code[i] = in;
	}
}

//Adds an instruction to the machine
void machine_add_instruction(machine *m, instruction in) {
	m->code[m->code_size] = in;
	m->code_size++;
}

void machine_execute_instruction(machine *m, instruction *inst) {
	//instruction *inst = &m->code[m->pc];
	//printf("Execute ");
	switch (inst->inst)
	{
	case INSTR_LOADI:
		//printf("loadi %s %d (%04X)\n", get_register_name(inst->op1), inst->imm, machine_to_opcode(inst));
		vm_loadi(m, inst->op1, inst->imm);
		break;
	case INSTR_LOADR:
		//printf("loadr %s %s (%04X)\n", get_register_name(inst->op1), get_register_name(inst->op2), machine_to_opcode(inst));
		vm_loadr(m, inst->op1, inst->op2);
		break;
	case INSTR_ADD:
		//printf("add %s %d (%04X)\n", get_register_name(inst->op1), inst->imm, machine_to_opcode(inst));
		vm_add(m, inst->op1, inst->imm);
		break;
	case INSTR_ADDR:
		//printf("addr %s %s (%04X)\n", get_register_name(inst->op1), get_register_name(inst->op2), machine_to_opcode(inst));
		vm_addr(m, inst->op1, inst->op2);
		break;
	case INSTR_SUB:
		//printf("sub %s %d (%04X)\n", get_register_name(inst->op1), inst->imm, machine_to_opcode(inst));
		vm_sub(m, inst->op1, inst->imm);
		break;
	case INSTR_SUBR:
		//printf("subr %s %s (%04X)\n", get_register_name(inst->op1), get_register_name(inst->op2), machine_to_opcode(inst));
		vm_subr(m, inst->op1, inst->op2);
		break;
	case INSTR_PUSHI:
		//printf("pushi %d (%04X)\n", inst->imm, machine_to_opcode(inst));
		vm_pushi(m, inst->imm);
		break;
	case INSTR_PUSHR:
		//printf("pushr %s (%04X)\n", inst->imm, get_register_name(inst->op1));
		vm_pushr(m, inst->op1);
		break;
	case INSTR_INC:
		//printf("inc\n");
		break;
	case INSTR_DEC:
		//printf("dec\n");
		break;
	case INSTR_CMP:
		//printf("cmp %s %d (%04X)\n", get_register_name(inst->op1), inst->imm, machine_to_opcode(inst));
		vm_cmp(m, inst->op1, inst->imm);
		break;
	case INSTR_JNZ:
		//printf("jnz -Not Yet Implimented\n");
		break;
	case INSTR_JMP:
		//printf("jmp -Not Yet Implimented\n");
		break;
	case INSTR_CMPR:
		//printf("cmpr %s %s (%04X)\n", get_register_name(inst->op1), get_register_name(inst->op2), machine_to_opcode(inst));
		vm_cmpr(m, inst->op1, inst->op2);
		break;
	case INSTR_HALT:
		printf("halt\n");
		break;

	default:
		//printf("UNKNOWN\n");
		break;
	}
}

//Used to setup the machine
//Zeros everything out and allocates memory

void machine_init(machine *m) {
	//General Purpose
	m->r0 = 0;
	m->r1 = 0;
	m->r2 = 0;
	m->r3 = 0;
	//Reserved
	m->pc = 0;
	m->sp = 0;
	//Flags
	m->cflag = 0;
	m->zflag = 0;
	m->oflag = 0;

	//Write counter
	m->wc = 0;

	m->code_size = 0;
	m->stack_size = 255;
	for (int i = 0; i < m->stack_size; i++) {
		m->stack[i] = 0;
	}
}


//Displays the registers/stack/flags
void machine_display_registers(machine *m) {
	printf("R0: %04X(%d)\t", m->r0, m->r0);
	printf("R1: %04X(%d)\t", m->r1, m->r1);
	printf("R2: %04X(%d)\t", m->r2, m->r2);
	printf("R3: %04X(%d)\n", m->r3, m->r3);

	printf("PC: %04X\t", m->pc);
	printf("SP: %04X\t", m->sp);
	printf("FLAGS[CZO]: %d %d %d\n", m->cflag, m->zflag, m->oflag);
}

//TODO: write memory display

//TODO: write stack display