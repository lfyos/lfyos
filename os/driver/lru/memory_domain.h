#ifndef OS_MEMORY_MEMORY_PROCESS
#define OS_MEMORY_MEMORY_PROCESS 
struct block_trigger{
	int write_block_number,free_block_number;
	int system_write_block_number,system_free_block_number;
};
struct memory_sleep_semaphore{
	int processor,semaphore;
};
struct memory_domain_struct{
	int flag;
	int file_number,max_file_number,file_ring;
	int trigger_close_file_number;
	int read_block_number,write_block_number;
	int max_block_number,block_number;
	int physical_max_block_number,physical_block_number;
	struct block_trigger trigger,physical_trigger;
	int block_ring,read_block_ring,write_block_ring;
	CAPABILITY capability;
};
#define MEMORY_PROCESS_FILE_FLUSH_FLAG		1
#define MEMORY_PROCESS_DEFAULT_FLAG		0

#endif
