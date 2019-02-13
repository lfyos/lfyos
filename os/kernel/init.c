#include"kernel.h" 
static void init_file_information()
{
	int i,j;

	for(j=0;j<OS_USER_FILE_NUMBER;j++)
		RESET_USER_FILE_INFORMATION(os->os_file[j]);
	for(i=0;i<OS_DOMAIN_NUMBER;i++)
		for(j=0;j<OS_USER_FILE_NUMBER;j++)
			RESET_USER_FILE_INFORMATION(
				os->os_domain[i].file[j]);
	for(i=0;i<OS_RETURN_STACK_NUMBER;i++)
		for(j=0;j<OS_USER_FILE_NUMBER;j++)
			RESET_USER_FILE_INFORMATION(
				os->os_return_stack[i].file[j]);
	return ;
} 
static void init_return_stack()
{
	int i;
	struct return_stack *rt;

	for(i=0;i<OS_RETURN_STACK_NUMBER;i++){
		rt=&(os->os_return_stack[i]);

		RESET_RUN_POINT(rt->environment.point);
		rt->current_thread_id=(-1);
		rt->current_domain_id=0;
		rt->domain_id=0;
		rt->flag=RETURN_STACK_DEFAULT_FLAG;
		rt->next_return_stack=i+1;
		rt->user_semaphore_id=(-1);

		RESET_THREAD_PHYSICAL_BLOCK(rt->physical_block);
		
		rt->signal_bitmap=0;

		rt->current_memory_body_id=(-1);
	}
	i=OS_RETURN_STACK_NUMBER-1;
	os->os_return_stack[i].next_return_stack=(-1);
	os->free_return_stack=0;

	return ;
} 
static void init_process()
{
	int i,j;

	for(i=0;i<OS_DOMAIN_NUMBER;i++){
		RESET_RUN_POINT((os->os_domain[i].start_point));

		os->os_domain[i].priority_policy=PROCESS_DEFAULT_PRIORITY;
		os->os_domain[i].semaphore=(-1);
		os->os_domain[i].cpu_set_id=0;

		os->os_domain[i].enter_thread_number=0;
		os->os_domain[i].domain_id=0;

		os->os_domain[i].max_thread_number=0;
		os->os_domain[i].thread_number=0;
		os->os_domain[i].thread_ring=(-1);

		os->os_domain[i].max_semaphore_number=0;
		os->os_domain[i].semaphore_number=0;
		os->os_domain[i].semaphore_ring=(-1);

		os->os_domain[i].max_return_stack_number=0;
		os->os_domain[i].return_stack_number=0;

		for(j=0;j<OS_USER_FILE_NUMBER;j++){
			RESET_USER_FILE_INFORMATION(
				os->os_domain[i].file[j]);
		}
		RESET_CAPABILITY(os->os_domain[i].capability);
		RESET_CAPABILITY(os->os_domain[i].
			thread_create_capability);
	}
	os->os_domain[0].max_thread_number=OS_THREAD_NUMBER-1;
	os->os_domain[0].thread_number=OS_THREAD_NUMBER-1;
	os->os_domain[0].thread_ring=1;
	os->os_domain[0].max_semaphore_number=OS_SEMAPHORE_NUMBER-1;
	os->os_domain[0].semaphore_number=OS_SEMAPHORE_NUMBER-1;
	os->os_domain[0].semaphore_ring=1;
	os->os_domain[0].max_return_stack_number=OS_RETURN_STACK_NUMBER;
	os->os_domain[0].return_stack_number=0;

	os->os_domain[1].enter_thread_number=1;
	os->os_domain[1].max_thread_number=1;
	os->os_domain[1].thread_number=1;
	os->os_domain[1].thread_ring=0;
	os->os_domain[1].semaphore_number=0;
	os->os_domain[1].max_semaphore_number=1;
	os->os_domain[1].max_return_stack_number=5;
	os->os_domain[1].return_stack_number=1;

	return ;
} 
static void init_semaphore()
{
	int i;

	for(i=0;i<OS_SEMAPHORE_NUMBER;i++){
		os->os_semaphore[i].value=0;
		os->os_semaphore[i].owner_domain_id=0;
		os->os_semaphore[i].thread_ring=-1;
		os->os_semaphore[i].semaphore_front=i-1;
		os->os_semaphore[i].semaphore_back=i+1;
		RESET_CAPABILITY(os->os_semaphore[i].capability);
		os->os_semaphore[i].heap_id=i;
		os->os_semaphore[i].v_value=0;
		os->os_semaphore[i].timer_action_flag=0;
		os->os_semaphore_heap[i].semaphore_id=i;
		SET_MOST_TIME(os->os_semaphore[i].first);
		SET_MINIMAL_STEP(os->os_semaphore[i].step);

		os->os_semaphore[i].entity_id=(-1);
		os->os_semaphore[i].entity_type_id=(-1);
	}
	os->os_semaphore[OS_SEMAPHORE_NUMBER-1].semaphore_back=1;
	os->os_semaphore[1].semaphore_front=OS_SEMAPHORE_NUMBER-1;

	os->os_semaphore[0].value=1-OS_THREAD_NUMBER;
	os->os_semaphore[0].thread_ring=OS_THREAD_NUMBER-1;
	os->os_semaphore[0].semaphore_front=0;
	os->os_semaphore[0].semaphore_back=0;

	return ;
} 
static void init_thread()
{
	int i;

	for(i=0;i<OS_THREAD_NUMBER;i++){

		os->os_thread[i].state=THREAD_SLEEP_STATE;

		os->os_thread[i].priority=0;
		os->os_thread[i].flag=THREAD_DEFAULT_FLAG;
		os->os_thread[i].heap_id=(-1);
		os->os_thread[i].domain_id=0;
		os->os_thread[i].process_front=i-1;
		os->os_thread[i].process_back=i+1;

		os->os_thread[i].semaphore_id=(-1);
		os->os_thread[i].sleep_semaphore_id=0;
		os->os_thread[i].semaphore_front=i-1;
		os->os_thread[i].semaphore_back=i+1;
	
		os->os_thread[i].cpu_set_id=0;
		os->os_thread[i].cpu_id=(-1);

		os->os_thread[i].return_stack_pointer=(-1);
		os->os_thread[i].system_call_function=
			(int (*)(union system_call_parameter *))0;

		os->os_thread[i].physical_cpu_id=(-1);
	};
	os->os_thread[1].semaphore_front=OS_THREAD_NUMBER-1;
	os->os_thread[OS_THREAD_NUMBER-1].semaphore_back=1;
	os->os_thread[1].process_front=OS_THREAD_NUMBER-1;
	os->os_thread[OS_THREAD_NUMBER-1].process_back=1;

	return ;
}
static void init_cpu()
{
	int i;

	for(i=1;i<OS_CPU_NUMBER;i++){
		os->os_cpu[i].thread_id=(-1);
		os->os_cpu[i].cpu_set_id=0;
		os->os_cpu[i].stack_location=i-1;
	}
	os->os_cpu[0].thread_id=0;
	os->os_cpu[0].cpu_set_id=0;
	os->os_cpu[0].stack_location=(-1);

	for(i=0;i<OS_CPU_SET_NUMBER;i++){
		os->os_cpu_set[i].thread_heap.run_thread_number=0;
		os->os_cpu_set[i].thread_heap.ready_thread_number=0;
	}
	os->os_cpu_set[0].thread_heap.run_thread_number=1;
	os->os_cpu_set[0].thread_heap.run_heap[0].thread_id=0;

	for(i=0;i<OS_CPU_SET_NUMBER;i++)
		os->os_cpu_set[i].free_cpu.free_cpu_top=0;

	for(i=1;i<OS_CPU_NUMBER;i++)
		os->os_cpu_set[0].free_cpu.free_cpu[i-1].cpu_id=i;

	os->os_cpu_set[0].free_cpu.free_cpu_top=OS_CPU_NUMBER-1;

	return ;
}
static void init_thread_0()
{
	struct thread_struct *t;
	struct return_stack *rt;

	t=&(os->os_thread[0]);

	t->state=THREAD_RUN_STATE;
	t->domain_id=1;
	t->heap_id=0;
	t->cpu_set_id=0;
	t->cpu_id=0;
	t->sleep_semaphore_id=-1;
	t->semaphore_front=-1;
	t->semaphore_back=-1;
	t->process_front=0;
	t->process_back=0;
	t->return_stack_pointer=os->free_return_stack;
	rt=&(os->os_return_stack[t->return_stack_pointer]);

	os->free_return_stack=rt->next_return_stack;
	rt->next_return_stack=(-1);

	RESET_START_RUN_POINT(rt->environment.point);

	rt->current_domain_id=1;
	rt->release_domain_id=1;
	rt->domain_id=0;
	rt->flag=RETURN_STACK_DEFAULT_FLAG;
	if(((rt->flag)&RETURN_STACK_PROCESS_P_FLAG)==0)
		rt->flag=((rt->flag)|RETURN_STACK_PROCESS_P_FLAG);
	rt->user_semaphore_id=(-1);

	RESET_THREAD_PHYSICAL_BLOCK(rt->physical_block);

	return ;
}
static void init_thread_semaphore()
{
	int i,sem_id;
	struct thread_struct *t;
	struct return_stack *rt;

	for(i=0;i<OS_THREAD_NUMBER;i++){
		t=&(os->os_thread[i]);
		sem_id=allocate_semaphore(
			os->os_thread[0].domain_id,
			0,&(os->system_capability),
			&(os->system_capability));
		t->semaphore_id=sem_id; 
		os->os_semaphore[sem_id].entity_type_id=
			SEMAPHORE_THREAD_ENTITY_TYPE;
		os->os_semaphore[sem_id].entity_id=i; 
		os->os_domain[1].max_semaphore_number++;
	}
	for(i=0;i<OS_RETURN_STACK_NUMBER;i++){
		rt=&(os->os_return_stack[i]);
		sem_id=allocate_semaphore(
			os->os_thread[0].domain_id,
			0,&(os->system_capability),
			&(os->system_capability));
		rt->user_semaphore_id=sem_id;
		os->os_semaphore[sem_id].entity_type_id=
			SEMAPHORE_RETURN_STACK_ENTITY_TYPE;
		os->os_semaphore[sem_id].entity_id=i; 
		os->os_domain[1].max_semaphore_number++;
	}
	return ;
} 
static void init_process_semaphore()
{
	int i,sem_id;

	os->os_domain[0].semaphore=(-1);
	for(i=1;i<OS_DOMAIN_NUMBER;i++){
		sem_id=allocate_semaphore(
			os->os_thread[0].domain_id,
			OS_RETURN_STACK_NUMBER,&(os->system_capability),
			&(os->system_capability));
		os->os_domain[i].semaphore=sem_id;
		os->os_semaphore[sem_id].entity_type_id=
			SEMAPHORE_PROCESS_ENTITY_TYPE;
		os->os_semaphore[sem_id].entity_id=i; 
		os->os_domain[1].max_semaphore_number++;
	}
	return ;
} 
static void init_driver_semaphore()
{
	int i,sem_id;
	for(i=0;i<driver_number;i++){
		driver[i].memory_spin_lock=1;
		driver[i].device_spin_lock=1;

		os->os_domain[1].max_semaphore_number++;
		sem_id=allocate_semaphore(os->os_thread[0].domain_id,0,
		     &(os->system_capability),&(os->system_capability));
		driver[i].memory_semaphore_id=sem_id;

		os->os_semaphore[sem_id].entity_type_id=
			SEMAPHORE_MEMORY_ENTITY_TYPE;
		os->os_semaphore[sem_id].entity_id=i;

		os->os_domain[1].max_semaphore_number++;
		sem_id=allocate_semaphore(os->os_thread[0].domain_id,0,
		     &(os->system_capability),&(os->system_capability));
		driver[i].device_semaphore_id=sem_id;

		os->os_semaphore[sem_id].entity_type_id=
			SEMAPHORE_DEVICE_ENTITY_TYPE;
		os->os_semaphore[sem_id].entity_id=i; 
	}
	return ;
}
void init_spinlock()
{
	int i;

	os->spinlock.os_lock=1;
	for(i=0;i<OS_THREAD_NUMBER;i++)
		os->spinlock.thread_lock[i]=1;
	return;
}
void os_init(int processor_flag)
{
	if(processor_flag){
		RESET_CAPABILITY(os->system_capability);

		init_spinlock();
		init_file_information();
		init_return_stack();
		init_process();
		init_semaphore();
		init_thread();
		init_thread_0();
		init_cpu();
		init_thread_semaphore();
		init_process_semaphore();
		init_driver_semaphore();

		start_all_processors();
	
		driver_number=0-driver_number;
	}
	start_point_for_all_processors();
	return ;
}
