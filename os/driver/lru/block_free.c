#include "../driver.h"
#define NO_FREE_BLOCK  (-1) 
int get_free_block(int memory_domain_number)
{
	int bn,return_value;
	struct memory_block *b;
	struct memory_domain_struct *p;

	p=(memory_body->memory_domain)+memory_domain_number;
	if((bn=((memory_body->head->free_block)))<0)
		return NO_FREE_BLOCK; 
	b=memory_body->memory_block+bn;
	(memory_body->head->free_block)=b->buf_back;
	b->block_state=FREE_BLOCK;

	b->parent_block_id=(-1);
	b->first_child_block_id=(-1);
	b->left_brother_block_id=bn;
	b->right_brother_block_id=bn;
	b->next_block_id=(-1);

	p->block_number++;
	((memory_body->head->free_block_number))--;

	b->physical_id=(-1);
	if((return_value=mount_physical_block(b,p))<0){
		free_free_block(memory_domain_number,bn);
		return return_value;
	}
	b->block_flag=DEFAULT_MEMORY_BLOCK_FLAG;
	return bn;
}
void free_free_block(int memory_domain_number,int block_number)
{
	int processor,semaphore;
	struct memory_block *b;
	struct memory_domain_struct *p;

	b=(memory_body->memory_block)+block_number;
	p=(memory_body->memory_domain)+memory_domain_number;

	dismount_physical_block(b,p);

	b->block_state=FREE_BLOCK;
	b->buf_back=((memory_body->head->free_block));
	(memory_body->head->free_block)=block_number;

	p->block_number--;
	((memory_body->head->free_block_number))++;

	if((processor=memory_body->wait_block->processor)>=0)
	if((semaphore=memory_body->wait_block->semaphore)>0){
		memory_body->wait_block->processor=(-1);
		memory_body->wait_block->semaphore=(-1);
		memory_wakeup(processor,semaphore);
	}
	return;
}
