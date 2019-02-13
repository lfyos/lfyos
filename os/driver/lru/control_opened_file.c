#include"../driver.h" 
#define WRONG_COMMAND		(-1)
#define WRONG_FILE_NUMBER	(-2)
#define WRONG_FILE_STATE	(-3)
#define WRONG_CAPABILITY	(-4) 
int control_opened_file(int sleep_semaphore,int memory_id,int block_id,
	int command,int file_window_id,int begin_address,
	int data_length,int flag,CAPABILITY *file_capability)
{
	int return_value,max_number;
	struct file_window *f;
	CAPABILITY  *c;

	max_number=memory_body->head->file_number;
	if((file_window_id<0)||(file_window_id>=max_number))
		return WRONG_FILE_NUMBER;
	f=(memory_body->file_window)+file_window_id;
	if(f->state!=FILE_OPEN)
		return WRONG_FILE_STATE;
	c=&(f->kernel_file_window.file.file_capability);
	if(!KERNEL_COMPARE_CAPABILITY(*c,*file_capability))
		return WRONG_CAPABILITY; 
	mark_file_accessed(file_window_id); 
	switch(command){
	case OPEN_FILE:
	case CLOSE_FILE:
	case OPERATE_FILE:
		break;
	default:
		return WRONG_COMMAND;
	}
	if((return_value=prepare_free_block(
		f->memory_domain_id,sleep_semaphore))<0)
			return return_value;
	f->lock_number++;
	return_value=operate_file_system(command,flag,memory_id,
		block_id,begin_address,data_length,f->memory_domain_id,
		sleep_semaphore,&(f->kernel_file_window));
	f->lock_number--;
	return return_value;
}
