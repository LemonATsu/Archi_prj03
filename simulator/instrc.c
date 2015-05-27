#include "instrc.h"


void ins_init(struct words *i_img[], int im_size, int ip_size, int ic_size, int ib_size, int is) {
    pc = char_to_num(i_img[0]->machine_code);
    ins_num = char_to_num(i_img[1]->machine_code);
    //printf("pc: 0x%08x, #of ins: 0x%08x\n", pc, ins_num);
    

    int x = 0, y = 2 + ins_num, max = MEMO_LIMIT / 4;
    //initialize
    i_mem_size = im_size;
    i_page_size = ip_size;
    i_cac_size = ic_size;
    i_blk_size = ib_size;
    i_set = is;

    i_mem = malloc(sizeof(m_unit)); 
    i_cac = malloc(sizeof(m_unit));
    i_tlb = malloc(sizeof(m_unit)); 
    i_page = malloc(sizeof(m_unit)); 
    
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

    i_page_entry = MEMO_LIMIT / i_page_size;
    i_tlb_entry = i_page_entry / 4;
    printf("%d %d for i.", i_page_entry, i_tlb_entry);

    for(x = 0; x < max; x ++) {
        struct ins* i_init = malloc(sizeof(struct ins));
        i_init->pc_addr = x * 4;
        i_init->bits = 0x00000000;
        i_disk[x] = i_init;
    }
    //load iimage contend;
    for(x = 2; x < y; x ++) {
        //the first instruction is start at pc(initial value)
        int offset = pc + (x - 2) * 4;
        struct ins* i_init = i_disk[offset / 4];
        i_init->pc_addr = offset;
        i_init->bits = char_to_num(i_img[x]->machine_code); 
    }
    /* 
    for(x = 0; x < MEMO_LIMIT / 4; x ++) {
        struct ins* i_check = i_disk[x];
        printf("PC: 0x%08x(%d), bits: 0x%08x\n", i_check->pc_addr, x, i_check->bits);

    }*/
}
