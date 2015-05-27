#ifndef INSTRC_H_
#define INSTRC_H_
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
struct ins* i_disk[MEMO_LIMIT];
m_unit* i_mem;
m_unit* i_cac;
m_unit* i_tlb;
m_unit* i_page;
int i_page_entry;
int i_tlb_entry;

int i_mem_size;
int i_page_size;
int i_cac_size;
int i_blk_size;
int i_set;

void ins_init(struct words *i_img[], int im_size, int ip_size, int ic_size, int ib_size, int is); 
#endif
