#pragma once

#define R0 0
#define R1 1
#define R2 2
#define R3 3

#define INSTR_LOADI 0x1
#define INSTR_LOADR 0x2
#define INSTR_ADD	0x3
#define INSTR_SUB	0x4
#define INSTR_INC	0x5
#define INSTR_DEC	0x6
#define INSTR_CMP	0x7
#define INSTR_NOP	0x8
#define INSTR_JMP	0x9
//More
#define INSTR_HALT	0xF



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

void vm_loadi(machine *m, int r_index, int value);
void vm_loadr(machine *m, int r_source, int r_dest);
void vm_add(machine *m, int r_index, int value);
void vm_sub(machine *m, int r_index, int value);

void vm_inc(machine *m, int value);
void vm_dec(machine *m, int value);

void vm_cmp(machine *m, int r_source, int r_dest);
void vm_nop(machine *m);
void vm_jmp(machine *m, int jmpto);
void vm_hlt(machine *m);

/*
	VM Tests
	------------


*/
void test_loadi(machine *m, int argc, char *argv[]);
void test_instruction(machine *m, int argc, char *argv[]);

//Misc Helpers
int get_int(char *input);
int filter_register(int reg);