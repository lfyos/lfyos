#include"../driver.h"
#define WRONG_FILE_NUMBER	(-1)
#define WRONG_FILE_STATE	(-2)
#define WRONG_CAPABILITY	(-3)
#define WRONG_SEMAPHORE		(-4)
#define MAP_NO_RIGHT		(-5) 
static void setup_map_routine(int block_id,int root_block_id,int right)
{
	int need_clear_id,need_block_id;
	struct memory_block *b,*b_r;

	if((need_block_id=request_physical_id())<0)
		return;
	for(b=block_id+(memory_body->memory_block);;){
		if((need_clear_id=memory_need_clear_tlb_item(
			memory_body->my_memory_body))<0)
				break;
		if(need_clear_id>=(memory_body->head->block_number))
			break;
		clear_block_number(need_clear_id);
	}
	b_r=root_block_id+(memory_body->memory_block);
	if(memory_set_tlb_item(memory_body->my_memory_body,
		b->file_window_id,b->logic_block_id,b_r->physical_id,
		right,block_id,&need_block_id)>=0)
	{
		for(;;){
			b->lock_number++;
			if(b->parent_block_id<0)
				break;
			b=(b->parent_block_id)
				+(memory_body->memory_block);
		}
	}
	if(need_block_id>=0)
		release_physical_id(need_block_id);
	return;
} 
int memory_setup_map(int semaphore_id,int file_window_id,
	int begin_address,int end_address,int read_only_flag,
	CAPABILITY *file_capability)
{
	int logic_address,bn,root_block_id,right,max_file_id;
	struct file_window *f;
	CAPABILITY *c;

	if(semaphore_id<=0)
		return WRONG_SEMAPHORE;
	max_file_id=((memory_body->head->file_number));
	if((file_window_id<0)||(file_window_id>=max_file_id))
		return WRONG_FILE_NUMBER;
	f=(memory_body->file_window)+file_window_id;
	if((f->state)!=FILE_OPEN)
		return WRONG_FILE_STATE; 
	c=&(f->kernel_file_window.file.file_capability);
	if(!KERNEL_COMPARE_CAPABILITY(*c,(*file_capability)))
		return WRONG_CAPABILITY;
	logic_address=f->kernel_file_window.window_length;
	if(begin_address<0)
		begin_address=0;
	else if(begin_address>logic_address)
		begin_address=logic_address;
	if(end_address<0)
		end_address=0;
	else if(end_address>logic_address)
		end_address=logic_address; 
	right=f->kernel_file_window.right;
	if(read_only_flag)
		right=MAKE_READ_ONLY_RIGHT(right);
	else if(!IS_READ_WRITE_RIGHT(right))
		return MAP_NO_RIGHT;
	else
		right=MAKE_READ_WRITE_RIGHT(right);
	mark_file_accessed(file_window_id); 
	f->lock_number++;
	for(	bn=0,logic_address=begin_address;
		logic_address<end_address;logic_address+=PAGE_SIZE)
	{
		if((bn=search_read_block(file_window_id,logic_address,
			semaphore_id,read_only_flag,&root_block_id))<0)
				break;
		setup_map_routine(bn,root_block_id,right);
		mark_block_accessed(bn);
	}
	f->lock_number--;
	return bn;
}
