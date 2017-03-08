#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"


#define NUM_REGS 4
unsigned regs[ NUM_REGS ];

unsigned program[] = { 0x1064, 0x11C8, 0x4201, 0xF000 };


int pc = 0;


int fetch() {
  return program[ pc++ ];
}

int stack[200];
int sp = 0;

int debug = 0;

int instrNum = 0;
int reg1     = 0;
int reg2     = 0;
int reg3     = 0;
int imm      = 0;


void decode( int instr ) {
  instrNum = (instr & 0xF000) >> 12;
  reg1     = (instr & 0xF00 ) >>  8;
  reg2     = (instr & 0xF0  ) >>  4;
  reg3     = (instr & 0xF   );
  imm      = (instr & 0xFF  );
}


int running = 1;


void eval()
{
  switch( instrNum )
  {
    case INSTR_HALT:
      if(debug) {
        printf( "halt\n" );
      }
      running = 0;
      break;

    case INSTR_LOADI:
      if(debug) {
        printf( "loadi r%d #%d\n", reg1, imm );
      }
      regs[ reg1 ] = imm;
      break;

    case INSTR_LOADR:
      if(debug) {
        printf("loadr r%d r%d",reg1, reg2);
      }
      regs[reg1] = regs[reg2];
      break;

    case INSTR_ADD:
      if(debug) {
        printf( "add r%d r%d r%d\n", reg1, reg2, reg3 );
      }
      regs[ reg1 ] = regs[ reg2 ] + regs[ reg3 ];
      break;

    case INSTR_SUB:
      if(debug) {
        printf( "sub r%d r%d r%d\n", reg1, reg2, reg3 );
      }
      regs[ reg1 ] = regs[ reg2 ] - regs[ reg3 ];
      break;

    case INSTR_PUSHI:
      if(debug) {
        printf("pushi #%d\n",imm);
      }
      stack[sp] = imm;
      sp++;
      break;

    case INSTR_PUSHR:
      if(debug) {
        printf("pushr r%d\n",reg1);
      }
      stack[sp] = regs[reg1];
      sp++;
      break;

    case INSTR_POP:
      if(debug) {
        printf("pop r%d\n",reg1);
      }
      regs[reg1] = stack[sp - 1];
      stack[sp - 1] = 0;
      sp--;
      break;

    
    case INSTR_INVOKE:
      //Real meat of the vm...the interupts table :)
      //This will use the stack adn registers
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

void showStack(int start, int size) {
  int i;
  for(i=start;i < size;i++) {
    printf("%04X: #%d\n",i,stack[i]);
  }
}

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

int main( int argc, char * argv[] ) {
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
    //printf("Opening %s\n",filename);
    FILE *fp;
    fp = fopen(filename,"rb");
    if(fp) {
      /*
      int data;
      fread(&data, 2, 1, fp);
      printf("Result %04X\n",data);
      //Psuedo run
      decode(data);
      eval();
      showRegs();
      */
      int data;
      while (fread(&data,4,1,fp) > 0)  {
        //printf("%04X\n",data);
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
    printf("vm <prorgam.bin> -d<Verbose Debugging>");
  }
  //run();
  return 0;
}
