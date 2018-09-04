#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"

/*
	Actual machine calls
	running these returns nothing(void)
*/

void vm_cmp(machine *m, int r_source, int val) {
	int ival = 0;
	switch (r_source) {
		case R0:
			ival = (val - m->r0);
			break;
		case R1:
			ival = (val - m->r1);
			break;
		case R2:
			ival = (val - m->r2);
			break;
		case R3:
			ival = (val - m->r3);
			break;
		case SP:
			ival = (val - m->sp);
			break;
	}
	printf("CMP %d\n", ival);
}

void vm_cmpr(machine *m, int r_source, int r_dest) {
	int isrc = 0;
	int idst = 0;

	switch (r_dest) {
		case R0:
			idst = m->r0;
			break;
		case R1:
			idst = m->r1;
			break;
		case R2:
			idst = m->r2;
			break;
		case R3:
			idst = m->r3;
			break;
		}


	switch (r_source) {
		case R0:
			isrc = m->r0;
			break;
		case R1:
			isrc = m->r1;
			break;
		case R2:
			isrc =  m->r2;
			break;
		case R3:
			isrc =  m->r3;
			break;
	}
	if ((isrc - idst) == 0) {
		m->zflag = 1;
	}
	if ((isrc - idst) <= -1) {
		m->cflag = 1;
		m->oflag = 1;
	}
	if ((isrc - idst) > 255) {
		m->oflag = 1;
	}
	

}


void vm_nop(machine *m) {
	m->pc++;
}

void vm_jmp(machine *m, int jmpto) {
	int imm = (jmpto & 0xFF);
	m->pc = imm;
}

void vm_inc(machine *m, int r_index) {
	switch (r_index) {
	case R0:
		m->r0++;
		break;
	case R1:
		m->r1++;
		break;
	case R2:
		m->r2++;
		break;
	case R3:
		m->r3++;
		break;
	}
}

void vm_dec(machine *m, int r_index) {
	switch (r_index) {
	case R0:
		m->r0--;
		break;
	case R1:
		m->r1--;
		break;
	case R2:
		m->r2--;
		break;
	case R3:
		m->r3--;
		break;
	}
}

void vm_addr(machine *m, int r_dest, int r_src) {
	int rval = get_register_value(m,r_dest);
	int sval = get_register_value(m,r_src);
	int result = rval + sval;
	switch (r_dest) {
	case R0:
		m->r0 = result;
		break;
	case R1:
		m->r1 = result;
		break;
	case R2:
		m->r2 = result;
		break;
	case R3:
		m->r3 = result;
		break;
	case SP:
		m->sp = result;
		break;
	}
}

void vm_subr(machine *m, int r_dest, int r_src) {
	int rval = get_register_value(m, r_dest);
	int sval = get_register_value(m, r_src);
	int result = rval - sval;
	switch (r_dest) {
	case R0:
		m->r0 = result;
		break;
	case R1:
		m->r1 = result;
		break;
	case R2:
		m->r2 = result;
		break;
	case R3:
		m->r3 = result;
		break;
	case SP:
		m->sp = result;
		break;
	}
}

void vm_add(machine *m, int r_dest, int value) {
	int imm = (value & 0xFF);
	switch (r_dest) {
	case R0:
		m->r0 = (m->r0 + imm);
		break;
	case R1:
		m->r1 = (m->r1 + imm);
		break;
	case R2:
		m->r2 = (m->r2 + imm);
		break;
	case R3:
		m->r3 = (m->r3 + imm);
		break;
	}
}

void vm_sub(machine *m, int r_dest, int value) {
	int imm = (value & 0xFF);
	switch (r_dest) {
	case R0:
		m->r0 = (m->r0 - imm);
		break;
	case R1:
		m->r1 = (m->r1 - imm);
		break;
	case R2:
		m->r2 = (m->r2 - imm);
		break;
	case R3:
		m->r3 = (m->r3 - imm);
		break;
	}
}

void vm_loadr(machine *m, int r_source, int r_dest) {
	int isrc = get_register_value(m,r_source);

	switch (r_dest) {

	case R0:
		m->r0 = isrc;
		break;

	case R1:
		m->r1 = isrc;
		break;

	case R2:
		m->r2 = isrc;
		break;

	case R3:
		m->r3 = isrc;
		break;

	default:
		return;
		break;
	}
}

void vm_pushi(machine *m, int value) {


}

void vm_pushr(machine *m, int r_index) {

}

void vm_loadi(machine *m, int r_index, int value) {
	int imm = (value & 0xFF);
	switch (r_index) {
	case R0:
		m->r0 = imm;
		break;

	case R1:
		m->r1 = imm;
		break;

	case R2:
		m->r2 = imm;
		break;

	case R3:
		m->r3 = imm;
		break;

	case SP:
		m->sp = imm;
		break;

	case PC:
		m->pc = imm;
		break;

	default:
		printf("Invalid Register r%d\n", r_index);
		break;
	}
}