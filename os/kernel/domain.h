#ifndef OS_PROCESS_STRUCT
#define OS_PROCESS_STRUCT

struct processor_domain_struct{
	run_point start_point;
	int priority_policy;
	int semaphore,cpu_set_id;

	int enter_thread_number,domain_id;

	int max_thread_number,thread_number,thread_ring;
	int max_semaphore_number,semaphore_number,semaphore_ring;
	int max_return_stack_number,return_stack_number;

	struct user_file_information file[OS_USER_FILE_NUMBER];
	CAPABILITY capability,thread_create_capability;
};
#define PROCESS_DEFAULT_PRIORITY	0

#endif 
