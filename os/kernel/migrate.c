#include"kernel.h"
#define WRONG_PROCESS			(-1)
#define NO_RETURN_STACK			(-2)
#define WRONG_THREAD			(-3)
#define WRONG_THREAD_STATE		(-4)
int thread_migrate(int thread_id,int destination_domain_id,
	int stack_flag,int giveup_monitor_flag,
	struct thread_physical_block *pb)
{
	int i,domain_id,old_domain_id,sem_value,process_p_flag,rt_id;
	struct thread_struct *t;
	struct return_stack *rt,*old_rt;
	struct processor_domain_struct *process,*old_process;

	if((thread_id<0)||(thread_id>=OS_THREAD_NUMBER))
		return WRONG_THREAD;
	t=&(os->os_thread[thread_id]);
	if((t->state!=THREAD_RUN_STATE)&&(t->state!=THREAD_READY_STATE))
		return WRONG_THREAD_STATE;

	rt_id=t->return_stack_pointer;
	old_rt=&(os->os_return_stack[rt_id]);
	old_domain_id=old_rt->current_domain_id;
	old_process=&(os->os_domain[old_domain_id]);
	if(destination_domain_id==0)
		domain_id=old_domain_id;
	else
		domain_id=destination_domain_id;
	if((domain_id<=0)||(domain_id>=OS_DOMAIN_NUMBER))
		return  WRONG_PROCESS;
	process=&(os->os_domain[domain_id]);

	old_rt->priority_bak=t->priority;
	old_rt->cpu_set_bak=t->cpu_set_id;

	if(!stack_flag){
		begin_thread_migration_priority(
			thread_id,old_domain_id,domain_id);
		rt=old_rt;
		old_process->enter_thread_number--;
	}else{
		if(old_process->return_stack_number
			>=(old_process->max_return_stack_number))
				return NO_RETURN_STACK;
		if((rt_id=os->free_return_stack)<0)
			return NO_RETURN_STACK;
		begin_thread_migration_priority(
			thread_id,old_domain_id,domain_id);
		rt=&(os->os_return_stack[rt_id]);
		os->free_return_stack=rt->next_return_stack;
		rt->next_return_stack=t->return_stack_pointer;
		t->return_stack_pointer=rt_id;
		(old_process->max_return_stack_number)++;
		(os->os_domain[0].max_return_stack_number)--;

		rt->release_domain_id=old_domain_id;

		rt->flag=RETURN_STACK_DEFAULT_FLAG;

		rt->current_memory_body_id=(-1);
	}
	process->enter_thread_number++;
	reset_semaphore_value(
		rt->user_semaphore_id,&(os->system_capability));
	COPY_CAPABILITY((process->capability),
		(os->os_semaphore[rt->user_semaphore_id].capability)); 

	rt->flag&=(~RETURN_STACK_TIME_OUT_FLAG);
	rt->current_thread_id=thread_id;
	rt->current_domain_id=domain_id;
	rt->domain_id=process->domain_id;

	process_p_flag=(old_rt->flag)&RETURN_STACK_PROCESS_P_FLAG;
	if(rt->current_domain_id==old_rt->current_domain_id){
		old_rt->flag|=RETURN_STACK_PROCESS_P_FLAG;
		rt->flag&=(~RETURN_STACK_PROCESS_P_FLAG);
		rt->flag|=process_p_flag;
	}else{
		if(giveup_monitor_flag)
		if((process_p_flag==0)&&(old_process->semaphore>0)){
			sem_value=1;
			if(system_call_v(old_process->semaphore,
			&sem_value,&(old_process->capability))>=0)
				old_rt->flag
					|=RETURN_STACK_PROCESS_P_FLAG;
		}
		rt->flag|=RETURN_STACK_PROCESS_P_FLAG;
	}
	COPY_GENERAL_REGISTER((old_rt->environment.reg),
		(rt->environment.reg));
	if(destination_domain_id==0){
		for(i=0;i<OS_USER_FILE_NUMBER;i++)
			{COPY_USER_FILE_INFORMATION(
				old_rt->file[i],rt->file[i]);}
		COPY_RUN_POINT((old_rt->environment.point),
			(rt->environment.point));
		rt->environment.reg.system_call=CHECK_POINT_REQUEST;
	}else{
		for(i=0;i<OS_USER_FILE_NUMBER;i++)
			{COPY_USER_FILE_INFORMATION(
				process->file[i],rt->file[i]);}
		COPY_RUN_POINT((process->start_point),
			(rt->environment.point));
		rt->environment.reg.system_call=FILE_OPERATION_REQUEST;
	}
	rt->environment.reg.r1=rt->user_semaphore_id;
	if(pb==((struct thread_physical_block *)0)){
		RESET_THREAD_PHYSICAL_BLOCK(rt->physical_block);
	}else{
		COPY_THREAD_PHYSICAL_BLOCK((*pb),(rt->physical_block));
	}
	i=end_thread_migration_priority(
		thread_id,old_domain_id,domain_id);
	modify_thread_priority(thread_id,i,process->cpu_set_id); 
	rt->priority_bak=t->priority;
	rt->cpu_set_bak=t->cpu_set_id;

	rt->signal_bitmap=0;

	return	(rt->environment.reg.system_call);
}
