#include"../driver.h"
#define CAN_NOT_MAP_BLOCK		(-1)
#define WRONG_PARENT_CAPABILITY		(-2) 
#define WRONG_FILE_NUMBER		(-3) 
#define FIND_NO_WAIT_FILE_WINDOW	(-4)
static int do_read_block(struct memory_block *b,int sleep_semaphore,
	int *logic_address,int *parent_file_window_id,
	CAPABILITY *parent_file_window_capability)
{
	int return_value;
	volatile struct file_read_parameter *read_par;
	
	RESET_CAPABILITY(*parent_file_window_capability);
	read_par=(struct file_read_parameter *)map_memory_block(
		memory_body->my_memory_body,b->physical_id);
	if(((int)read_par)==0)
		return CAN_NOT_MAP_BLOCK;
	read_par->parent_file_window_id=-1;
	read_par->begin_address=(*logic_address);
	read_par->block_flag=DEFAULT_MEMORY_BLOCK_FLAG;
	RESET_CAPABILITY(read_par->parent_file_window_capability);

	b->lock_number++; 
	if((return_value=read_write_file_system(READ_FILE,b,
		logic_address,sleep_semaphore))<0)
	{
		if(read_par->begin_address>=0)
		if(read_par->begin_address<MAX_FILE_WINDOW_LENGTH)
			*logic_address=read_par->begin_address;
		*parent_file_window_id=read_par->parent_file_window_id;
		b->block_flag=read_par->block_flag;
		COPY_CAPABILITY(read_par->parent_file_window_capability,
			*parent_file_window_capability);
	}
	b->lock_number--; 
	mark_memory_block_unmap(
		memory_body->my_memory_body,b->physical_id);
	return return_value;
}
int read_one_block(int sleep_semaphore,int *repeat_flag,
	int *file_window_id,int *logic_address,int *block_wait_queue)
{
	int bn,pbn,return_value;
	struct memory_block *b;
	struct file_window *f;
	CAPABILITY *c,parent_capability;
	
	*repeat_flag=FALSE;
	f=(memory_body->file_window)+(*file_window_id); 
	if((return_value=prepare_free_block(
		f->memory_domain_id,sleep_semaphore))<0)
			return return_value;
	bn=search_block(*file_window_id,*logic_address/PAGE_SIZE);
	if(bn>=0){
		for(pbn=bn;;){
			b=pbn+(memory_body->memory_block);
			switch(b->block_state){
			case READING_BLOCK:
			case WRITING_BLOCK:
				if(wait_at_block(pbn,sleep_semaphore))
					return FIND_NO_WAIT_FILE_WINDOW;
				*repeat_flag=TRUE;
				return bn;
			}
			if((pbn=b->parent_block_id)<0)
				break;
		}
		return bn;
	}
	if((bn=mount_reading_block(
		*file_window_id,logic_address,TRUE))<0)
			return bn;
	b=bn+(memory_body->memory_block); 
	if((return_value=do_read_block(b,sleep_semaphore,logic_address,
		file_window_id,&parent_capability))>=0){
			unmount_reading_block(bn,FALSE);
			return bn;
	}
	if((*file_window_id)<0){
		unmount_reading_block(bn,TRUE);
		return return_value;
	}
	if((*file_window_id)>=(memory_body->head->file_number)){
		unmount_reading_block(bn,TRUE);
		return WRONG_FILE_NUMBER;
	}
	c=&(f->kernel_file_window.file.file_capability);
	if(!KERNEL_COMPARE_CAPABILITY(*c,parent_capability)){
		unmount_reading_block(bn,TRUE);
		return WRONG_PARENT_CAPABILITY;
	}
	b->next_block_id=(*block_wait_queue);
	*block_wait_queue=bn;
	*repeat_flag=TRUE;

	return return_value;
}
