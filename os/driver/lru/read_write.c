#include"../driver.h" 
#define NO_FREE_BLOCK			(-1)
#define CAN_NOT_MAP_BLOCK		(-2)
#define TOO_LARGE_ADDRESS		(-3)
int read_write_file_system(int command,struct memory_block *b,
	int *begin_address,int sleep_semaphore)
{
	char *p;
	int data_length,return_value,fs_stack_block;
	struct memory_block *bs;
	struct file_window *f;
	struct file_system_call_parameter *par;

	f=(b->file_window_id)+(memory_body->file_window);
	data_length=f->kernel_file_window.window_length;
	if((*begin_address)>=data_length)
		return TOO_LARGE_ADDRESS;
	if((data_length=data_length-(*begin_address))>PAGE_SIZE)
		data_length=PAGE_SIZE;
	if((fs_stack_block=get_free_block(f->memory_domain_id))<0)
		return NO_FREE_BLOCK;
	bs=fs_stack_block+(memory_body->memory_block);
	p=map_memory_block(
		memory_body->my_memory_body,bs->physical_id);
	if(((int)p)==0)
		return CAN_NOT_MAP_BLOCK;
	p=p+(PAGE_SIZE/sizeof(char));
	par=(-1)+((struct file_system_call_parameter *)p);

	par->command=command;
	par->flag=FILE_SYSTEM_CALL_PARAMETER_DEFAULT_FLAG;
	par->begin_address=(*begin_address);
	par->data_length=data_length;
	par->block.stack_memory_id=memory_body->my_memory_body;
	par->block.stack_block_id=bs->physical_id;
	par->block.memory_id=memory_body->my_memory_body;
	par->block.block_id=b->physical_id;

	COPY_FILE_WINDOW((*f),(par->kernel_file_window));

	f->lock_number++; 
	return_value=operate_device(par,sleep_semaphore); 
	f->lock_number--; 
	free_free_block(f->memory_domain_id,fs_stack_block);

	mark_memory_block_unmap(
		memory_body->my_memory_body,bs->physical_id); 
	return return_value;
}
