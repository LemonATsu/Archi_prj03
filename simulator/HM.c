#include <stdio.h>
#include <stdlib.h>
#include "HM.h"

int cur_type;
int cur_ppn;
// type = 0 for i, type = 1 for d
void HM_check(int addr, int type) {
    int set;    
    m_unit* tar_page;
    m_unit* tar_mem;
    m_unit* tar_cac;
    m_unit* tar_tlb;
    
    cur_type = type;

    if(type) {
        //printf("***********D type **********\n");
        set = d_set;
        tar_page = d_page;
        tar_mem = d_mem;
        tar_cac = d_cac;
        tar_tlb = d_tlb;
        cur_ppn = d_pn;
    } else {
        set = i_set;
        tar_page = i_page;
        tar_mem = i_mem;
        tar_cac = i_cac;
        tar_tlb = i_tlb;
        cur_ppn = i_pn;
    }
    int tag = addr / tar_page->size; // each tlb can link to a  page size of words
    int tlb_res = TLB_search(tar_tlb, tag);
    if(tlb_res) {
        // TLB HIT, check CACHE
        tar_tlb->hm[HIT] ++;
        //if(type) printf("TLB HIT : %d, tag : %d ---------------HIT# %d\n", addr, tag, tar_tlb->hm[HIT]);
        int update = LRU_search(tar_tlb, tag); // search the hit index
        // fuck update
        LRU_update(tar_tlb, update, tar_tlb->entry, tag); // update the recency
        int trans = (tar_page->content[tag] * tar_page->size) | (addr % tar_page->size);
        
       // if(type) printf("addr: %d, ppn %d, trans %d, entry %d\n", addr, tar_page->content[tag], trans, trans / tar_cac->size % tar_cac->entry);
        
        int cac_res = CAC_search(tar_cac, trans); // check Cache
        if(cac_res) {
            // hit, update recency
            tar_cac->hm[HIT]++;
            //if(type)printf("-----------CACHE HIT %d\n",addr);
            update = CLRU_search(tar_cac, trans);
            CLRU_update(tar_cac, update, trans);
        }
        else {
            // miss, update CACHE
            // why not check memory?
            // if item isn't in memory, it's impossible that
            // TLB hit. So we are sure that it's already in memory. 
            // update Cache directly
            //if(type)printf("-----------CACHE MISS %d\n",addr);
            tar_cac->hm[MISS] ++;
            CLRU_insert(tar_cac, trans);
        }
        // fuck update
        // update page(memory)
        //update = LRU_search(tar_page, tag);
        //LRU_update(tar_page, update, tar_page->entry, tag); // update page
    } else {
        //if(type) printf("TLB MISS : %d, tag : %d\n", addr, tag);
        // TLB MISS
        tar_tlb->hm[MISS] ++;
        int pte_res = PTE_search(tar_page, tag);
        if(pte_res) {
            // PTE HIT
            tar_page->hm[HIT] ++;
            int update = LRU_search(tar_page, tag);
            LRU_update(tar_page, update, tar_page->entry, tag); // update page
            LRU_insert(tar_tlb, tar_tlb->entry, tag); // insert page into TLB

            int trans = (tar_page->content[tag] * tar_page->size) | (addr % tar_page->size);
        //    if(type) printf("addr: %d, ppn %d, trans %d\n", addr, tar_page->content[tag], trans);

            int cac_res = CAC_search(tar_cac, trans);
            if(cac_res) {
                // update cache recency
                tar_cac->hm[HIT]++;
                update = CLRU_search(tar_cac, trans);
                CLRU_update(tar_cac, update, trans);
          //      if(type)printf("-----------CACHE HIT %d\n",addr);
            }
            else {
            //    if(type)printf("-----------CACHE MISS %d\n",addr);
                // update cache
                tar_cac->hm[MISS] ++;
                CLRU_insert(tar_cac, trans);
                //LRU_update(tar_page, update, tar_page->entry, tag); // update page
            }
        } else {
            // if TLB, PTE both miss, cache must be miss.
            // update PTE, TLB and Cache.
            tar_page->hm[MISS] ++;
            tar_cac->hm[MISS] ++;
            LRU_insert(tar_page, tar_page->entry, tag);
            TLB_invalid(tar_tlb, tar_page);
            LRU_insert(tar_tlb, tar_tlb->entry, tag);
            
            int trans = (tar_page->content[tag] * tar_page->size) | (addr % tar_page->size);
            //if(type) printf("addr: %d, ppn %d, trans %d, entry %d\n", addr, tar_page->content[tag], trans, trans / tar_cac->size % tar_cac->entry);
            CLRU_insert(tar_cac, trans);
        }
    }
/*
    if(type) {
        int q, c;
        printf("In cac\n");
        for(q = 0; q < tar_cac->entry; q ++) {
            printf("%d %d entry\n", tar_cac->c_recency[q][0], tar_cac->c_recency[q][0] / 4 % 4);
        }
    }*/
}


