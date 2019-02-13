#include "../driver.h"
#define NO_FREE_MEMORY_BLOCK			(-1)
static void not_enough_memory_wait(int sleep_semaphore)
{
	int next_processor,next_semaphore; 
	next_processor=memory_body->wait_block->processor;
	next_semaphore=memory_body->wait_block->semaphore;
	memory_body->wait_block->processor=memory_body->my_processor;
	memory_body->wait_block->semaphore=sleep_semaphore;
	memory_wait(sleep_semaphore,next_processor,next_semaphore);
	return;
}
int prepare_free_block(int memory_domain,int sleep_semaphore)
{
	int bn,i,ret_val,can_write,should_free;
	struct memory_block *b;
	struct file_window *f;
	struct memory_domain_struct *p;
	
	p=(memory_body->memory_domain)+memory_domain;
	for(i=0;i<(p->max_block_number);i++){
		if(!should_prepare_write(p,&can_write,&should_free))
			return 0;
		if(can_write){
			bn=p->block_ring;
			mark_block_accessed(bn);
			b=bn+(memory_body->memory_block);
			f=(memory_body->file_window)+(b->file_window_id);
			if(((f->flag)&FILE_WINDOW_SWAP_OUT_FLAG)!=0){
				ret_val=flush_block(bn,sleep_semaphore,
					FALSE,should_free,FALSE,-1);
				if((ret_val<0)||(!should_free))
					return ret_val;
			}
		}else{
			i--;
			not_enough_memory_wait(sleep_semaphore);
		}
	}
	return NO_FREE_MEMORY_BLOCK;
}
