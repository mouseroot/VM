//Instructions
#define INSTR_LOADI 1
#define INSTR_LOADR 2
#define INSTR_LOADM 3
#define INSTR_ADD   4
#define INSTR_SUB   5
#define INSTR_JMP   6
#define INSTR_PUSHI 7
#define INSTR_PUSHR 8
#define INSTR_INVOKE 9
#define INSTR_POP 10
#define INSTR_HALT 15

//Invoke functions

//Print Char
//Prints the Char in Register 0
// loadr r0 #35 ; '#'
// invoke #1
#define INV_PRINTC 1

//Print String
//Prints the String in the Stack pointed at by Register 0
//Size is Register 1
#define INV_PRINTS 2
