#include"driver.h" 
#define WRONG_FILE_PROXY		(-1)
#define NO_DRIVER_FUNCTION		(-2) 
int operate_device(
	struct file_system_call_parameter *par,int sleep_semaphore)
{
	int return_value,process_id;
	int device_id,memory_body_id;
	char *d;

	par->reply_result=0; 
	process_id=par->kernel_file_window.file.server_id;
	device_id=(-process_id);

	if((device_id>=0)&&(device_id<driver_number)){
		if(((int)(driver[device_id].device_driver_routine))==0)
			return NO_DRIVER_FUNCTION;
		else{
			memory_body_id=unlock_memory_and_lock_device(
				device_id);
			return_value=call_device_driver(
				driver[device_id].device_driver_routine,
				par,sleep_semaphore);
			unlock_device_and_lock_memory(
				memory_body_id,device_id);
		}
	}else if((process_id>0)&&(process_id<OS_DOMAIN_NUMBER)){ 
		d=map_memory_block(par->block.stack_memory_id,
			par->block.stack_block_id);
		return_value=sizeof(struct file_system_call_parameter);
		return_value=PAGE_SIZE-return_value;
		clear_data(d,return_value);
		mark_memory_block_unmap(par->block.stack_memory_id,
			par->block.stack_block_id);
		if(par->command==READ_FILE){
			d=map_memory_block(par->block.memory_id,
				par->block.block_id);
			clear_data(d,PAGE_SIZE);
			mark_memory_block_unmap(par->block.memory_id,
				par->block.block_id);
		}
		return_value=migrate_to_file_system(
			process_id,par,&(par->block));
	}else
		return_value=WRONG_FILE_PROXY;

	if(return_value>=0)
		return_value=par->reply_result;
	return return_value;
}
