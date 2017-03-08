vm: vm.c
	@echo  Compiling Virtual Machine
	gcc vm.c -o vm.exe

vmasm: vmasm.c
	@echo  Compiling Virtual Machine Assembler
	gcc vmasm.c -o vmasm.exe

clean:
	@echo  Removing binaries and bin files
	$(RM) vm.exe vmasm.exe *.bin
