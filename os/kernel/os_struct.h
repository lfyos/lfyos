#ifndef OS_OS_STRUCT
#define OS_OS_STRUCT

struct os_data{
	struct {
		int os_lock;
		int thread_lock[OS_THREAD_NUMBER];
	}spinlock;
	struct {
		int thread_id,cpu_set_id,stack_location;
	}os_cpu[OS_CPU_NUMBER];
	struct{
		struct{
			struct{
				int cpu_id;
			}free_cpu[OS_CPU_NUMBER];
			int free_cpu_top;
		}free_cpu;
		struct{
			struct thread_heap run_heap[OS_CPU_NUMBER];
			struct thread_heap ready_heap[OS_THREAD_NUMBER];
			int run_thread_number,ready_thread_number;
		}thread_heap;
	}os_cpu_set[OS_CPU_SET_NUMBER];
	struct thread_struct os_thread[OS_THREAD_NUMBER];
	struct return_stack os_return_stack[OS_RETURN_STACK_NUMBER];
	int free_return_stack;
	struct processor_domain_struct os_domain[OS_DOMAIN_NUMBER];
	struct semaphore_struct os_semaphore[OS_SEMAPHORE_NUMBER];
	struct{
		int semaphore_id;
	}os_semaphore_heap[OS_SEMAPHORE_NUMBER];
	struct user_file_information os_file[OS_USER_FILE_NUMBER]; 
	CAPABILITY system_capability;
};
struct current_information {
	int thread_id;

	struct thread_environment *env;
	struct return_stack *rt;
	struct thread_struct *t;
	struct processor_domain_struct *current_process;
	struct user_file_information *file_pointer[OS_USER_FILE_NUMBER];
	int file_change_flag;
	struct {
		int return_stack_id,thread_id,domain_id;
		struct thread_physical_block physical_block;
		struct user_file_information file[OS_USER_FILE_NUMBER];
	}bak;
}; 
#endif 
