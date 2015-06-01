#ifndef DATA_H_
#define DATA_H_
#define MEMO_LIMIT 1024
int reg[33];
int sp;
int data_num;
int pc;
int ins_num;
int error_halt;
// actually only contains MEMO_LIMIT/4 instructions
typedef struct words{
    unsigned char machine_code[5];
} words;

struct data {
    int addr;
    int dat[5];
};

struct ins {
    int pc_addr; //6 bits
    int bits;
};

typedef struct m_unit {
    int entry;
    int size;
    int content[1025];
    int recency[1025];
    int c_recency[MEMO_LIMIT][MEMO_LIMIT];
    int way;
    int ppn;
    int hm[2];
    int isPTE;
} m_unit;

struct data* d_disk[1025];



void data_init(struct words *d_img[]);
int data_read(int addr, int byte);
void data_write(int addr, int data, int byte);
int char_to_num(unsigned char code[]);

#endif
