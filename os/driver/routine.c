#include"../driver/driver.h"
#include"../include/os.h" 
#define FILE_SYSTEM_ERROR	(-1)
#define WRONG_DRIVER_NUMBER	(-2)
#define WRONG_DRIVER_FUNCTION	(-3) 
#define WRONG_STACK_MEMORY_ID	(-4)
#define WRONG_BLOCK_MEMORY_ID	(-5) 
void start_device_routine()
{
	int return_value,device_id;
	call_kernel_register r;
	struct file_system_call_parameter *par;

	par=get_file_system_parameter();
	device_id=0-(par->kernel_file_window.file.server_id);

	if((device_id<0)||(device_id>=driver_number))
		return_value=WRONG_DRIVER_NUMBER;
	else if(((int)(driver[device_id].device_driver_routine))==0)
		return_value=WRONG_DRIVER_FUNCTION;
	else
		return_value=driver[device_id].device_driver_routine(
			par,current->rt->user_semaphore_id);
	if((par->reply_id>=0)&&(par->reply_id<driver_number))
	if(((int)(driver[par->reply_id].request_complete))!=0)
		driver[par->reply_id].request_complete(par->reply_code);

	r.system_call=THREAD_RETURN_SYSTEM_CALL;
	r.r1=0;
	call_kernel(&r);
	return;
}
void start_server_routine(struct general_register *env)
{
	call_kernel_register r;
	struct file_system_call_parameter *par;

	par=get_file_system_parameter();

	if((env->system_call)!=1)
		par->reply_result=FILE_SYSTEM_ERROR;
	else if(env->r1<0)
		par->reply_result=env->r1;
	else 
		par->reply_result=env->r2;

	if((par->reply_id>=0)&&(par->reply_id<driver_number))
	if(((int)(driver[par->reply_id].request_complete))!=0)
		driver[par->reply_id].request_complete(par->reply_code);

	r.system_call=THREAD_RETURN_SYSTEM_CALL;
	r.r1=0;
	call_kernel(&r);
	return;
}
int server_interruption_routine(int copy_flag,
	struct thread_physical_block *pb)
{
	call_kernel_register r;
	struct thread_physical_block pb_bak;
	struct file_system_call_parameter *sys_par,*usr_par;
	union system_call_parameter *par;

	if(pb->stack_memory_id<0)
		return WRONG_STACK_MEMORY_ID;
	if(pb->stack_memory_id>=driver_number)
		return WRONG_STACK_MEMORY_ID;
	if(copy_flag){
		sys_par=(struct file_system_call_parameter *)
			(map_memory_block(pb->stack_memory_id,
				pb->stack_block_id));
		if(((int)sys_par)==0)
			return WRONG_STACK_MEMORY_ID;
		sys_par=(struct file_system_call_parameter *)
			((PAGE_SIZE/sizeof(char))+(char *)sys_par)-1;
		if((sys_par->block.stack_memory_id>=driver_number)
		  ||(sys_par->block.stack_memory_id<0))
		{
			mark_memory_block_unmap(
				pb->stack_memory_id,pb->stack_block_id);
			return WRONG_BLOCK_MEMORY_ID;
		}
		usr_par=(struct file_system_call_parameter *)
			(map_memory_block(
				sys_par->block.stack_memory_id,
				sys_par->block.stack_block_id));
		if(((int)usr_par)==0){
			mark_memory_block_unmap(
				pb->stack_memory_id,pb->stack_block_id);
			return WRONG_BLOCK_MEMORY_ID;
		}
		usr_par=(struct file_system_call_parameter *)
			((PAGE_SIZE/sizeof(char))+(char *)usr_par)-1;

		copy_data(sys_par,usr_par,
			sizeof(struct file_system_call_parameter));
		mark_memory_block_unmap(
			sys_par->block.stack_memory_id,
			sys_par->block.stack_block_id);
		mark_memory_block_unmap(
			pb->stack_memory_id,pb->stack_block_id);
	}
	par=get_kernel_parameter();
	COPY_THREAD_PHYSICAL_BLOCK((par->thread_physical_block),pb_bak);
	COPY_THREAD_PHYSICAL_BLOCK((*pb),(par->thread_physical_block));
	r.system_call=CREATE_REQUEST_THREAD_SYSTEM_CALL;
	call_kernel(&r);
	COPY_THREAD_PHYSICAL_BLOCK(pb_bak,(par->thread_physical_block));
	return r.system_call;
}
