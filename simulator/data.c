#include "data.h"
#include <stdlib.h>
#include <stdio.h>
#include "HM.h"
//address < 1024 -> have 256 words at most
//unsigned int sp;
//unsigned int data_num;
// global data has been put in obj.h


//short int data_read_half(int addr);
//short int data_read_byte(int addr);
//void data_write_byte(int addr, int data);

void data_init(struct words *d_img[]) {
    sp = char_to_num(d_img[0]->machine_code);
    data_num = char_to_num(d_img[1]->machine_code);

    int x, y = 2 + data_num;

    for(x = 2; x < y; x++) {
        //save data into D memory, details are still unsure
        //printf("data: 0x%08x\n", char_to_num(d_img[x]->machine_code));
        struct data* read_dat = malloc(sizeof(struct data));
        d_disk[x - 2] = read_dat;
        if(!d_img[x]) {
            printf("fatal error! num of data in D memory doesn't match!\n");
        }

        int temp = char_to_num(d_img[x]->machine_code), y;
        for(y = 0; y < 4; y ++) {
            read_dat->dat[y] = (temp >> (8 * (3 - y))) & 0x000000ff;
        }
        read_dat->addr = (x - 2) * 4;
    }

   // initialize to zero
   x = MEMO_LIMIT / 4;
   for(y = y - 2; y < x; y ++) {
        int z;
        struct data* init_dat = malloc(sizeof(struct data));
        d_disk[y] = init_dat;
        for(z = 0; z < 4; z ++) {
            init_dat->dat[z] = 0x00000000;
        }
        init_dat->addr = y * 4;
   }
}

int data_read(int addr, int byte) {
    int result = 0, tar, fix = 3 - (4 - byte), out = 0;
    struct data* d_read;

    if(addr == 0) {
        tar = 0;
    }  else {
        tar = addr / 4;
    }

    d_read = d_disk[tar];

    if(d_read) {
        int x = addr % 4, c;
        for(c = 0; c < byte; c ++) {
            result += d_read->dat[x + c] << (8 * ((byte - 1) - c));
        }
    }

    return result;
}



void data_write(int addr, int data, int byte){
    int out = 0;
    struct data* d_write;
    unsigned int tar;
 
    if(addr == 0) {
        tar = 0;
    } else {
        tar = addr / 4;
    }

    d_write = d_disk[tar];

    if(d_write) {
        int c;
        int x = addr % 4;
        for(c = 0; c < byte; c ++) {
            d_write->dat[x + c] = (data >> (8 * ((byte - 1) - c))) & 0x000000ff;
        }
    }
    else {
        //error handle
        printf("error: D memory out of bound(write)\n");
    }
}



int char_to_num(unsigned char code[]) {
    unsigned int result = 0;
    int x = 0;
    for(x = 0; x < 4; x++) {
        int i = code[x];
        //each char is 8bits, shift 8;
        result += (i << ((3 - x) * 8));
    }
    return result;
}

int sign_extend(int data, int sign_bit) {
    int result;
    if((data >> (sign_bit - 1) & 0x1) == 0x1) {
        if(sign_bit == 16) result = data | 0xffff0000;
        else if(sign_bit == 8) result = data | 0xffffff00;
    } else {
        if(sign_bit == 16) result = data | 0x00000000;
        else if(sign_bit == 8) result = data | 0x00000000;
    }
    return result;
}
