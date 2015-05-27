#include <stdio.h>
#include <stdlib.h>
#include "reader.h"
#include "data.h"
#include "instrc.h"
#include "execute.h"
void test_printer(struct words* a[], struct words* b[]);

int main(int argc, char **argv){
    //start reading data
    struct words *ins_array[300000];
    struct words *d_array[300000];
    int e_param[12] = {0, 64, 32, 8, 16, 16, 4, 4, 16, 4, 1}, i;
    
    for(i = 1; i < argc; i ++) {
        e_param[i] = atoi(argv[i]);
    }

    image_reader(ins_array, d_array);
    data_init(d_array, e_param[2], e_param[4], e_param[8], e_param[9], e_param[10]);
    printf("start ins init\n");
    ins_init(ins_array, e_param[1], e_param[3], e_param[5], e_param[6], e_param[7]);
    printf("start execute\n");
    execute();
    //data_write(12, 0x56781234);
    //printf("i know sp too: 0x%08x, with data: %08x\n", sp, data_read(12));
    //test_printer(ins_array, d_array);
}

void test_printer(struct words* a[], struct words* b[]) {
    // for check ins_array and d_array 
    int i = 0, x = 0;
    unsigned int out = 0;
    printf("a:\n");
    while(a[i]) {
        printf("%08x", char_to_num(a[i]->machine_code));
        printf("\n");
        i ++;
    }
    i = 0;
    printf("b:\n");
    while(b[i]) {
        printf("%08x", char_to_num(b[i]->machine_code));
        printf("\n");
        i ++;
    }
}
