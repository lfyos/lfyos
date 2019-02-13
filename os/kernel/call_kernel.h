#ifndef OS_KERNEL_SYSTEM_CALL
#define OS_KERNEL_SYSTEM_CALL 
struct thread_set_attribute { 
	int priority,cpu_set_id,signal_bitmap;
	int mandatory_exit_number; 
	CAPABILITY current_domain_capability;
};
struct thread_get_attribute{
	struct return_stack return_stack_data;
	struct thread_struct thread_data;
	CAPABILITY user_semaphore_capability,current_domain_capability;
};
union system_call_parameter{
	struct {
		CAPABILITY capability_1,capability_2;
	}capability;
	struct {
		struct processor_domain_struct domain_data;
		CAPABILITY capability;
	}domain_attribute;
	struct thread_physical_block thread_physical_block;
	struct thread_set_attribute set_thread_attribute;
	struct thread_get_attribute get_thread_attribute;
	struct {
		struct kernel_time first_time,step_time;

		CAPABILITY old_semaphore_capability;
		CAPABILITY new_semaphore_capability;
	}set_semaphore_time;
	struct semaphore_struct semaphore_attribute;
	struct{
		struct user_file_information file[OS_USER_FILE_NUMBER];
		int set_flag_and_result[OS_USER_FILE_NUMBER];
	}set_user_file_information;
	struct set_root_directory{
		struct kernel_file_window kernel_file_window;
		CAPABILITY system_capability;
	}set_root_directory;
}; 
#endif 
