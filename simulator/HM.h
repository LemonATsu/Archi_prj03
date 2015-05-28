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
int search(int tag, int size, m_unit* tar);
int PTE_check(int entry, m_unit* tar);
void CAC_update(int addr, m_unit* cac, int set);
void TLB_update(int tag, m_unit* tlb);
void LRU_update(m_unit* tar, int size);
void PTE_update(int tag, m_unit* pte);
void HM_init(int param[]);
void HM_check(int addr, int type);

#endif
