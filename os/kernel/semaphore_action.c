#include"kernel.h"
void process_semaphore_action(int semaphore_id)
{
	int v_value,rt_id,thread_id,flag;
	struct semaphore_struct *s;
	struct return_stack *rt;
	struct thread_struct *t; 

	s=&(os->os_semaphore[semaphore_id]); 
	flag=SEMAPHORE_V_OPERATION_TIMER_ACTION; 
	if(((s->timer_action_flag)&flag)!=0){
		v_value=s->v_value;
		system_call_v(semaphore_id,&v_value,&(s->capability));
	} 
	flag=SEMAPHORE_PROCESS_TIMER_ACTION;
	if(((s->timer_action_flag)&flag)!=0)
	if(s->entity_type_id==SEMAPHORE_PROCESS_ENTITY_TYPE){
		if((thread_id=create_thread(
			(struct user_file_information *)0,
			s->entity_id,s->entity_id,
			timeout_create_thread_priority(s->entity_id), 
				os->os_domain[s->entity_id].cpu_set_id,
				(struct thread_environment *)0,
				&(os->os_domain[s->entity_id].
					capability)))>=0)
		{ 
			t=&(os->os_thread[thread_id]);
			rt_id=t->return_stack_pointer;
			rt=&(os->os_return_stack[rt_id]);
			COPY_RUN_POINT(os->os_domain[s->entity_id].
				start_point,rt->environment.point);
			rt->environment.reg.system_call
				=PROCESS_TIME_OUT_REQUEST;
			rt->environment.reg.r1=s->entity_id;
			rt->environment.reg.r2=semaphore_id;
		}
	}

	flag=SEMAPHORE_RETURN_STACK_TIMER_ACTION;
	if((flag&(s->timer_action_flag))!=0)
	if(s->entity_type_id==SEMAPHORE_RETURN_STACK_ENTITY_TYPE)
	if((rt_id=(s->entity_id))>=0)
	if(rt_id<OS_RETURN_STACK_NUMBER){
		rt=&(os->os_return_stack[rt_id]);
		rt->flag|=RETURN_STACK_TIME_OUT_FLAG;
		thread_id=rt->current_thread_id;
		t=&(os->os_thread[thread_id]);
		if((t->state)==THREAD_SLEEP_STATE)
		if(semaphore_id==(t->sleep_semaphore_id))
		if(rt_id==(t->return_stack_pointer))
			v_thread(thread_id,TRUE);
	} 
	return;
}
