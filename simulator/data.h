#ifndef DATA_H_
#define DATA_H_
#define MEMO_LIMIT 1024
#define HIT 1
#define MISS 0

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
    int content[1025];
    int recency[1025];
    int LRU;
    int LRU_num;
    int hm[2];
} m_unit;

struct data* d_disk[1025];

m_unit* d_mem;
m_unit* d_cac;
m_unit* d_tlb;
m_unit* d_page;

int d_page_entry;
int d_tlb_entry;

int d_mem_size;
int d_page_size;
int d_cac_size;
int d_blk_size;
int d_set;


void data_init(struct words *d_img[], int dm_size, int dp_size, int dc_size, int db_size, int d_s);
int data_read(int addr, int byte);
void data_write(int addr, int data, int byte);
int char_to_num(unsigned char code[]);

#endif
