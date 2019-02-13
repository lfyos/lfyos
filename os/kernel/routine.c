#include"kernel.h"
#define FATAL_ERROR_WHEN_PREPARE_FOR_RESTORE -1 
static void set_current_information(int switch_cpu_flag)
{
	int i,cpu_id,thread_id;
	struct thread_struct *t;

	if(current->thread_id>=0)
		unlock_os_thread(current->thread_id);
	for(i=0;i<=OS_CPU_NUMBER;i++,switch_cpu_flag=TRUE){
		cpu_id=get_virtual_cpu(switch_cpu_flag);
		if((thread_id=os->os_cpu[cpu_id].thread_id)>=0)
		if(thread_id<OS_THREAD_NUMBER)
		if(lock_os_thread(thread_id)){
			t=&(os->os_thread[thread_id]);
			if(t->state==THREAD_RUN_STATE){
				set_current_item(thread_id);
				return;
			}
			unlock_os_thread(thread_id);
		}
	}
	current->thread_id=-1;
}
static void process_file_capability_modify()
{
	int i=0;
	struct user_file_information *f;

	if(!COMPARE_THREAD_PHYSICAL_BLOCK(
		current->bak.physical_block,current->rt->physical_block))
	{
		COPY_THREAD_PHYSICAL_BLOCK(current->rt->physical_block, 
			current->bak.physical_block);
		i++;
	} 
	if((current->bak.return_stack_id)
		!=(current->t->return_stack_pointer))
	{
		current->bak.return_stack_id
			=current->t->return_stack_pointer;
		current->file_change_flag++;
		i++;
	}
	if((current->bak.thread_id)!=(current->thread_id)){
		current->bak.thread_id=current->thread_id;
		current->file_change_flag++;
		i++;
	}
	if((current->bak.domain_id)!=(current->rt->current_domain_id)){
		current->bak.domain_id=current->rt->current_domain_id;
		current->file_change_flag++;
		i++;
	}
	if(i>0)
		mark_physical_block_modify();
	if((current->file_change_flag)==0)
		return; 
	current->file_change_flag=0;
	for(i=0;i<OS_USER_FILE_NUMBER;i++){
		if(((f=&(os->os_file[i]))->memory_body_id)<0)
		if(((f=&(current->rt->file[i]))->memory_body_id)<0)
			f=((struct user_file_information *)0);

		if(f==((struct user_file_information *)0)){
			if((current->file_pointer[i])
				!=((struct user_file_information *)0))
					mark_file_capability_modify(i);
		}else if(current->file_pointer[i]
			==(struct user_file_information *)0){
				mark_file_capability_modify(i);
				COPY_USER_FILE_INFORMATION(
					(*f),(current->bak.file[i]));
		}else if(!COMPARE_USER_FILE_INFORMATION(
			(current->bak.file[i]),(*f)))
		{
			mark_file_capability_modify(i);
			COPY_USER_FILE_INFORMATION(
				(*f),(current->bak.file[i]));
		}
		current->file_pointer[i]=f;
	}
}
static int execute_call_function(
	int (*fun)(union system_call_parameter *))
{
	char *p;
	int return_value,memory_body_id,memory_block_id;

	memory_body_id=current->rt->physical_block.stack_memory_id;
	memory_block_id=current->rt->physical_block.stack_block_id;
	if((memory_body_id<0)||(memory_body_id>=driver_number)){
		p=kernel_get_thread_physical_block(current->thread_id);
		p+=(PAGE_SIZE/sizeof(char));
		return fun(((union system_call_parameter *)p)-1);
	}
	p=map_memory_block(memory_body_id,memory_block_id);
	p+=(PAGE_SIZE/sizeof(char));
	return_value=fun(((union system_call_parameter *)p)-1);
	mark_memory_block_unmap(memory_body_id,memory_block_id); 
	return return_value;
}
static int register_system_call(
	int (*fun)(union system_call_parameter *))
{
	if((current->thread_id)>=0){
		current->t->system_call_function=fun;
		return TRUE;
	}
	return FALSE;
} 
static int register_drop_call(
	int (*fun)(union system_call_parameter *))
{
	int ret_val;
	if(lock_os_kernel()){
		ret_val=execute_call_function(fun);
		unlock_os_kernel();
		set_current_information(ret_val);
		return TRUE;
	}
	return FALSE;
}
static void register_os_function(
	int (*fun)(union system_call_parameter *),int system_call_flag) 
{
	switch(system_call_flag){
	default:
		break;
	case IS_SYSTEM_CALL:
		register_system_call(fun);
		break;
	case IS_NO_LOCK_KERNEL_DROP:
		register_drop_call(fun);
		break;
	case IS_NO_LOCK_KERNEL_CONTINUE_WAIT:
		for(;register_drop_call(fun)!=TRUE;)
			;
		break;
	case IS_NO_LOCK_KERNEL_AS_SYSTEM_CALL_ONCE:
		if(register_drop_call(fun)!=TRUE)
			register_system_call(fun);
		break;
	case IS_NO_LOCK_KERNEL_AS_SYSTEM_CALL_FOREVER:
		for(;;){
			if(register_drop_call(fun)==TRUE)
				break;
			if(register_system_call(fun)==TRUE)
				break;
		}
		break;
	}
	return;
}
static int thread_exit_routine(union system_call_parameter *par)
{
	thread_return(current->thread_id,
		FATAL_ERROR_WHEN_PREPARE_FOR_RESTORE);
	return FALSE;
}
static int execute_system_call()
{
	int flag,(*fun)(union system_call_parameter *); 
	for(flag=FALSE;;){
		set_current_information(flag);
		if(current->thread_id<0)
			return 0;
		if((int)(fun=current->t->system_call_function)==0)
			return 1;
		if(!lock_os_kernel())
			flag=TRUE;
		else{
			current->t->system_call_function=
				(int(*)(union system_call_parameter*))0;
			flag=execute_call_function(fun);
			unlock_os_kernel();
		}
	}
}
struct thread_environment *system_call(
	int (*fun)(union system_call_parameter *),int flag) 
{	
	if((current->thread_id)>=0){
		current->t->physical_cpu_id=(-1);
		prepare_for_save();
	}
	for(;;fun=thread_exit_routine,flag=IS_NO_LOCK_KERNEL_DROP){
		if(((int)fun)!=0)
			register_os_function(fun,flag); 
		if(execute_system_call()==0)
			return ((struct thread_environment *)0);
		process_file_capability_modify();
		if(prepare_for_restore()){
			current->t->physical_cpu_id=get_physical_cpu();
			return (current->env);
		}
	}
}
void start_point_for_all_processors()
{
	int i;
	current->thread_id=(-1);
	set_current_information(TRUE);
	
	for(i=0;i<OS_USER_FILE_NUMBER;i++){
		RESET_USER_FILE_INFORMATION(current->bak.file[i]);
		current->file_pointer[i]
			=(struct user_file_information *)0;
		mark_file_capability_modify(i);
	}
	current->file_change_flag=1;

	current->bak.return_stack_id=(-1);
	current->bak.thread_id=(-1);
	current->bak.domain_id=(-1);
	RESET_THREAD_PHYSICAL_BLOCK(current->bak.physical_block);
	mark_physical_block_modify();

	restore_parameter((int (*)())0,IS_NO_LOCK_KERNEL_DROP); 
	/*	never return here	*/
}
