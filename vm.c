#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"


#define NUM_REGS 4
unsigned regs[ NUM_REGS ];

//Default Program 
unsigned program[] = { 0x1064, 0x11C8, 0x4201, 0xF000 };

//Program Counter
int pc = 0;

//Gets the next instruction
int fetch() {
  return program[ pc++ ];
}

//Stack 
int stack[200];
//Stack pointer
int sp = 0;

//Verbose Debugging
int debug = 0;

//Opcode Fields (todo: struct this)
int instrNum = 0;
int reg1     = 0;
int reg2     = 0;
int reg3     = 0;
int imm      = 0;



//An int is a WORD 4 0000
//Take an int and break it down
void decode( int instr ) {
  instrNum = (instr & 0xF000) >> 12;
  reg1     = (instr & 0xF00 ) >>  8;
  reg2     = (instr & 0xF0  ) >>  4;
  reg3     = (instr & 0xF   );
  imm      = (instr & 0xFF  );
}

//VM running flag
int running = 1;


//The meat and bones of the VM
void eval()
{
  switch( instrNum )
  {
    //Halt Instruction
    case INSTR_HALT:
      if(debug) {
        printf( "halt\n" );
      }
      running = 0;
      break;

    //Load Immediate Value
    case INSTR_LOADI:
      if(debug) {
        printf( "loadi r%d #%d\n", reg1, imm );
      }
      regs[ reg1 ] = imm;
      break;

    //Load Value from register into other register
    case INSTR_LOADR:
      if(debug) {
        printf("loadr r%d r%d",reg1, reg2);
      }
      regs[reg1] = regs[reg2];
      break;

    //Add the values of 2 registers and store into another
    case INSTR_ADD:
      if(debug) {
        printf( "add r%d r%d r%d\n", reg1, reg2, reg3 );
      }
      regs[ reg1 ] = regs[ reg2 ] + regs[ reg3 ];
      break;
    //Subtract the values of 2 registers and store into another
    case INSTR_SUB:
      if(debug) {
        printf( "sub r%d r%d r%d\n", reg1, reg2, reg3 );
      }
      regs[ reg1 ] = regs[ reg2 ] - regs[ reg3 ];
      break;

    //Push a Immediate value onto the stack
    case INSTR_PUSHI:
      if(debug) {
        printf("pushi #%d\n",imm);
      }
      stack[sp] = imm;
      sp++;
      break;

    //Push a value of the register onto the stack
    case INSTR_PUSHR:
      if(debug) {
        printf("pushr r%d\n",reg1);
      }
      stack[sp] = regs[reg1];
      sp++;
      break;

    //Pop a value off the stack onto the register
    case INSTR_POP:
      if(debug) {
        printf("pop r%d\n",reg1);
      }
      regs[reg1] = stack[sp - 1];
      stack[sp - 1] = 0;
      sp--;
      break;

    //Invokes a custom function
    case INSTR_INVOKE:
      //Real meat of the vm...the interupts table :)
      //This will use the stack and registers
      if(debug) {
        printf("invoke #%d\n",imm);
      }

      if(imm == INV_PRINTC) {
        //Print a Char
        int preview = regs[reg1];
        if(debug) {
          printf("%c\n",preview);
        } else {
          printf("%c", preview);
        }
      }
      //Print a String from stack
      else if(imm == INV_PRINTS) {
        int stack_p = regs[0];
        int str_size = regs[1];
        if(debug) {
          printf("PRINTS %d %d\n",stack_p,str_size);
        }
        int p;
        for(p=0;p < str_size;p++) {
          int prev = stack[stack_p + p];
          printf("%c",prev);
        }
      }
      break;

    //set the program counter to a sepcific spot
    case INSTR_JMP:
      if(debug) {
        printf("jmp r%d",reg1);
      }
      pc = regs[reg1];
      break;
  }
}

/* display all registers as 4-digit hexadecimal words */
void showRegs() {
  int i;
  for( i=0; i<NUM_REGS; i++ ) {
    printf( "r%d %04X (#%03d)\n",i, regs[ i ] ,regs[i]);
  }
  printf("SP: %d\nPC: %d\n",sp,pc);
}

//Print a preview of the stack
void showStack(int start, int size) {
  int i;
  for(i=start;i < size;i++) {
    printf("%04X: #%d\n",i,stack[i]);
  }
}

//autorun 
void run() {
  while( running )
  {
    int instr = fetch();
    decode( instr );
    eval();
    if(debug) {
      showRegs();
      showStack(0,10);
    }
  }
  if(debug) {
    printf("Debug -- Final Report --\n");
    showRegs();
    showStack(0,20);
  }
}

