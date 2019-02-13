#include "../driver.h"
static void clear_block_number_routine(int block_id)
{
	int i,j,return_value,free_block_id[2],free_block_id_length;
	struct memory_block *b,*bp;

	b=block_id+(memory_body->memory_block);
	switch(b->block_state){
	case NOT_MODIFIED_BLOCK:
	case MODIFIED_BLOCK:
		free_block_id_length=sizeof(free_block_id);
		free_block_id_length/=sizeof(free_block_id[0]);
		for(;;){
			if(b->lock_number<=0){
				b->lock_number=0;
				wakeup_block(block_id);
				break;
			} 
			for(i=0;i<free_block_id_length;i++)
				free_block_id[i]=-1; 
			return_value=memory_clear_tlb_item(
				memory_body->my_memory_body,
				b->file_window_id,b->logic_block_id,
				block_id,&(free_block_id[0]));
			for(i=0;i<free_block_id_length;i++)
				if((j=free_block_id[i])>=0)
					release_physical_id(j);
			if(return_value<0)
				break;
			for(i=block_id;i>=0;i=bp->parent_block_id){
				bp=i+(memory_body->memory_block);
				bp->lock_number--;
			}
			if(return_value>0)
			if((bp->block_state)==NOT_MODIFIED_BLOCK)
				bp->block_state=MODIFIED_BLOCK;
		}
	}
}
void clear_block_number(int block_id)
{
	int i,first_block_id;
	struct memory_block *b;

	b=(memory_body->memory_block)+block_id;
	for(b->next_block_id=(-1);block_id>=0;){
		b=(memory_body->memory_block)+block_id;
		clear_block_number_routine(block_id);
		block_id=b->next_block_id;
		b->next_block_id=-1;
		if((first_block_id=b->first_child_block_id)>=0)
			for(i=first_block_id;;){
				b=(memory_body->memory_block)+i;
				b->next_block_id=block_id;
				block_id=i;
				i=b->right_brother_block_id;
				if(i==first_block_id)
					break; 
			}
	}
	return;
}
