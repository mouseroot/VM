#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm2.h"

void vm_cmp(machine *m, int r_source, int val) {
	int ival = 0;
	switch (r_source) {
		case 0:
			ival = (val - m->r0);
			break;
		case 1:
			ival = (val - m->r1);
			break;
		case 2:
			ival = (val - m->r2);
			break;
		case 3:
			ival = (val - m->r3);
			break;
	}
	printf("CMP %d\n", ival);
}

void vm_cmpr(machine *m, int r_source, int r_dest) {
	int isrc = 0;
	int idst = 0;

	switch (r_dest) {
		case 0:
			idst = m->r0;
			break;
		case 1:
			idst = m->r1;
			break;
		case 2:
			idst = m->r2;
			break;
		case 3:
			idst = m->r3;
			break;
		}


	switch (r_source) {
		case 0:
			isrc = m->r0;
			break;
		case 1:
			isrc = m->r1;
			break;
		case 2:
			isrc =  m->r2;
			break;
		case 3:
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
	case 0:
		m->r0++;
		break;
	case 1:
		m->r1++;
		break;
	case 2:
		m->r2++;
		break;
	case 3:
		m->r3++;
		break;
	}
}

void vm_dec(machine *m, int r_index) {
	switch (r_index) {
	case 0:
		m->r0--;
		break;
	case 1:
		m->r1--;
		break;
	case 2:
		m->r2--;
		break;
	case 3:
		m->r3--;
		break;
	}
}

void vm_add(machine *m, int r_dest, int value) {
	int imm = (value & 0xFF);
	switch (r_dest) {
	case 0:
		m->r0 = (m->r0 + imm);
		break;
	case 1:
		m->r1 = (m->r1 + imm);
		break;
	case 2:
		m->r2 = (m->r2 + imm);
		break;
	case 3:
		m->r3 = (m->r3 + imm);
		break;
	}
}

void vm_sub(machine *m, int r_dest, int value) {
	int imm = (value & 0xFF);
	switch (r_dest) {
	case 0:
		m->r0 = (m->r0 - imm);
		break;
	case 1:
		m->r1 = (m->r1 - imm);
		break;
	case 2:
		m->r2 = (m->r2 - imm);
		break;
	case 3:
		m->r3 = (m->r3 - imm);
		break;
	}
}

void vm_loadr(machine *m, int r_source, int r_dest) {

	int rval = 0;
	switch (r_source) {

	case 0:
		rval = m->r0;
		break;

	case 1:
		rval = m->r1;
		break;

	case 2:
		rval = m->r2;
		break;

	case 3:
		rval = m->r3;
		break;

	default:
		return;
		break;
	}

	switch (r_dest) {
	case 0:
		m->r0 = rval;
		break;
	case 1:
		m->r1 = rval;
		break;
	case 2:
		m->r2 = rval;
		break;
	case 3:
		m->r3 = rval;
		break;
	}
}

void vm_loadi(machine *m, int r_index, int value) {
	int imm = (value & 0xFF);
	switch (r_index) {
	case 0:
		m->r0 = imm;
		break;

	case 1:
		m->r1 = imm;
		break;

	case 2:
		m->r2 = imm;
		break;

	case 3:
		m->r3 = imm;
		break;

	default:
		printf("Invalid Register r%d\n", r_index);
		break;
	}
}