//Main
int _main( int argc, char * argv[] ) {
  if(argc > 1) {
    //Assume filename
    char *filename = argv[1];

    //Check for 2 params
    if(argc > 2) {
      char *opt = argv[2];
      printf("Parsing argument %s\n",opt);
      if(strstr(opt,"-d")) {
        printf("Verbose debug output enabled\n");
        debug = 1;
      }
    }
    FILE *fp;
    //fopen_s(&fp, filename,"rb");
    if(fp) {

      int data;
      while (fread(&data,4,1,fp) > 0)  {
        decode(data);
        eval();
      }
      if(debug) {
        showRegs();
        showStack(0,20);
      }
    }
    fclose(fp);
  } else {
    //No arguments
    printf("VM v0.1\n");
    printf("Usage: vm <prorgam.bin> -d<Verbose Debugging>\n");
  }
  //run();
  return 0;
}

/*

New Additions
8/30/2018
- Structure some of the data
- Seperate the code
- create an opcode struct
- easy functions for modifying it - 0.3
- enable breakpoints
- real debugging	- 0.6
- more opcodes		- 0.8
- stdlib builtin	- 0.9
- JIT core (FUTURE)
- I/O
- NET I/O (FUTURE)



//Program Counter
int pc = 0;

//Gets the next instruction
int fetch() {
return program[ pc++ ];
}

//Stack
int stack[200];
//Stack pointer
int sp = 0;

//Verbose Debugging
int debug = 0;
*/

/*
	Instruction

*/
struct instruction *vm_decode_instruction(int inst);

struct instruction {
	int inst;	//The single byte instruction (1-15)0-F
	int r1;		//Register operand 1
	int r2;		//Register operand 2
	int r3;		//Register operand 3
	int imm;	//2 byte value
};

struct vm_cpu {
	int pc;
	int sp;
	int *stack;
	short r1;
	short r2;
	short r3;
	struct instruction *code;
	int running;
};

/*
	VM Initialize
	- 0 all data and setup the stack
*/
void vm_init(struct vm_cpu *vm, int ramsize) {
	vm->pc = 0;
	vm->sp = 0;
	vm->stack = malloc(ramsize * sizeof(char));
	vm->r1 = 0;
	vm->r2 = 0;
	vm->r3 = 0;
	vm->running = 0;
	vm->code = malloc(5 * sizeof(struct instruction));
	vm->code[0] = *vm_decode_instruction(0x11FF);
	vm->code[1] = *vm_decode_instruction(0x12FF);

}

/*
	VM Display Registers
*/
void vm_register_display(struct vm_cpu *vm) {
	printf("VM Registers\n");
	printf("PC: %04X\n", vm->pc);
	printf("SP: %04X\n", vm->sp);
	printf("R1: %04X\n", vm->r1);
	printf("R2: %04X\n", vm->r2);
	printf("R3: %04X\n", vm->r3);
	printf("Running: %d", vm->running);
}

/*
	VM Decode Instruction
	Parse an int as an opcode and return an struct instruction
*/
struct instruction *vm_decode_instruction(int inst) {
	struct instruction *instr = malloc(sizeof(struct instruction));
	instr->inst = (inst & 0xF000) >> 12;
	instr->r1 = (inst & 0xF00) >> 8;
	instr->r2 = (inst & 0xF0) >> 4;
	instr->r3 = (inst & 0xF);
	instr->imm = (inst & 0xFF);
	return instr;
}

/*
	VM Display Instruction

*/
void vm_instruction_display(struct instruction *in) {
	//Sanity check
	if(in->inst > 15 || in->inst < 0)
	printf("Instruction: %01X ", in->inst);
	switch (in->inst) {
		case INSTR_LOADI:
			printf("LOADI R%d -> %04x\n",in->r1,in->imm);
			break;

		case INSTR_LOADR:
			printf("LOADR R%d -> R%d\n",in->r1,in->r2);
			break;
	}
}


//Re-write-ish
int main(int argc, char *argv[]) {
	printf("VM v0.3\n");
	struct vm_cpu machine;
	vm_init(&machine, 200);
	int code_size = sizeof(&machine.code);
	printf("Size: %d\n", code_size);
	for (int i = 0; i < code_size; i++) {
		struct instruction *op = &machine.code[i];
		vm_instruction_display(op);
	}
	//vm_register_display(&machine);

	return 0;

}
