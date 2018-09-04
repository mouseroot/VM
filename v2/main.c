#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"


void print_banner() {
	printf("VM2 <Register: 0-3> <value: 0-65535>\n");
	printf("VM2 -t --test <instruction> <operand> <operand> - Executes a single instruction\n");
	printf("VM2 -a --assemble - drops into assemble mode <repl assembler>\n");
}


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

			//HALT
			if (strcmp(name, "halt\n") == 0) {
				//vm_cmpr(m, r, r2);
				int op = machine_encode(INSTR_HALT, 0, 0, 0);
				instruction *halt = machine_decode(op);
				machine_add_instruction(m, *halt);
				m->stack[m->wc] = 61440;
				m->wc++;
			}

			if (size > 2) {
				int r = get_register(command_array[2]);
				int r2 = get_register(command_array[3]);
				int imm = get_int(command_array[3]);

				//LOADI
				if (strcmp(name, "loadi") == 0) {
					//vm_loadi(m, r, imm);
					int op = machine_encode(INSTR_LOADI, r, 0, imm);
					instruction *loadi = machine_decode(op); //1012 - loadi r0 0xB
					machine_add_instruction(m, *loadi);
					m->stack[m->wc] = op;
					m->wc++;

				}

				//LOADR
				if (strcmp(name, "loadr") == 0) {
					//vm_loadr(m, r, r2);
					int op = machine_encode(INSTR_LOADR, r, r2, 0);
					instruction *loadr = machine_decode(op);
					machine_add_instruction(m, *loadr);
					m->stack[m->wc] = op;
					m->wc++;
				}

				//ADD
				if (strcmp(name, "add") == 0) {
					//vm_add(m, r, imm);
					int op = machine_encode(INSTR_ADD, r, 0, imm);
					instruction *add = machine_decode(op);
					machine_add_instruction(m, *add);
					m->stack[m->wc] = op;
					m->wc++;
				}
				//ADDR
				if (strcmp(name, "addr") == 0) {
					//vm_add(m, r, imm);
					int op = machine_encode(INSTR_ADDR, r, r2, 0);
					instruction *addr = machine_decode(op);
					machine_add_instruction(m, *addr);
					m->stack[m->wc] = op;
					m->wc++;
				}

				//SUB
				if (strcmp(name, "sub") == 0) {
					//vm_sub(m, r, imm);
					int op = machine_encode(INSTR_SUB, r, 0, imm);
					instruction *sub = machine_decode(op);
					machine_add_instruction(m, *sub);
					m->stack[m->wc] = op;
					m->wc++;
				}

				//SUBR
				if (strcmp(name, "subr") == 0) {
					//vm_sub(m, r, imm);
					int op = machine_encode(INSTR_SUBR, r, r2, 0);
					instruction *subr = machine_decode(op);
					machine_add_instruction(m, *subr);
					m->stack[m->wc] = op;
					m->wc++;
				}
				//CMP
				if (strcmp(name, "cmp") == 0) {
					//vm_cmp(m, r, imm);
					int op = machine_encode(INSTR_CMP, r, 0, imm);
					instruction *cmp = machine_decode(op);
					machine_add_instruction(m, *cmp);
					m->stack[m->wc] = op;
					m->wc++;
				}

				//CMPR
				if (strcmp(name, "cmpr") == 0) {
					//vm_cmpr(m, r, r2);
					int op = machine_encode(INSTR_CMPR, r, r2, 0);
					instruction *cmpr = machine_decode(op);
					machine_add_instruction(m, *cmpr);
					m->stack[m->wc] = op;
					m->wc++;
				}
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
					if (in->inst == INSTR_HALT) {
						printf("#%d\t%s\n", i, get_instruction_name(in->inst));
					}
					else {
						printf("#%d\t%s %s %s\n", i, get_instruction_name(in->inst), get_register_name(in->op1), get_register_name(in->op2));
					}
				}
				
			}
		}


		//run command
		if (strcmp(command_array[0], "run\n") == 0) {
			m->pc = 0;
			m->sp = 0;
			m->r0 = 0;
			m->r1 = 0;
			m->r2 = 0;
			m->r3 = 0;
			for(int i=0;i < m->stack_size;i++) {
				instruction *current = machine_decode(m->stack[m->pc]);
				machine_execute_instruction(m, current);
				m->pc++;
			};

			machine_display_registers(m);
		}

		//write command
		if (strcmp(command_array[0], "write\n") == 0) {
			printf("Writeing %d instructions (%zu Bytes)\n", m->code_size, (m->code_size * sizeof(int)));
			FILE *writeFile;
			char filename[255];
			input("File to write to: ", filename);
			char *pos = strchr(filename, '\n');
			*pos = 0;
			writeFile = fopen(filename, "wb");
			if (writeFile) {
				printf("Opening %s for writing\n", filename);
				for (int i = 0; i < m->code_size; ++i) {
					instruction *in = &m->code[i];
					int op = machine_to_opcode(in);
					fwrite(&op, sizeof(int), 1, writeFile);
					printf("writeing %04X\n", op);
					m->wc++;
				}
				fclose(writeFile);
			}
			else {
				printf("Error opening %s for writeing!!\n",filename);
			}

		}
		//load command
		if (strcmp(command_array[0], "load\n") == 0) {
			FILE *loadFile;
			char filename[255];
			input("File to open: ", filename);
			char *pos = strchr(filename, '\n');
			*pos = 0;
			loadFile = fopen(filename, "rb");
			if (loadFile) {
				printf("Opening %s for reading\n",filename);
				int size = fsize("test.bin") / sizeof(int);
				if (size > 0) {
					for (int i = 0; i < size; i++) {
						int read = 0;
						fread(&read, sizeof(int), 1, loadFile);
						instruction *ins = machine_decode(read);
						printf("Reading %04X (%d) -> %s\n", read,read,get_instruction_name(ins->inst));
						machine_add_instruction(m, *ins);
						m->stack[m->wc + i] = read;
						m->wc++;

					}
				}

			}
			else {
				printf("Could not open %s for reading\n",filename);
			}
		}
		//purge command
		if (strcmp(command_array[0], "purge\n") == 0) {
			printf("Resetting code in buffer\n");
			machine_fill_instruction(m, machine_decode(0));
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
