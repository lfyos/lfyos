#include"../driver.h" 
#define NO_FREE_BLOCK			(-1) 
#define CAN_NOT_MAP_BLOCK		(-2) 
int operate_file_system(
	int command,int flag,int memory_id,int physical_number,
	int begin_address,int data_length,int memory_domain_id,
	int sleep_semaphore,struct kernel_file_window *file)
{
	int return_value,fs_stack_block;
	struct memory_block *bs;
	struct file_system_call_parameter *par;
	char *p;

	if((fs_stack_block=get_free_block(memory_domain_id))<0)
		return NO_FREE_BLOCK; 
	bs=memory_body->memory_block+fs_stack_block;

	p=map_memory_block(memory_body->my_memory_body,bs->physical_id);
	if(((int)p)==0)
		return CAN_NOT_MAP_BLOCK;
	p+=PAGE_SIZE/sizeof(char);
	par=(-1)+((struct file_system_call_parameter *)p);

	par->command=command;
	par->flag=flag;
	par->begin_address=begin_address;
	par->data_length=data_length;
	par->block.stack_memory_id=memory_body->my_memory_body;
	par->block.stack_block_id=bs->physical_id;
	par->block.memory_id=memory_id;
	par->block.block_id=physical_number;

	COPY_FILE_WINDOW((*file),(par->kernel_file_window)); 
	return_value=operate_device(par,sleep_semaphore); 
	free_free_block(memory_domain_id,fs_stack_block);

	mark_memory_block_unmap(
		memory_body->my_memory_body,bs->physical_id);
	return return_value;
}
