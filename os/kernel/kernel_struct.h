#ifndef OS_KERNEL_STRUCT
#define OS_KERNEL_STRUCT

struct semaphore_struct{
	int value;
	int owner_domain_id,thread_ring;
	int semaphore_front,semaphore_back;
	CAPABILITY capability;

	int entity_id,entity_type_id;

	int heap_id,v_value,timer_action_flag;
	struct kernel_time first,step;
}; 
#define SEMAPHORE_PROCESS_ENTITY_TYPE		0
#define SEMAPHORE_THREAD_ENTITY_TYPE		1
#define SEMAPHORE_RETURN_STACK_ENTITY_TYPE	2
#define SEMAPHORE_MEMORY_ENTITY_TYPE		3
#define SEMAPHORE_DEVICE_ENTITY_TYPE		4

#define SEMAPHORE_V_OPERATION_TIMER_ACTION		1
#define SEMAPHORE_PROCESS_TIMER_ACTION			2
#define SEMAPHORE_RETURN_STACK_TIMER_ACTION		4

struct general_register{
	int system_call;		/*ax	*/
	int r1,r2,r3,r4,r5,r6,r7;	/*bx,cx,dx,si,di,bp,r7	*/
}; 
#define COPY_GENERAL_REGISTER(s,d)		\
{						\
	(d).system_call=(s).system_call;	\
	(d).r1=(s).r1;				\
	(d).r2=(s).r2;				\
	(d).r3=(s).r3;				\
	(d).r4=(s).r4;				\
	(d).r5=(s).r5;				\
	(d).r6=(s).r6;				\
	(d).r7=(s).r7;				\
} 
struct thread_environment{
	struct general_register reg;
	run_point point;		/*ip,cs,flag,sp,ss */
}; 
struct thread_physical_block {
	int stack_memory_id,stack_block_id;
	int memory_id,block_id;
};
#define COPY_THREAD_PHYSICAL_BLOCK(s,d)			\
{							\
	(d).stack_memory_id	=(s).stack_memory_id;	\
	(d).stack_block_id	=(s).stack_block_id;	\
	(d).memory_id		=(s).memory_id;		\
	(d).block_id		=(s).block_id;		\
}
#define COMPARE_THREAD_PHYSICAL_BLOCK(s,d)			\
(								\
	  (	(s).stack_memory_id ==	(d).stack_memory_id	)\
	&&(	(s).stack_block_id  ==	(d).stack_block_id	)\
	&&(	(s).memory_id       ==	(d).memory_id		)\
	&&(	(s).block_id        ==	(d).block_id		)\
)
#define RESET_THREAD_PHYSICAL_BLOCK(d)		\
{						\
	(d).stack_memory_id	=	(-1);	\
	(d).stack_block_id	=	(-1);	\
	(d).memory_id		=	(-1);	\
	(d).block_id		=	(-1);	\
} 
struct return_stack{
	struct thread_environment environment;

	int flag;
	int current_thread_id;
	int current_domain_id,release_domain_id,domain_id;

	int next_return_stack;
	int user_semaphore_id;

	struct thread_physical_block physical_block;

	int cpu_set_bak,priority_bak;

	struct user_file_information file[OS_USER_FILE_NUMBER];

	int signal_bitmap;
	
	int current_memory_body_id;
}; 
#define RETURN_STACK_DEFAULT_FLAG	(RETURN_STACK_PROCESS_P_FLAG)
#define RETURN_STACK_TIME_OUT_FLAG		1
#define RETURN_STACK_PROCESS_P_FLAG		2
#define RETURN_STACK_LOCK_MEMORY_FLAG		4
#define RETURN_STACK_NO_RETURN_VALUE_FLAG	8

union system_call_parameter ;

struct thread_struct{ 
	int state,flag,priority;
	int heap_id,domain_id,process_front,process_back;

	int semaphore_id,sleep_semaphore_id;
	int semaphore_front,semaphore_back;
	int cpu_set_id,cpu_id;

	int return_stack_pointer;

	int (*system_call_function)(union system_call_parameter *par);

	int physical_cpu_id;
};
#define THREAD_DEFAULT_FLAG				0
#define THREAD_SET_V_OPERATION_RESULT_FLAG	 	1

#define THREAD_SLEEP_STATE	1
#define THREAD_RUN_STATE	2
#define THREAD_READY_STATE	3 

struct thread_heap{
	int thread_id;
}; 
#endif 
