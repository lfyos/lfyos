#include"kernel.h"
#include"../driver/driver.h"
#define  WRONG_PROCESS_ID		(-1)
#define  WRONG_BLOCK_NUMBER		(-3) 
int create_request_thread(struct thread_physical_block *system_pb)
{
	int thread_id,domain_id,ret_val;
	struct file_system_call_parameter *par;
	struct processor_domain_struct *p;
	struct return_stack *rt;

	if(system_pb->stack_memory_id<0)
		return WRONG_BLOCK_NUMBER;
	if(system_pb->stack_memory_id>=driver_number)
		return WRONG_BLOCK_NUMBER;
	par=(struct file_system_call_parameter *)map_memory_block(
		system_pb->stack_memory_id,system_pb->stack_block_id); 
	if(((int)par)==0)
		return WRONG_BLOCK_NUMBER;
	par=((struct file_system_call_parameter *)
		((PAGE_SIZE/sizeof(char))+((char *)par)))-1;

	domain_id=par->kernel_file_window.file.remote.domain_id;
	if((domain_id>=OS_DOMAIN_NUMBER)||(domain_id<=0)){
		mark_memory_block_unmap(system_pb->stack_memory_id,
			system_pb->stack_block_id); 
		return WRONG_PROCESS_ID;
	}
	p=&(os->os_domain[domain_id]);
	ret_val=par->kernel_file_window.file.remote.priority;
	ret_val=remote_create_thread_priority(ret_val,domain_id);

	if((thread_id=create_thread((struct user_file_information *)0,
		domain_id,domain_id,ret_val,p->cpu_set_id,
		(struct thread_environment *)0,
		&(par->kernel_file_window.file.
			remote.domain_capability)))<0)
		{
			mark_memory_block_unmap(system_pb->stack_memory_id,
				system_pb->stack_block_id); 
			return thread_id;
		}
	ret_val=os->os_thread[thread_id].return_stack_pointer;
	rt=&(os->os_return_stack[ret_val]);
	COPY_THREAD_PHYSICAL_BLOCK((*system_pb),(rt->physical_block));

	if((ret_val=par->kernel_file_window.file.remote.server_id)<=0){
		INIT_DEVICE_RUN_POINT(rt->environment.point);
	}else{
		INIT_SERVER_RUN_POINT(rt->environment.point);
		if((ret_val=thread_migrate(thread_id,
			ret_val,TRUE,TRUE,&(par->block)))<0)
		{
			thread_return(thread_id,0);
			thread_id=ret_val;
		}
	}
	mark_memory_block_unmap(system_pb->stack_memory_id,
		system_pb->stack_block_id); 
	return thread_id;
}
