#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm2.h"


void test_instruction(machine *m, int argc, char *argv[]) {
	if (argc > 1) {
		int count = (argc - 1);
		if (count >= 4) {
			char *name = argv[2];
			int r1 = filter_register(get_int(argv[3]));

			if (strstr("loadi", name)) {
				
				int rval = get_int(argv[4]);
				vm_loadi(m, r1, rval);
			}
			else if (strstr("loadr", name)) {
				int r2 = filter_register(get_int(argv[4]));
				vm_loadr(m, r1, r2);
			}
			else if (strstr("add", name)) {
				int rval = get_int(argv[4]);
				vm_add(m, r1, rval);
			}
			else if (strstr("sub", name)) {
				int rval = get_int(argv[4]);
				vm_sub(m, r1, rval);
			}
			else if (strstr("inc", name)) {
				vm_inc(m, r1);
			}
			else if (strstr("dec", name)) {
				vm_dec(m, r1);
			}
			else if (strstr("cmp", name)) {
				int r2 = filter_register(get_int(argv[4]));
				vm_cmp(m, r1, r2);
			}
			else if (strstr("nop", name)) {

			}
			else if (strstr("jmp", name)) {
				vm_jmp(m, r1);
			}
			else if (strstr("hlt", name)) {

			}
			else {
				printf("Invalid Test\n");
			}

		}
	}
}


//Testing loadi from the cmdline
void test_loadi(machine *m, int argc, char *argv[]) {
	if (argc > 1) {
		int count = (argc - 1);
		if (count == 2) {
			int r1 = filter_register(get_int(argv[1]));
			int rval = get_int(argv[2]);
			vm_loadi(m, r1, rval);
		}
	}
}

