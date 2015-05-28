#ifndef HM_H_
#define HM_H_
#include <stdio.h>
#include <stdlib.h>
#include "instrc.h"
#define HIT 1
#define MISS 0
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
int d_pn;
int d_cur_pn[MEMO_LIMIT];
int d_pn_c;
int dc_recency[MEMO_LIMIT][MEMO_LIMIT];
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
int i_pn;
int i_cur_pn[MEMO_LIMIT];
int i_pn_c;
int ic_recency[MEMO_LIMIT][MEMO_LIMIT];

int CLRU_search(m_unit* cac, int addr);
void CLRU_insert(m_unit* cac, int addr);
void CLRU_update(m_unit* cac, int from, int addr);

int CAC_search(m_unit* cac, int addr);
int TLB_search(m_unit* tlb, int tag);
int PTE_search(m_unit* pte, int tag);
void LRU_insert(m_unit* tar, int size, int tag);
void LRU_update(m_unit* tar, int from, int size, int tag);
int LRU_search(m_unit* tar, int tag);

void report(FILE* fptr_report);

#endif
