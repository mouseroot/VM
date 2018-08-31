#pragma once

#define R0 0
#define R1 1
#define R2 2
#define R3 3

#define INSTR_LOADI 1
#define INSTR_LOADR 2
#define INSTR_ADD 3


typedef struct {
	int inst;
	int op1;
	int op2;
	int op3;
	int imm;
} instruction;

typedef struct {
	int r0;
	int r1;
	int r2;
	int r3;
	int pc;
	int sp;
	int *stack;
	instruction *code;
	int code_size;
	int stack_size;
} machine;

/*
	Machine functions
	-----------------

*/

void machine_init(machine* m, int codesize);
void machine_display_registers(machine *m);
instruction *machine_decode(int inst);
void machine_fill_instruction(machine *m, int inst);

/*
	VM Functions
	------------

*/

void machine_vm_loadi(machine *m, int r_index, int value);
void machine_vm_loadr(machine *m, int r_source, int r_dest);
void machine_vm_add(machine *m, int r_index, int value);