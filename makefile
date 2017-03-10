vm: vm.c
	@echo Compiling Virtual Machine
	gcc vm.c -o vm

vmasm: vmasm.c
	@echo Compiling Virtual Machine Assembler
	gcc vmasm.c -o vmasm

clean:
	@echo Removing binaries and bin files
	$(RM) vm vmasm *.bin

install:
	@echo Installing VM and VMASM
