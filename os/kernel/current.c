#include"kernel.h"
#define FATAL_ERROR_WHEN_SET_CURRENT_INFORMATION	-1
static int monitor_p_operation(union system_call_parameter *par)
{
	int domain_id,sem_id;
	domain_id=current->rt->current_domain_id;
	if((domain_id<=0)||(domain_id>=OS_DOMAIN_NUMBER))
		return FALSE;
	sem_id=os->os_domain[domain_id].semaphore;
	if((sem_id<=0)||(sem_id>=OS_SEMAPHORE_NUMBER))
		return FALSE;
	p(current->thread_id,sem_id,TRUE,FALSE,&(os->system_capability));
	return FALSE;
} 
static int memory_body_p_operation(union system_call_parameter *par)
{
	int body_id,sem_id,*pointer;
	
	body_id=current->rt->current_memory_body_id;
	if((body_id<0)||(body_id>=driver_number))
		return FALSE;
	sem_id=driver[body_id].memory_semaphore_id;
	if((sem_id<0)||(sem_id>=OS_SEMAPHORE_NUMBER))
		return FALSE;
	pointer=&(driver[body_id].memory_spin_lock);
	if(operate_spinlock(pointer,-1)>0)
		return FALSE;
	p(current->thread_id,sem_id,TRUE,FALSE,&(os->system_capability));
	return FALSE;
}
static int thread_exit_routine(union system_call_parameter *par)
{
	thread_return(current->thread_id,
		FATAL_ERROR_WHEN_SET_CURRENT_INFORMATION);
	return FALSE;
}
static int signal_process_routine(union system_call_parameter *par)
{
	int signal_bitmap;
	struct return_stack *rt;
	struct thread_struct *t;
	struct processor_domain_struct *p;

	t=current->t;
	rt=current->rt;
	p=&(os->os_domain[rt->current_domain_id]);

	signal_bitmap=rt->signal_bitmap;
	rt->signal_bitmap=0;

	if(thread_migrate(current->thread_id,0,TRUE,FALSE,
		&(rt->physical_block))>=0)
	{
		rt->flag|=RETURN_STACK_NO_RETURN_VALUE_FLAG;
		rt=&(os->os_return_stack[t->return_stack_pointer]);
		rt->environment.reg.system_call=SIGNAL_PROCESS_REQUEST;
		rt->environment.reg.r1=signal_bitmap;
		COPY_RUN_POINT_INSTRUCTION_POINTER(
			(p->start_point),(rt->environment.point));
	} 
	return FALSE;
}
void set_current_item(int thread_id)
{
	int domain_id;

	current->thread_id=thread_id;
	current->t=&(os->os_thread[current->thread_id]);
	current->rt=&(os->os_return_stack[
		current->t->return_stack_pointer]);
	current->env=&(current->rt->environment);
	domain_id=current->rt->current_domain_id;
	current->current_process=&(os->os_domain[domain_id]);

	if(IS_USER_MODE(current->rt->environment.point)){
		domain_id=os->os_domain[domain_id].domain_id;
		if(((current->rt->domain_id)!=domain_id)
		||(((current->rt->flag)&RETURN_STACK_TIME_OUT_FLAG)!=0))
		{
			current->t->system_call_function
				=thread_exit_routine;
			current->rt->flag&=(~RETURN_STACK_TIME_OUT_FLAG);
			return;
		}
	}
	if((current->t->system_call_function)!=((int (*)())0))
		return;
	if(((current->rt->flag)&RETURN_STACK_PROCESS_P_FLAG)!=0){
		current->rt->flag&=(~RETURN_STACK_PROCESS_P_FLAG);
		current->t->system_call_function=monitor_p_operation;
		return;
	} 
	if(((current->rt->flag)&RETURN_STACK_LOCK_MEMORY_FLAG)!=0){
		current->rt->flag&=(~RETURN_STACK_LOCK_MEMORY_FLAG);
		current->t->system_call_function=memory_body_p_operation;
		return;
	}
	if(((current->rt->signal_bitmap)!=0)
	&&(IS_USER_MODE(current->rt->environment.point))){
		current->t->system_call_function=signal_process_routine;
		return;
	}
	return;
}
