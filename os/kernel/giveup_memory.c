#include"kernel.h" 
int give_up_memory_body(struct return_stack *rt)
{
	int sem_id,*pointer,v_value,body_id;

	body_id=rt->current_memory_body_id;
	if((body_id<0)||(body_id>=driver_number))
		return -1;
	pointer=&(driver[body_id].memory_spin_lock);
	sem_id=driver[body_id].memory_semaphore_id;
	if((sem_id<0)||(sem_id>=OS_SEMAPHORE_NUMBER))
		return -2;
	
	rt->flag|=RETURN_STACK_LOCK_MEMORY_FLAG;
	if(operate_spinlock(pointer,1)>=0)
		return 1;
	v_value=1;
	system_call_v(sem_id,&v_value,&(os->system_capability));
	return 2; 
}
