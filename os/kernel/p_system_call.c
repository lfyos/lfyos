#include"kernel.h" 
static int giveup_monitor(struct return_stack *rt)
{
	int sem_id,v_value,return_value;

	if(((rt->flag)&RETURN_STACK_PROCESS_P_FLAG)!=0)
		return 0; 
	sem_id=os->os_domain[rt->current_domain_id].semaphore;
	if((sem_id<=0)||(sem_id>=OS_SEMAPHORE_NUMBER))
		return 0; 
	v_value=1;
	return_value=system_call_v(sem_id,&v_value,
		&(os->os_semaphore[sem_id].capability));
	if(return_value>=0)
		rt->flag=((rt->flag)|RETURN_STACK_PROCESS_P_FLAG);
	return return_value;
}
int system_call_p(int thread_id,int semaphore_id,int sleep_flag,
	int giveup_monitor_flag,CAPABILITY *capability)
{
	int return_value;
	struct return_stack *rt;
	struct thread_struct *t;

	t=&(os->os_thread[thread_id]);
	rt=&(os->os_return_stack[t->return_stack_pointer]);

	if(giveup_monitor_flag)
		if((return_value=giveup_monitor(rt))<0)
			return return_value;
	return p(thread_id,semaphore_id,sleep_flag,TRUE,capability);
}
