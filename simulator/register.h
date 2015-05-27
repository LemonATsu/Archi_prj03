#ifndef REGISTER_H_
#define REGISTER_H_
#include <stdio.h>
#include <stdlib.h>
void reg_init();
int reg_read(int tar);
void reg_write(int tar, int data);
#endif
