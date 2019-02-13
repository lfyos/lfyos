#ifndef OS_MEMORY_FUNCTION
#define OS_MEMORY_FUNCTION

extern int init_memory_body_routine(struct memory_body_struct *b,
	int flag,struct install_memory_body_parameter *par,
	char *base,int capacity);
extern int wait_at_block(int block_id,int sleep_semaphore);
extern void wakeup_block(int block_id);
extern int request_physical_id();
extern void release_physical_id(int physical_id);
extern int get_free_block(int memory_domain_number);
extern void free_free_block(int memory_domain_number,int block_number);
extern int mount_physical_block(
	struct memory_block *b,struct memory_domain_struct *p);
extern void dismount_physical_block(
	struct memory_block *b,struct memory_domain_struct *p); 
extern void clear_block_number(int block_id);
extern void free_one_block(int block_number); 
extern int should_prepare_write(struct memory_domain_struct *p,
	int *can_write,int *should_free);
extern int prepare_free_block(int memory_domain,int sleep_semaphore);
extern int copy_on_write(int bn,int root_id);
extern int search_block(int file_window_id,int logic_block_id);
extern int search_read_block(int file_window_id,int logic_address,
	int sleep_semaphore,int read_only_flag,int *root_block_id);
extern int mount_reading_block(int file_window_id,
	int *logic_address,int read_flag);
extern void unmount_reading_block(int bn,int read_fail_flag);
extern int mount_wait_reading_block(int block_id,int wait_queue);
extern int read_one_block(int sleep_semaphore,int *repeat_flag,
	int *file_window_id,int *logic_address,int *block_wait_queue);
extern int read_write_file_system(int command,struct memory_block *b,
	int*begin_address,int sleep_semaphore);
extern int operate_file_system(int command,int flag,int memory_id,
	int physical_number,int begin_address,int data_length,
	int memory_domain_id,int sleep_semaphore,
	struct kernel_file_window *file); 
extern int search_open_file(
	struct kernel_file_window *f,int flag,int memory_domain);
extern void mount_file(int file_id,struct memory_domain_struct *p);
extern int open_file_window(
	struct kernel_file_window *kernel_file_window,
	int flag,int memory_domain,CAPABILITY *domain_capability,
	int sleep_semaphore);
extern int close_file_window(int file_window_id,int giveup_flag,
	int wait_flag,CAPABILITY *file_capability,
	int sleep_semaphore,int *flush_number); 
extern int flush_block(int block_number,int sleep_semaphore,
       int giveup_flag,int free_block_flag,int wait_flag,int parent_bn);
extern int flush_memory_domain(int max_flush_number,
	int giveup_flag,int free_block_flag,int wait_flag,
	int domain_id,int sleep_semaphore,
	CAPABILITY *domain_capability,int *flush_number);
extern int flush_file_window(int file_window_id,
	int update_file_window_id,int max_flush_number,
	int giveup_flag, int free_block_flag,int wait_flag,
	int sleep_semaphore,CAPABILITY *file_capability,
	CAPABILITY *update_file_capability,int *flush_number); 
extern int memory_setup_map(int semaphore_id,
	int file_window_id,int begin_address,int end_address,
	int read_only_flag,CAPABILITY *file_capability);
extern void mark_file_accessed(int file_id);
extern void mark_block_accessed(int block_number); 
extern int query_memory_resource(
	int memory_domain_id,struct memory_domain_struct *r);
extern int allocate_memory_resource(
	int memory_domain_id,struct memory_domain_struct *r); 
extern int control_opened_file(int sleep_semaphore,int memory_id,
	int block_id,int command,int file_window_id,int begin_address,
	int data_length,int flag,CAPABILITY *file_capability);
extern int control_unopened_file(int command,int flag,int begin_address,
	int data_length,int memory_id,int block_id,
	struct kernel_file_window *file,int memory_domain,
	int sleep_semaphore,CAPABILITY *domain_capability); 
extern int set_query_file_window_attribute(int set_or_query_flag,
	int file_window_id,struct kernel_file_window *file_window);
extern int wakeup_file_window(int file_window_id,
	int no_wait_flag,int max_number,CAPABILITY *file_capability);
extern int wakeup_memory_domain(int domain_id,int no_wait_flag,int max_number,
	int max_file_number,CAPABILITY *domain_capability);
extern int memory_call_process(int routine_id,
	union memory_call_parameter *par); 
extern int compute_block_hash_function(int file,int logic_block);
extern int compute_file_hash_function(struct kernel_file_window *k_f_w);

#endif
