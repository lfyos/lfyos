#include"kernel.h"
#define WRONG_SEMAPHORE_ID	-1
#define WRONG_CAPABILITY	-2
void kernel_semaphore_time_process()
{
	int semaphore_id;
	struct kernel_time my_time,current_time;
	struct semaphore_struct *s;

	for(;;){
		get_current_time(&current_time); 
		semaphore_id=os->os_semaphore_heap[0].semaphore_id;
		s=&(os->os_semaphore[semaphore_id]);
		if(TIME_GREAT_EQUAL((s->first),current_time))
			break;
		COPY_TIME((s->step),my_time);
		TIME_ADD((s->first),my_time);
		if(TIME_LESS_EQUAL(my_time,current_time)){
			s->v_value=(-1);
			SET_MOST_TIME(my_time);
		} 
		process_semaphore_action(semaphore_id);
		set_semaphore_time(semaphore_id,
			&my_time,s->timer_action_flag);
	}
	return ;
}
int set_semaphore_time_system_call(int semaphore_id,int v_value,
	int semaphore_action_flag,struct kernel_time *first_time,
	struct kernel_time *step_time,CAPABILITY *sem_capability)
{
	struct semaphore_struct *s;
	struct kernel_time my_time,current_time;

	if((semaphore_id<0)||(semaphore_id>=OS_SEMAPHORE_NUMBER))
		return WRONG_SEMAPHORE_ID;

	s=&(os->os_semaphore[semaphore_id]);
	if(!KERNEL_COMPARE_CAPABILITY(s->capability,(*sem_capability)))
		return WRONG_CAPABILITY;

	SET_MINIMAL_STEP(my_time);
	get_current_time(&current_time); 
	if(TIME_LESS_EQUAL((*first_time),current_time)){ 
		COPY_TIME(current_time,(*first_time));
		TIME_ADD(my_time,(*first_time));
	}
	if(TIME_LESS_EQUAL((*step_time),my_time))
		COPY_TIME(my_time,(*step_time));
	COPY_TIME((*step_time),(s->step));
	s->v_value=v_value;
 	set_semaphore_time(semaphore_id,
		first_time,semaphore_action_flag);
	return 0;
}
int set_semaphore_capability_time_and_p_operation(
	int thread_id,int semaphore_id,int do_p_flag,
	int sleep_flag,int giveup_monitor_flag,
	int semaphore_action_flag,int v_value,
	struct kernel_time *first_time,struct kernel_time *step_time,
	CAPABILITY *old_semaphore_capability,
	CAPABILITY *new_semaphore_capability)
{
	int return_value; 
	if((return_value=set_semaphore_time_system_call(semaphore_id,
		v_value,semaphore_action_flag,first_time,
		step_time,old_semaphore_capability))<0)
			return return_value;
	if(do_p_flag)
		return_value=system_call_p(thread_id,semaphore_id,
			sleep_flag,giveup_monitor_flag,
			old_semaphore_capability);
	
	COPY_CAPABILITY((*new_semaphore_capability),
		(os->os_semaphore[semaphore_id].capability));
	return return_value;
}