void TLB_invalid(m_unit* tlb, m_unit* pte) {
    int x, y, exist;

    for(x = 0; x < tlb->entry; x ++) {
        exist = 0;
        for(y = 0; y < pte->entry; y ++) {
            if(pte->recency[y] == tlb->recency[x]) {
                exist = 1; 
                break;
            }
        }
        if(!exist) tlb->recency[x] = -1;
    }
    TLB_reset(tlb);
}

void TLB_reset(m_unit* tlb) {
    int x,  y;
    for(x = 0; x < tlb->entry; x ++) {
        if(tlb->recency[x] == -1) {
            for(y = x + 1; y < tlb->entry; y ++) {
                int temp = tlb->recency[y];
                if(temp != -1) {
                    tlb->recency[y] = -1;
                    tlb->recency[x] = temp;
                    break;
                }
            }
        }
    }
}

int Cequals(int addr, int cur) {
    //if(cur_type)printf("%d %d CEQUALS\n", addr, cur);
    if(addr == cur) {
      //  if(cur_type)printf("%d %d are EQUALS\n", addr, cur); 
        return 1;
    }

    int result = 0;
    int blk_size = cur_type ? d_cac->size : i_cac->size;
    int mod = cur_type ? d_cac->entry : i_cac->entry;
    int e_addr = (addr / blk_size) % mod;
    int cur_addr = (cur / blk_size) % mod;

    if(cur_addr != e_addr) return 0;
    
    // in same set entry

    int max = blk_size - 4;

    if(abs(addr - cur) <= max) {
        //if(cur_type)printf("%d %d are EQUALS\n", addr, cur); 
        return 1;
    }
    else return 0;
}

int CAC_search(m_unit* cac, int addr) {
    int x, y;
    //printf("%d cac_ent\n", cac->entry);

    // search the cache to fine the addr.
    for(x = 0; x < cac->entry; x += y) {
       //printf("x moving %d\n", x);
        for(y = 0; y < cac->way; y ++) {
            if(Cequals(addr, cac->c_recency[x][y])) return HIT;
        }
    }
    return MISS;
}

void CAC_invalid(m_unit* cac, m_unit* pte, int page) {
    int x, y;
    int cac_reset = 0;
    for(x = 0; x < cac->entry; x += y) {
        cac_reset = 0;
        for(y = 0; y < cac->way; y ++) {
            if(cac->c_recency[x][y] == -1) {
                //y = cac->way;
                continue;
            }
            
            int t_page = (cac->c_recency[x][y] / pte->size) & 0x1;
            if(t_page == page) {
                cac->c_recency[x][y] = -1;
                cac_reset = 1;
            }
        }
        if(cac_reset) CAC_reset(cac, x);
    }
    
}


void CAC_reset(m_unit* cac, int set) {
    int x, y;
    for(x = 0; x < cac->way; x ++) {
        int tar = cac->c_recency[set][x];
        if(tar == -1) {
            for(y = x + 1; y < cac->way; y ++) {
                int temp = cac->c_recency[set][y];
                if(temp != -1) {
                    cac->c_recency[set][x] = temp;
                    cac->c_recency[set][y] = -1;
                    break;
                }
             
            }
        }
    }
}

