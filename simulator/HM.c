#include <stdio.h>
#include <stdlib.h>
#include "HM.h"

// type = 0 for i, type = 1 for d
void HM_check(int addr, int type) {
    int set;    
    m_unit* tar_page;
    m_unit* tar_mem;
    m_unit* tar_cac;
    m_unit* tar_tlb;
    
    if(type) {
        set = d_set;
        tar_page = d_page;
        tar_mem = d_mem;
        tar_cac = d_cac;
        tar_tlb = d_tlb;
    } else {
        set = i_set;
        tar_page = i_page;
        tar_mem = i_mem;
        tar_cac = i_cac;
        tar_tlb = i_tlb;
    }
}

void HM_init(int param[]) {
    int x;
    
    d_mem = malloc(sizeof(m_unit));
    d_cac = malloc(sizeof(m_unit));
    d_tlb = malloc(sizeof(m_unit));
    d_page = malloc(sizeof(m_unit));
    
    // divide 4 for turn byte to word
    d_mem->size = param[2];
    d_mem->entry = d_mem->size;
    d_page->size = param[4];
    d_page->entry = MEMO_LIMIT / param[4]; // page entry = disk_size / page_size
    d_cac->size = param[9];
    d_cac->entry = param[8] / d_cac->size; // cache entry = total cache / cache size
    d_set = param[10]; //cache association
    d_tlb->entry = d_page->entry / 4; // tlb entry = quarter of page entry 
 
    d_page->hm[HIT] = 0; d_page->hm[MISS] = 0;
    d_cac->hm[HIT] = 0; d_cac->hm[MISS] = 0;
    d_tlb->hm[HIT] = 0; d_tlb->hm[MISS] = 0;
    d_mem->hm[HIT] = 0; d_mem->hm[MISS] = 0;
   
     for(x = 0; x < MEMO_LIMIT; x ++) {
       d_mem->content[x] = 0; 
       d_mem->recency[x] = -1;
       d_cac->content[x] = 0; 
       d_cac->recency[x] = -1; 
       d_tlb->content[x] = 0; 
       d_tlb->recency[x] = -1; 
       d_page->content[x] = 0; 
       d_page->recency[x] = -1; 
    }
    
    i_mem = malloc(sizeof(m_unit)); 
    i_cac = malloc(sizeof(m_unit));
    i_tlb = malloc(sizeof(m_unit)); 
    i_page = malloc(sizeof(m_unit)); 
    
    i_mem->size = param[1];
    i_mem->entry = i_mem->size;
    i_page->size = param[3];
    i_page->entry = MEMO_LIMIT / param[3]; // page entry = disk_size / page_size
    i_cac->size = param[6];
    i_cac->entry = param[5] / i_cac->size; // cache entry = total cahche / cache size
    i_tlb->entry = i_page->entry / 4;
    i_set = param[7];
    
    i_page->hm[HIT] = 0; i_page->hm[MISS] = 0;
    i_cac->hm[HIT] = 0; i_cac->hm[MISS] = 0;
    i_tlb->hm[HIT] = 0; i_tlb->hm[MISS] = 0;
    i_mem->hm[HIT] = 0; i_mem->hm[MISS] = 0;
    
    for(x = 0; x < MEMO_LIMIT; x ++) {
       i_mem->content[x] = 0; 
       i_mem->recency[x] = -1;
       i_cac->content[x] = 0; 
       i_cac->recency[x] = -1; 
       i_tlb->content[x] = 0; 
       i_tlb->recency[x] = -1; 
       i_page->content[x] = 0; 
       i_page->recency[x] = -1; 
    }

    printf("%d %d for i.\n", i_page->entry, i_tlb->entry);
    printf("%d %d for d.\n", d_page->entry, d_tlb->entry);

}
