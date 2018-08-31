#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"


unsigned int buildOp(int instr, int reg1, int reg2, int reg3, int imm) {
    int op = instr << 12;
    if(imm != 0) {
        op = op | (imm & 0xFF);
        op = op | reg1 << 8;
    } else {
        op = op | reg1 << 8;
        op = op | reg2 << 4;
        op = op | (reg3 & 0xF);
    }
    //printf("OP: %04X\n",op);
    return op;
}

int parseLine(char *line) {
  //Parse Line (Meat & Bones of the assembler)

  //Ignore Spaces
  if(strlen(line) <= 1) {
      return -1;
  }
  //Ignore Comments
  if(strstr(line,";")) {
      return -1;
  }
  //Ignore Assembler Directives .'s
  if(strstr(line,".")) {
      return -1;
  }
  printf("PARSING %s",line);
  char *p;
  char *next;
  int counter = 0;
  int inst = 0;
  int r1 = 0;
  int r2 = 0;
  int r3 = 0;
  int imm = 0;
  while ((p = strtok_s(next, " ", &next)) != NULL)
  {
    if(counter == 0) {

        //LOADI
        if(strstr(p,"loadi")) {
            inst = INSTR_LOADI;
        }

        //LOADR
        else if(strstr(p,"loadr")) {
            inst = INSTR_LOADR;
        }

        //ADD
        else if(strstr(p,"add")) {
            inst = INSTR_ADD;
        }

        //SUB
        else if(strstr(p,"sub")) {
            inst = INSTR_SUB;
        }

        //INVOKE
        else if(strstr(p,"invoke")) {
            inst = INSTR_INVOKE;
        }

        //JMP
        else if(strstr(p,"jmp")) {
            inst = INSTR_JMP;
        }

        //HALT
        else if(strstr(p,"halt")) {
            inst = INSTR_HALT;
        }

        //PUSHI
        else if(strstr(p,"pushi")) {
            inst = INSTR_PUSHI;
        }
        //PUSHR
        //POP
        else if(strstr(p,"pop")) {
            inst = INSTR_POP;
        }
    }
    else if(counter == 1) {
        if(strstr(p,"r0")) {
            r1 = 0;
        }
        else if(strstr(p,"r1")) {
            r1 = 1;
        }
        else if(strstr(p,"r2")) {
            r1 = 2;
        }
        else if(strstr(p,"r3")) {
            r1 = 3;
        }
        else {
            if(strstr(p,"#") || strstr(p,"@")) {
                *p++;
                int result = atoi(p);
                imm = result;
            }
        }
    }
    else if(counter == 2) {
        if(strstr(p,"r0")) {
            r2 = 0;
        }
        else if(strstr(p,"r1")) {
            r2 = 1;
        }
        else if(strstr(p,"r2")) {
            r2 = 2;
        }
        else if(strstr(p,"r3")) {
            r2 = 3;
        }
        else {
            if(strstr(p,"#") || strstr(p,"@")) {
                *p++;
                int result = atoi(p);
                imm = result;
            }
        }
    }
    else if(counter == 3) {
        if(strstr(p,"r0")) {
            r3 = 0;
        }
        else if(strstr(p,"r1")) {
            r3 = 1;
        }
        else if(strstr(p,"r2")) {
            r3 = 2;
        }
        else if(strstr(p,"r3")) {
            r3 = 3;
        }
    }
    //p = strtok (NULL, " ");
    counter++;
  }
  //Gernate Opcode
  int generated = buildOp(inst,r1,r2,r3,imm);
  printf("GENERATED OPCODE: %04X\n\n",generated);
  return generated;
}

int main(int argc, char *argv[]) {
    printf("VM Assembler\n");
    if(argc > 1) {
        FILE *fp;
        int compiled_program[100];
        int write_counter = 0;
        int ic = 0;
        //Assume filename
        char *filename = argv[1];
        //Assume outputname
        char *outputname = argv[2];
        printf("Opening %s...\n", filename);
        fopen_s(&fp, filename,"r");

        if(fp) {
            const size_t line_size = 300;
            char* line = malloc(line_size);
            while (fgets(line, line_size, fp) != NULL)  {
                unsigned int genop = parseLine(line);
                if(genop != -1) {
                    compiled_program[ic] = genop;
                    ic++;
                }
            }
            printf("Assembled %d Opcodes\n", ic);
            int ii;
            for(ii=0;ii < ic;ii++) {
                printf("%04X ",compiled_program[ii]);
            }
            printf("\n");
            //Now to write to an output file XD
            FILE *outFile = NULL;
            fopen_s(&outFile, outputname,"wb+");
            if(outFile) {
                printf("Writing to %s\n",outputname);
                for(write_counter=0;write_counter < ic;write_counter++) {
                    int byte2Write = compiled_program[write_counter];
                    //Split into actual bytes?
                    //int p0 = (byte2Write & 0xFF00) >> 8;
                    
                    //int p1 = byte2Write & 0x00FF;
                    
                    //printf("Writing %02X (%d)\n",p0,p0);
                    //printf("Writing %02X (%d)\n\n",p1,p1);

                    //fwrite((const void*)&p0, 1, 1, outFile);
                    //fwrite((const void*)&p1, 1, 1, outFile);
                    printf("Writing %04X\n",byte2Write);
                    fwrite(&byte2Write,4,1,outFile);
                }
                fclose(outFile);
                printf("Write Complete\n");
            }

        }
        else {
            printf("Could not open %s\n", filename);
        }
        fclose(fp);
    } else {
        //No arguments
        printf("vmasm <file.asm> <output.bin>\n");
    }

    return 0;
}