int CLRU_search(m_unit* cac, int addr) {
    int x;
    int set = (addr / cac->size) % cac->entry;
    // search the set to find the index that contains target address
    for(x = 0; x < cac->way; x ++) {
        if(cac->c_recency[set][x] == addr) return x;
    }
    return -1;
}

void CLRU_insert(m_unit* cac, int addr) {
    int x;
    int set = (addr / cac->size) % cac->entry;
    for(x = 0; x < cac->way; x ++) {
        // if it's -1, it is invalid and can be insert directly
        if(cac->c_recency[set][x] == -1) {
            cac->c_recency[set][x] = addr;
            return;
        }
    } 
    // table full
    // replace
    CLRU_update(cac, 0, addr);

}

void CLRU_update(m_unit* cac, int from, int addr){
    int x;
    int set = (addr / cac->size) % cac->entry;
    cac->c_recency[set][from] = addr;
    for(x = from; x < cac->way - 1; x ++) {
        // move the target address to the end of the queue.
        // the one on the first place will be replace first
        // (from = 0, replace, otherwise it represent update recency)
        int temp = cac->c_recency[set][x + 1];
        cac->c_recency[set][x + 1] = cac->c_recency[set][x];
        cac->c_recency[set][x] = temp;
    }
}


int PTE_search(m_unit* pte, int tag) {
    int x;
    for(x = 0; x < pte->entry; x ++) {
        if(pte->recency[x] == tag) {
            return HIT;
        }
    }
    return MISS;
}

int TLB_search(m_unit* tlb, int tag) {
    int x;
    for(x = 0; x < tlb->entry; x ++) {
        if(tlb->recency[x] == tag) {
            return HIT;
        }
    }
    return MISS;
}

int LRU_search(m_unit* tar, int tag) {
    int x;
    for(x = 0; x < tar->entry; x ++) {
        if(tar->recency[x] == tag) return x;
    }
    return -1;
}

void LRU_insert(m_unit* tar, int size, int tag) {
    int x;
    for(x = 0; x < size; x ++) {
        if(tar->recency[x] == -1) {
            // insert success
            tar->content[tag] = tar->ppn;
            tar->ppn ++;
            if(tar->ppn == cur_ppn) tar->ppn = 0;
            
            tar->recency[x] = tag;
            return;
        }
    }
    // table is full.
    // replace policy
    LRU_update(tar, 0, size, tag);
    
}

