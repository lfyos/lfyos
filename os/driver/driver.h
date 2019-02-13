#include "../kernel/kernel.h"
#include "lru/memory.h"

#ifndef LFYOS_USER_MODE 

extern int operate_device(
	struct file_system_call_parameter *par,int sleep_semaphore);
extern void unlock_device_and_lock_memory(
	int memory_body_id,int device_id);
extern int unlock_memory_and_lock_device(int device_id);
extern void memory_wakeup(int processor,int semaphore);
extern void memory_wait(int sleep_semaphore,
	int next_processor,int next_semaphore);
extern int migrate_to_file_system(int processor_process,
	struct file_system_call_parameter *par,
	struct thread_physical_block *pb);
extern int service_routine(int type_id,int service_id,int par);
extern int other_service_routine(int service_id,int par);

#endif

extern int page_fault_process(int flag,int cr2,int err_code);
extern int memory_main_map_process(int memory_body_id,
	int file_window_id,int begin_logic_address,
	int end_logic_address,int read_only_flag,
	CAPABILITY *file_capability);
extern char *map_memory_block(int body_id,int block_id);
extern void mark_memory_block_unmap(int body_id,int block_id);
extern void lfyos_exit();
