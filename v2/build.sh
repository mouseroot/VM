#!/bin/sh
echo Building
gcc main.c misc.c machine.c test.c vm.c -o vm_build
