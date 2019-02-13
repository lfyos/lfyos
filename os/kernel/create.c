#include "kernel.h" 
#define  WRONG_OS_DOMAIN_NUMBER 	(-1)
#define  WRONG_CAPABILITY 		(-2)
#define  CREATE_TOO_MANY_THREAD 	(-3)
#define  NO_RETURN_STACK	 	(-4)
#define  NO_THREAD_TO_CREATE 		(-5)
static int get_a_free_thread(int domain_id,int priority,
	CAPABILITY *domain_capability)
{
	int i,thread_id,front_thread,back_thread,rt_id;
	struct return_stack *rt;
	struct thread_struct *t;
	struct processor_domain_struct *process;

	if((domain_id<=0)||(domain_id>=OS_DOMAIN_NUMBER))
		return WRONG_OS_DOMAIN_NUMBER;
	process=&(os->os_domain[domain_id]);
	if(!KERNEL_COMPARE_CAPABILITY(
		(process->thread_create_capability),
		(*domain_capability)))
			return WRONG_CAPABILITY;
	if((process->thread_number)>=(process->max_thread_number))
		return CREATE_TOO_MANY_THREAD;
	if((thread_id=os->os_domain[0].thread_ring)<0)
		return NO_THREAD_TO_CREATE;
	if((process->return_stack_number)
		>=(process->max_return_stack_number))
			return NO_RETURN_STACK;
	if((rt_id=os->free_return_stack)<0)
		return NO_RETURN_STACK;
	for(i=0;;i++){
		if(i>=(os->os_domain[0].thread_number))
			return NO_THREAD_TO_CREATE;
		t=&(os->os_thread[thread_id]); 
		if(lock_os_thread(thread_id))
			break;
		thread_id=t->process_back;
	}
	rt=&(os->os_return_stack[rt_id]);
	os->free_return_stack=rt->next_return_stack;
	rt->next_return_stack=(-1);
	rt->current_thread_id=thread_id;
	t->return_stack_pointer=rt_id;

	rt->release_domain_id=domain_id;

	rt->signal_bitmap=0;

	if((t->process_front)==thread_id)
		os->os_domain[0].thread_ring=(-1);
	else{
		os->os_domain[0].thread_ring=t->process_back;
		os->os_thread[t->process_front].process_back
			=t->process_back;
		os->os_thread[t->process_back].process_front
			=t->process_front;
	}
	if((process->thread_ring)<0){
		t->process_front=thread_id;
		t->process_back=thread_id;
		process->thread_ring=thread_id;
	}else{
		front_thread=process->thread_ring;
		back_thread=os->os_thread[front_thread].process_back;
		os->os_thread[front_thread].process_back=thread_id;
		os->os_thread[back_thread].process_front=thread_id;
		os->os_thread[thread_id].process_front=front_thread;
		os->os_thread[thread_id].process_back=back_thread;
	}
	t->system_call_function=(int (*)())0;

	process->thread_number++;
	os->os_domain[0].thread_number--;
	process->return_stack_number++;
	os->os_domain[0].return_stack_number--;

	return thread_id;
}
static void set_thread_item(int thread_id,
	struct user_file_information *return_stack_file_info,
	int domain_id,int current_process,
	struct thread_environment *thread_environment)
{
	int i;
	struct return_stack *rt;
	struct thread_struct *t;

	t=&(os->os_thread[thread_id]);
	rt=&(os->os_return_stack[t->return_stack_pointer]);

	t->domain_id=domain_id; 
	t->flag=THREAD_DEFAULT_FLAG;

	rt->current_memory_body_id=(-1);

	if(((int)return_stack_file_info)==0)
		return_stack_file_info
			=&(os->os_domain[current_process].file[0]);
	for(i=0;i<OS_USER_FILE_NUMBER;i++,return_stack_file_info++)
		COPY_USER_FILE_INFORMATION((*return_stack_file_info),
			(rt->file[i]));

	if(thread_environment!=((struct thread_environment *)0)){
		COPY_GENERAL_REGISTER(((thread_environment)->reg),
			((rt->environment).reg));
		COPY_RUN_POINT((thread_environment->point),
			(rt->environment.point));
	}
	rt->current_domain_id=current_process;
	os->os_domain[current_process].enter_thread_number++;
	rt->domain_id=os->os_domain[current_process].domain_id;
	rt->flag=RETURN_STACK_DEFAULT_FLAG;

	RESET_THREAD_PHYSICAL_BLOCK(rt->physical_block);

	rt->cpu_set_bak=t->cpu_set_id;
	rt->priority_bak=t->priority;

	if(thread_environment!=((struct thread_environment *)0)){
		thread_environment->reg.system_call=thread_id;
		thread_environment->reg.r1=0;
		thread_environment->reg.r2=rt->user_semaphore_id;
	};
	rt->environment.reg.system_call=thread_id;
	rt->environment.reg.r1=1;
	rt->environment.reg.r2=rt->user_semaphore_id;

	return;
}
int create_thread(struct user_file_information *return_stack_file_info,
	int domain_id,int current_process,int priority,int cpu_set_id,
	struct thread_environment *thread_environment,
	CAPABILITY *domain_capability)
{
	int thread_id;
	struct return_stack *rt;
	struct thread_struct *t;
	struct processor_domain_struct *p;

	if((thread_id=get_a_free_thread(
		domain_id,priority,domain_capability))<0)
			return thread_id;
	t=&(os->os_thread[thread_id]);
	t->priority=priority;
	t->cpu_set_id=cpu_set_id;
	set_thread_item(thread_id,return_stack_file_info,
		domain_id,current_process,thread_environment);
	os->os_semaphore[0].value=(-OS_THREAD_NUMBER);
	v_thread(thread_id,FALSE);

	rt=&(os->os_return_stack[t->return_stack_pointer]);
	p=&(os->os_domain[rt->current_domain_id]); 

	COPY_CAPABILITY((os->system_capability),
		(os->os_semaphore[t->semaphore_id].capability));
	COPY_CAPABILITY((p->capability),
		(os->os_semaphore[rt->user_semaphore_id].capability));
	reset_semaphore_value(t->semaphore_id,&(os->system_capability));
	reset_semaphore_value(
		rt->user_semaphore_id,&(os->system_capability));

	unlock_os_thread(thread_id);

	return thread_id;
}
