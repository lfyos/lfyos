#include"../driver.h" 
#define WRONG_MEMORY_DOMAIN_NUMBER	-1
#define WRONG_CAPABILITY		-2 
int control_unopened_file(int command,int flag,int begin_address,
	int data_length,int memory_id,int block_id,
	struct kernel_file_window *file,int memory_domain_id,
	int sleep_semaphore,CAPABILITY *domain_capability)
{
	int return_value,max_number;
	struct memory_domain_struct *p;

	max_number=memory_body->head->memory_domain_number;
	if((memory_domain_id<0)||(memory_domain_id>=max_number))
		return WRONG_MEMORY_DOMAIN_NUMBER;
	p=memory_domain_id+(memory_body->memory_domain);
	if(!KERNEL_COMPARE_CAPABILITY(p->capability,*domain_capability))
		return WRONG_CAPABILITY;
	if((return_value=prepare_free_block(
		memory_domain_id,sleep_semaphore))<0)
			return return_value;
	return operate_file_system(command,flag,
			memory_id,block_id,begin_address,data_length,
			memory_domain_id,sleep_semaphore,file);
}
