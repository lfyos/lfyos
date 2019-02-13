#ifndef OS_FUNCTION_CALL
#define OS_FUNCTION_CALL

extern int get_virtual_cpu(int switch_virtual_cpu_flag); 
extern int lock_os_kernel();
extern void unlock_os_kernel();
extern int lock_os_thread(int thread_id);
extern void unlock_os_thread(int thread_id); 
extern int local_create_thread_priority(int thread_id,int domain_id);
extern int remote_create_thread_priority(int priority,int domain_id);
extern int shutdown_create_thread_priority(int domain_id);
extern int timeout_create_thread_priority(int domain_id);
extern int min_thread_priority(
	int thread_id,int domain_id,CAPABILITY *cap); 
extern void begin_thread_migration_priority(
	int thread_id,int current_domain_id,int domain_id);
extern int end_thread_migration_priority(
	int thread_id,int current_domain_id,int domain_id); 
extern void begin_thread_return_priority(int thread_id);
extern void thread_return_priority(int thread_id);
extern int end_thread_return_priority(int thread_id); 
extern void thread_begin_p_semaphore_priority(
	int thread_id,int semaphore_id);
extern int thread_end_p_semaphore_priority(
	int thread_id,int semaphore_id); 
extern void thread_begin_v_semaphore_priority(
	int thread_id,int semaphore_id);
extern int thread_end_v_semaphore_priority(
	int thread_id,int semaphore_id);
extern void time_share_priority(int current_thread_id);
extern int set_system_file_capability(int system_file_id,
	int map_length,int right,CAPABILITY *file_capability,
	int memory_body_id,int file_id);
extern int set_return_stack_file_capability(
	int thread_id,int thread_file_id,int map_length,int right,
	CAPABILITY *file_capability,int memory_body_id,int file_id); 
extern int set_all_return_stack_file_capability(int thread_id,
	int *result,struct user_file_information *par); 
extern void leave_run_state(int thread_id);
extern void enter_run_state(int thread_id); 
extern void ready_heap_up_deal(int thread_id);
extern void ready_heap_down_deal(int thread_id);
extern void remove_from_ready_heap(int thread_id);
extern void insert_into_ready_heap(int thread_id); 
extern void run_heap_up_deal(int thread_id);
extern void run_heap_down_deal(int thread_id);
extern void remove_from_run_heap(int thread_id);
extern void insert_into_run_heap(int thread_id); 
extern void remove_from_heap(int thread_id);
extern void insert_into_heap(int thread_id); 
extern void modify_thread_priority(
	int thread_id,int new_priority,int new_cpu_set_id); 
extern void move_semaphore(int semaphore_id,int destination_process);
extern void kernel_semaphore_time_process();
extern void process_semaphore_action(int semaphore_id);
extern void set_semaphore_time(int semaphore_id,
	struct kernel_time *time_new,int semaphore_action_flag); 
extern int reset_semaphore_value(int sem_id,
	CAPABILITY *semaphore_capability); 
extern int thread_migrate(int thread_id,int destination_domain_id,
	int stack_flag,int giveup_monitor_flag,
	struct thread_physical_block *pb);
extern int give_up_memory_body(struct return_stack *rt);
extern int thread_return(int thread_id,int return_code); 
extern int p(int thread_id,int semaphore_id,
	int sleep_flag,int result_flag,CAPABILITY *cap);
extern int system_call_p(int thread_id,int semaphore_id,
	int sleep_flag,int giveup_monitor_flag,CAPABILITY *cap);
extern int v_thread(int thread_id,int time_out_flag);
extern int system_call_v(int semaphore_id,int *steps,CAPABILITY *cap); 
extern int set_semaphore_time_system_call(int semaphore_id,int v_value,
	int semaphore_action_flag,struct kernel_time *first_time,
	struct kernel_time *step_time,CAPABILITY *sem_capability); 
extern int set_semaphore_capability_time_and_p_operation(
	int thread_id,int semaphore_id,int do_p_flag,
	int sleep_flag,int giveup_monitor_flag,
	int semaphore_action_flag,int v_value,
	struct kernel_time *first_time,struct kernel_time *step_time,
	CAPABILITY *old_semaphore_capability,
	CAPABILITY *new_semaphore_capability); 
extern int allocate_resource(struct processor_domain_struct *process,
	int domain_id,CAPABILITY *sysetm_capability);
extern int query_resource(
	struct processor_domain_struct*process,int domain_id);
extern int create_thread(
	struct user_file_information *return_stack_file_info,
	int domain_id,int current_process,int priority,int cpu_set_id,
	struct thread_environment *thread_environment,
	CAPABILITY *domain_capability); 
extern int create_request_thread(
	struct thread_physical_block *system_pb); 
extern int change_cpu_set(
	int cpu_id,int new_cpu_set_id,CAPABILITY *system_capability); 
extern int allocate_semaphore(int domain_id,int value,
	CAPABILITY*domain_capability,CAPABILITY*semaphore_capability);
extern int release_semaphore(
	int sem_id,CAPABILITY *semaphore_capability); 
extern int set_thread_attribute(int thread_id,
	struct thread_set_attribute *attr);
extern int get_thread_attribute(int thread_id,
	struct thread_get_attribute *attr);
extern int send_signal_to_thread(int sender_id,int receiver_id,
	int *signal_bitmap,CAPABILITY *cap); 
extern int shutdown_process(int *success_number,int *fail_number);
extern void os_init(int processor_flag);
extern struct thread_environment *system_call(
	int (*system_call_function)(),int system_call_flag); 
extern void set_current_item(int thread_id);
extern void start_point_for_all_processors(); 
extern void start_device_routine(); 
extern void start_server_routine(struct general_register *env);
extern int server_interruption_routine(
	int copy_flag,struct thread_physical_block *pb); 
extern int time_call_process(union system_call_parameter *par);
extern int system_call_process(union system_call_parameter *par);
#endif 
