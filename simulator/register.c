#ifndef REGISTER_C_
#define REGISTER_C_
#include "register.h"
#include "data.h"
void reg_init() {
    int x;
    for(x = 0; x < 32; x ++) {
        reg[x] = 0x00000000;
    }
}

int reg_read(int tar) {
    return reg[tar];
}

void reg_write(int tar, int data) {
    reg[tar] = data;
}
#endif
