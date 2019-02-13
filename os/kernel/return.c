#include"kernel.h"
#define WRONG_THREAD		-1
#define WRONG_THREAD_STATE	-2 
static int exit_thread(int thread_id)
{
	int front_thread,back_thread;
	struct thread_struct *t;

	t=&(os->os_thread[thread_id]);
	reset_semaphore_value(t->semaphore_id,&(os->system_capability));
	os->os_semaphore[0].value=(-OS_THREAD_NUMBER);
	p(thread_id,0,TRUE,FALSE,&(os->system_capability));

	if(t->process_front==thread_id)
		os->os_domain[t->domain_id].thread_ring=-1;
	else{
		os->os_domain[t->domain_id].thread_ring
			=t->process_front;
		os->os_thread[t->process_front].process_back
			=t->process_back;
		os->os_thread[t->process_back].process_front
			=t->process_front;
	}
	os->os_domain[t->domain_id].thread_number--;
	os->os_domain[0].thread_number++;

	t->domain_id=0;
	if(os->os_domain[0].thread_ring<0){
		t->process_front=thread_id;
		t->process_back=thread_id;
	}else{
		back_thread=os->os_domain[0].thread_ring;
		front_thread=os->os_thread[back_thread].process_front;
		os->os_thread[front_thread].process_back=thread_id;
		os->os_thread[back_thread].process_front=thread_id;
		os->os_thread[thread_id].process_front=front_thread;
		os->os_thread[thread_id].process_back=back_thread;
	}
	os->os_domain[0].thread_ring=thread_id;

	t->return_stack_pointer=(-1);
	t->flag=THREAD_DEFAULT_FLAG;
	return 0;
}
int thread_return(int thread_id,int return_code)
{
	int i,sem_value,rt_id,flag;
	struct thread_struct *t;
	struct processor_domain_struct *process;
	struct return_stack *last_rt,*rt;

	if((thread_id<0)||(thread_id>=OS_THREAD_NUMBER))
		return WRONG_THREAD;
	t=&(os->os_thread[thread_id]);
	if(t->state!=THREAD_RUN_STATE)
	if(t->state!=THREAD_READY_STATE){
		v_thread(thread_id,FALSE);
		if(t->state!=THREAD_RUN_STATE)
		if(t->state!=THREAD_READY_STATE)
			return WRONG_THREAD_STATE;
	}
	if((rt_id=t->return_stack_pointer)<0){
		exit_thread(thread_id);
		return 0;
	}
	rt=&(os->os_return_stack[rt_id]);

	begin_thread_return_priority(thread_id);

	for(i=0;;i++){
		thread_return_priority(thread_id);
		reset_semaphore_value(rt->user_semaphore_id,
			&(os->system_capability)); 
		process=&(os->os_domain[rt->current_domain_id]);
		process->enter_thread_number--;
		if(((rt->flag)&RETURN_STACK_PROCESS_P_FLAG)==0)
		if((rt->domain_id)==(process->domain_id))
		if(process->semaphore>0){
			sem_value=1;
			system_call_v(process->semaphore,
				&sem_value,&(process->capability));
		}

		rt->flag|=RETURN_STACK_PROCESS_P_FLAG;
		t->return_stack_pointer=rt->next_return_stack;
		rt->next_return_stack=os->free_return_stack;
		os->free_return_stack=rt_id;
		rt->current_thread_id=(-1);
		os->os_domain[rt->release_domain_id].
			return_stack_number--;
		os->os_domain[0].return_stack_number++;

		if((rt_id=t->return_stack_pointer)<0)
			return exit_thread(thread_id);

		last_rt=rt;
		rt=&(os->os_return_stack[rt_id]);
		process=&(os->os_domain[rt->current_domain_id]);

		if(((rt->domain_id)==(process->domain_id))
		&&((rt->flag)&RETURN_STACK_TIME_OUT_FLAG)==0){ 
			flag=RETURN_STACK_NO_RETURN_VALUE_FLAG;
			if((flag&(rt->flag))!=0)
				rt->flag&=(~flag);
			else{
				if(i==0)
					COPY_GENERAL_REGISTER(
					(last_rt->environment.reg),
					(rt->environment.reg));
				rt->environment.reg.system_call=(i+1);
				rt->environment.reg.r1=return_code;
			}
			break;
		}
	}
	modify_thread_priority(thread_id,
		end_thread_return_priority(thread_id),rt->cpu_set_bak);

	rt->priority_bak=t->priority;
	rt->cpu_set_bak=t->cpu_set_id;

	return 0;
}
