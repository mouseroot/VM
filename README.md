# VM
Virtual Machine and Assembler in C

Build the virtual machine
```
make vm
```
Build the Assembler
```
make vmasm
```

# VM Specs
4bit Instructions eg: 0x1064

100 byte stack

4 Registers r0 r1 r2 r3

Use # for immediate values eg: Real numbers

Use @ for memory address(stack space)

# Instructions
```
Loadi - Load immediate value
Example: loadi r0 #100

Loadr - Load value of register into another register
Example: loadr r0 r1

add - add the values of 2 registers
Example: add r0 r1

sub - subract the values of 2 registers and return it
Example: sub r0 r1

jmp - jmp to an immediate value
Example: jmp #5

pushi - push immediate value onto stack
Example: pushi #5

pushr - push the value of a register onto the stack
Example: pushr r0

pop   - takes the last value of the stack and stores it in a register
Example: pop r0

invoke - runs a built-in function
Example: invoke #2

halt - stops vm execution
Example: halt
```

# Assembling a prorgam
```
vmasm source.asm output.bin
```
# Running a program
```
vm output.bin
```
# Example programs
Very simple Adder
```
loadi r0 #100
loadi r1 #200
add r2 r0 r1
halt
```
