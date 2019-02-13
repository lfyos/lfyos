#ifndef OS_DRIVER_STRUCT
#define OS_DRIVER_STRUCT 
union memory_call_parameter ; 
struct driver{
	int memory_body_capacity;
	void *memory_body_pointer;

	char *(*get_memory_body_base)();
	char *(*driver_map_memory_block)(int block_id);
	void (*driver_mark_memory_block_unmap)(int block_id);
	void (*initialize)(struct driver *dr);
	int (*memory_call_routine)(int routine_id,
		union memory_call_parameter *par); 
	int (*device_driver_routine)(
		struct file_system_call_parameter *par,int semaphore);
	void (*request_complete)(int reply_code);

	int memory_semaphore_id,memory_spin_lock;
	int device_semaphore_id,device_spin_lock;
};
extern struct driver driver[];
extern int driver_number;

extern char *map_memory_block(int body_id,int block_id);
extern void mark_memory_block_unmap(int body_id,int block_id);
#endif
