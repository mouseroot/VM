#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm2.h"


int fsize(char *filename) {
	FILE *fp;
	int fsize = 0;
	fopen_s(&fp, filename, "r");
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
	printf("Execute ");
	switch (inst->inst)
	{
	case INSTR_LOADI:
		printf("loadi %s %d (%04X)\n", get_register_name(inst->op1), inst->imm, machine_to_opcode(inst));
		vm_loadi(m, inst->op1, inst->imm);
		break;
	case INSTR_LOADR:
		printf("loadr %s %s (%04X)\n", get_register_name(inst->op1), get_register_name(inst->op2), machine_to_opcode(inst));
		vm_loadr(m, inst->op1, inst->op2);
		break;
	case INSTR_ADD:
		printf("add %s %d (%04X)\n", get_register_name(inst->op1), inst->imm, machine_to_opcode(inst));
		vm_add(m, inst->op1, inst->imm);
		break;
	case INSTR_ADDR:
		printf("addr %s %s (%04X)\n", get_register_name(inst->op1), get_register_name(inst->op2), machine_to_opcode(inst));
		vm_addr(m, inst->op1, inst->op2);
		break;
	case INSTR_SUB:
		printf("sub %s %d (%04X)\n", get_register_name(inst->op1), inst->imm, machine_to_opcode(inst));
		vm_sub(m, inst->op1, inst->imm);
		break;
	case INSTR_SUBR:
		printf("subr %s %s (%04X)\n", get_register_name(inst->op1), get_register_name(inst->op2), machine_to_opcode(inst));
		vm_subr(m, inst->op1, inst->op2);
		break;
	case INSTR_PUSHI:
		printf("pushi %d (%04X)\n", inst->imm, machine_to_opcode(inst));
		vm_pushi(m, inst->imm);
		break;
	case INSTR_PUSHR:
		printf("pushr %s (%04X)\n", inst->imm, get_register_name(inst->op1));
		vm_pushr(m, inst->op1);
		break;
	case INSTR_INC:
		printf("inc\n");
		break;
	case INSTR_DEC:
		printf("dec\n");
		break;
	case INSTR_CMP:
		printf("cmp %s %d (%04X)\n", get_register_name(inst->op1), inst->imm, machine_to_opcode(inst));
		vm_cmp(m, inst->op1, inst->imm);
		break;
	case INSTR_JNZ:
		printf("jnz -Not Yet Implimented\n");
		break;
	case INSTR_JMP:
		printf("jmp -Not Yet Implimented\n");
		break;
	case INSTR_CMPR:
		printf("cmpr %s %s (%04X)\n", get_register_name(inst->op1), get_register_name(inst->op2), machine_to_opcode(inst));
		vm_cmpr(m, inst->op1, inst->op2);
		break;
	case INSTR_HALT:
		printf("halt\n");
		break;

	default:
		printf("UNKNOWN\n");
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

	m->code_size = 0;
	m->stack_size = 255;
	for (int i = 0; i < m->stack_size; i++) {
		m->stack[i] = 0;
	}
}


//Displays the registers/stack/flags
void machine_display_registers(machine *m) {
	printf("R0: %04X(%d)\t",m->r0, m->r0);
	printf("R1: %04X(%d)\t", m->r1, m->r1);
	printf("R2: %04X(%d)\t", m->r2, m->r2);
	printf("R3: %04X(%d)\n", m->r3, m->r3);

	printf("PC: %04X\t", m->pc);
	printf("SP: %04X\t", m->sp);
	printf("FLAGS: \nCarry: %d\nZero: %d\nOverflow: %d\n", m->cflag, m->zflag, m->oflag);
}

//TODO: write memory display

//TODO: write stack display

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


void print_banner() {
	printf("VM2 <Register: 0-3> <value: 0-65535>\n");
	printf("VM2 -t --test <instruction> <operand> <operand> - Executes a single instruction\n");
	printf("VM2 -a --assemble - drops into assemble mode <repl assembler>\n");
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


/*
	Core IO Logic
*/

void parse_command(machine *m, char *command)
{
	//If we get more then a blank string
	if (strlen(command) > 1) {
		char *command_array[20];
		int i = 0;
		//Get input string, split by " "
		command_array[i] = strtok(command, " ");
		while (command_array[i] != NULL) {
			command_array[++i] = strtok(NULL, " ");
		}
		int size = i;


		//asm command
		if (strcmp(command_array[0], "asm") == 0) {
			char *name = command_array[1];
			int r = get_register(command_array[2]);
			int r2 = get_register(command_array[3]);
			int imm = get_int(command_array[3]);

			//LOADI
			if (strcmp(name, "loadi") == 0) {
				//vm_loadi(m, r, imm);
				int op = machine_encode(INSTR_LOADI, r, 0, imm);
				instruction *loadi = machine_decode(op); //1012 - loadi r0 0xB
				machine_add_instruction(m, *loadi);
			}

			//LOADR
			if (strcmp(name, "loadr") == 0) {
				//vm_loadr(m, r, r2);
				int op = machine_encode(INSTR_LOADR, r, r2, 0);
				instruction *loadr = machine_decode(op);
				machine_add_instruction(m, *loadr);
			}

			//ADD
			if (strcmp(name, "add") == 0) {
				//vm_add(m, r, imm);
				int op = machine_encode(INSTR_ADD, r, 0, imm);
				instruction *add = machine_decode(op);
				machine_add_instruction(m, *add);
			}

			//SUB
			if (strcmp(name, "sub") == 0) {
				//vm_sub(m, r, imm);
				int op = machine_encode(INSTR_SUB, r, 0, imm);
				instruction *sub = machine_decode(op);
				machine_add_instruction(m, *sub);
			}

			//CMP
			if (strcmp(name, "cmp") == 0) {
				//vm_cmp(m, r, imm);
				int op = machine_encode(INSTR_CMP, r, 0, imm);
				instruction *cmp = machine_decode(op);
				machine_add_instruction(m, *cmp);
			}

			//CMPR
			if (strcmp(name, "cmpr") == 0) {
				//vm_cmpr(m, r, r2);
				int op = machine_encode(INSTR_CMPR, r, r2, 0);
				instruction *cmpr = machine_decode(op);
				machine_add_instruction(m, *cmpr);
			}
		}

		//list command
		if (strcmp(command_array[0], "list\n") == 0) {
			printf("listing %d instructions\n",m->code_size);
			for (int i = 0; i < m->code_size; i++) {
				instruction *in = &m->code[i];
				if(in->imm) {
					printf("#%d\t%s %s %d\n", i, get_instruction_name(in->inst), get_register_name(in->op1), in->imm);
				}
				else {
					printf("#%d\t%s %s %s\n", i, get_instruction_name(in->inst), get_register_name(in->op1), get_register_name(in->op2));
				}
				
			}
		}


		//run command
		if (strcmp(command_array[0], "run\n") == 0) {
			/*
			for (int i = 0; i < m->code_size; i++) {
				instruction *in = &m->code[i];
				machine_execute_instruction(m,in);
				//printf("#%d\t%s %s %d\n", i, get_instruction_name(in->inst), get_register_name(in->op1), in->imm);
			}
			*/
			do {
				instruction *current = &m->stack[m->pc];
				machine_execute_instruction(m, current);
			} while (m->pc != m->stack_size);

			machine_display_registers(m);
		}

		//write command
		if (strcmp(command_array[0], "write\n") == 0) {
			printf("Writeing %d instructions (%zu Bytes)\n", m->code_size, (m->code_size * sizeof(int)));
			FILE *writeFile;
			char filename[255];
			input("File to write to: ", filename);
			writeFile = fopen("test.bin", "wb");
			if (writeFile) {
				printf("Opening %s for writing\n", "test.bin");
				for (int i = 0; i < m->code_size; ++i) {
					instruction *in = &m->code[i];
					int op = machine_to_opcode(in);
					fwrite(&op, sizeof(int), 1, writeFile);
				}
				fclose(writeFile);
			}
			else {
				printf("Error opening %s for writeing!!!\nAborting\n", filename);
			}

		}
		//load command
		if (strcmp(command_array[0], "load\n") == 0) {
			FILE *loadFile;
			loadFile = fopen("test.bin", "rb");
			if (loadFile) {
				printf("Opening test.bin for reading\n");
				int size = fsize("test.bin") / sizeof(int);
				if (size > 0) {
					for (int i = 0; i < size; i++) {
						int read = 0;
						fread(&read, sizeof(int), 1, loadFile);
						instruction *ins = machine_decode(read);
						printf("Reading %04X (%d) -> %s\n", read,read,get_instruction_name(ins->inst));
						machine_add_instruction(m, *ins);
						m->stack[i] = read;

					}
				}

			}
			else {
				printf("Could not open test.bin for reading\n");
			}
		}
		//purge command
		if (strcmp(command_array[0], "purge\n") == 0) {
			printf("Resetting code in buffer\n");
			machine_fill_instruction(m, machine_decode(0));
			for (int i = 0; i < m->code_size; i++) {
				
			}
		}
		//mem command
		if (strcmp(command_array[0], "mem\n") == 0) {
			printf("Dumping memory\n");
			int half = m->stack_size / 2;
			int full = m->stack_size;
			int part = m->stack_size / 4;
			for (int i = 0; i < full; i++) {
				printf("%04X\n", m->stack[i]);
			}
		}

		
		
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
		machine my_vm;
		machine_init(&my_vm);

		if (strstr(option, "-a") || strstr(option, "--assemble")) {
			char command[1024];
			do {
				input("Assembler> ", command);
				parse_command(&my_vm, command);
			} while (strcmp(command, "quit\n") != 0);
		}
		else if (strstr(option, "-t") || strstr(option, "--test")) {
			if (count >= 3) {
				test_instruction(&my_vm, argc, argv);
				machine_display_registers(&my_vm);
			}
		}
	}


}