void LRU_update(m_unit* tar, int from, int size, int tag) {
    int x;
    int old = tar->recency[from];
    // let tag's ppn equals to the one that be replaced
    tar->content[tag] = tar->content[old];
    if(tar->isPTE) {
        m_unit* cac = cur_type ? d_cac : i_cac;
        CAC_invalid(cac, tar, tar->content[tag]);
    }
    tar->content[old] = -1;
    tar->recency[from] = tag;
    for(x = from; x < size - 1; x ++) {
        int temp = tar->recency[x + 1];
   
        tar->recency[x + 1] = tar->recency[x];
        tar->recency[x] = temp; 
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
    d_cac->way = param[10]; //cache association
    d_cac->entry = param[8] / d_cac->size / d_cac->way; // total / block_size = total block. total_block / way = total set(entry). each set(entry) has way block
    d_tlb->entry = d_page->entry / 4; // tlb entry = quarter of page entry 
    d_pn = d_mem->size / d_page->size; // ppn is equal to 
    d_pn_c = 0;

    d_tlb->entry = (d_pn <= d_tlb->entry) ? d_pn : d_tlb->entry; // the actual valid will be limit by ppn. Equals to ppn if tlb entry > ppn
    d_page->entry = (d_pn <= d_page->entry) ? d_pn : d_page->entry; // the actual valid page will be limit by ppn. same as above

    d_page->hm[HIT] = 0; d_page->hm[MISS] = 0;
    d_cac->hm[HIT] = 0; d_cac->hm[MISS] = 0;
    d_tlb->hm[HIT] = 0; d_tlb->hm[MISS] = 0;
    d_mem->hm[HIT] = 0; d_mem->hm[MISS] = 0;
    for(x = 0; x < MEMO_LIMIT; x ++) {
        d_mem->content[x] = -1; 
        d_mem->recency[x] = -1;
        d_cac->content[x] = -1; 
        d_cac->recency[x] = -1; 
        d_tlb->content[x] = -1; 
        d_tlb->recency[x] = -1;
        d_page->content[x] = -1; 
        d_page->recency[x] = -1; 
        d_cur_pn[x] = -1;
        d_page->ppn = 0;
        d_page->isPTE = 1;
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
    i_cac->way = param[7];
    i_cac->entry = param[5] / i_cac->size / i_cac->way; // cache entry = total cahche / cache size
    i_tlb->entry = i_page->entry / 4;
    i_pn = i_mem->size / i_page->size; 
    i_pn_c = 0;
    
    i_tlb->entry = (i_pn <= i_tlb->entry) ? i_pn : i_tlb->entry; // the actual valid will be limit by ppn. Equals to ppn if tlb entry > ppn
    i_page->entry = (i_pn <= i_page->entry) ? i_pn : i_page->entry; // the actual valid page will be limit by ppn. same as above

    i_page->hm[HIT] = 0; i_page->hm[MISS] = 0;
    i_cac->hm[HIT] = 0; i_cac->hm[MISS] = 0;
    i_tlb->hm[HIT] = 0; i_tlb->hm[MISS] = 0;
    i_mem->hm[HIT] = 0; i_mem->hm[MISS] = 0;
    
    for(x = 0; x < MEMO_LIMIT; x ++) {
        i_mem->content[x] = -1; 
        i_mem->recency[x] = -1;
        i_cac->content[x] = -1; 
        i_cac->recency[x] = -1; 
        i_tlb->content[x] = -1; 
        i_tlb->recency[x] = -1; 
        i_page->content[x] = -1; 
        i_page->recency[x] = -1; 
        i_cur_pn[x]= -1;
        i_page->ppn = 0;
        i_page->isPTE = 1;
    }
    int z;
    for(x = 0; x < MEMO_LIMIT; x ++) {
        for(z = 0; z < MEMO_LIMIT; z ++) {
            // initialize cache set recency
            i_cac->c_recency[x][z] = -1;
            d_cac->c_recency[x][z] = -1;
        }
    }

}


void report(FILE* fptr_report) {

    fprintf( fptr_report, "ICache :\n");
    fprintf( fptr_report, "# hits: %u\n", i_cac->hm[HIT] );
    fprintf( fptr_report, "# misses: %u\n\n", i_cac->hm[MISS] );
    fprintf( fptr_report, "DCache :\n");
    fprintf( fptr_report, "# hits: %u\n", d_cac->hm[HIT] );
    fprintf( fptr_report, "# misses: %u\n\n", d_cac->hm[MISS] );
    fprintf( fptr_report, "ITLB :\n");
    fprintf( fptr_report, "# hits: %u\n", i_tlb->hm[HIT] );
    fprintf( fptr_report, "# misses: %u\n\n", i_tlb->hm[MISS] );
    fprintf( fptr_report, "DTLB :\n");
    fprintf( fptr_report, "# hits: %u\n", d_tlb->hm[HIT] );
    fprintf( fptr_report, "# misses: %u\n\n", d_tlb->hm[MISS]);
    fprintf( fptr_report, "IPageTable :\n");
    fprintf( fptr_report, "# hits: %u\n", i_page->hm[HIT] ); fprintf( fptr_report, "# misses: %u\n\n", i_page->hm[MISS] );
    fprintf( fptr_report, "DPageTable :\n");
    fprintf( fptr_report, "# hits: %u\n", d_page->hm[HIT] ); fprintf( fptr_report, "# misses: %u\n\n", d_page->hm[MISS] );
}
