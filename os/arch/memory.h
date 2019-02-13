#ifndef OS_MEMORY_ARCH
#define OS_MEMORY_ARCH

#define PAGE_SIZE		(0x1000)
#define MAX_FILE_WINDOW_LENGTH	(512*1024*1024)

#define READ_ONLY_RIGHT		(0x05)
#define READ_WRITE_RIGHT	(0x07)
#define NO_ACCESS_RIGHT		(0x00)

#define MAKE_READ_ONLY_RIGHT(right)	(((right)&0xfffffff8)|0x05)
#define MAKE_READ_WRITE_RIGHT(right)	(((right)&0xfffffff8)|0x07)
#define IS_READ_WRITE_RIGHT(right)	((((right)&0x07)==0x07))

struct file_system_call_parameter ;
extern int call_device_driver(
	int (*control)(struct file_system_call_parameter *par,
		int sleep_semaphore),
	struct file_system_call_parameter *par,int sleep_semaphore);
extern int memory_set_tlb_item(int memory_body_id,int file_window,
	int logic_block_number,int physical_block_id,int right,
	int block_id,int *free_block_id);
extern int memory_clear_tlb_item(int memory_body_id,int file_window,
	int logic_block_number,int block_id,int *free_block_id);
extern int memory_need_clear_tlb_item(int memory_body_id); 
extern void mount_page_table(int memory_body_id,int file_id,
	int map_length,CAPABILITY *file_capability);
extern void unmount_page_table(int memory_body_id,int file_id);
static inline void remote_unlock_memory_body(int memory_body_id)
	{asm volatile("cli\n\t");}
static inline void remote_lock_memory_body(int memory_body_id)
	{asm volatile("sti\n\t");}
#endif 
