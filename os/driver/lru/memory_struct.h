#ifndef OS_MEMORY_STRUCT
#define OS_MEMORY_STRUCT

#define FILE_OPEN	1
#define FILE_CLOSE	2

#define FILE_WINDOW_SWAP_OUT_FLAG	1
#define FILE_WINDOW_COPY_ON_WRITE_FLAG	2
#define FILE_WINDOW_NO_WAIT_FLAG	4
#define FILE_WINDOW_DEFAULT_FLAG	\
	(FILE_WINDOW_SWAP_OUT_FLAG|FILE_WINDOW_COPY_ON_WRITE_FLAG)

struct file_window{
	struct kernel_file_window kernel_file_window;

	int state,flag;
	int memory_domain_id;
	int block_ring,block_number;
	int file_front,file_back;
	int hash_front,hash_back;
	int lock_number;
};
struct physical_block{
	int physical_id;
}; 
struct memory_block{
	int block_state,block_flag;

	int lock_number;
	int file_window_id,logic_block_id;
	int physical_id;
	int file_front,file_back;
	int buf_front,buf_back;
	int hash_front,hash_back;
	struct memory_sleep_semaphore sleep_semaphore;

	int parent_block_id,first_child_block_id;
	int left_brother_block_id,right_brother_block_id;
	int next_block_id;
};
#define FREE_BLOCK		0
#define READING_BLOCK		1
#define NOT_MODIFIED_BLOCK	2
#define MODIFIED_BLOCK		3
#define WRITING_BLOCK		4

struct memory_hash{
	int block_hash_ring,file_hash_ring;
}; 
struct memory_body_head_struct{ 
	int free_block_number,free_physical_block,free_block,free_file;
	int physical_block_number,block_number;
	int file_number,memory_domain_number,hash_number;
};
struct memory_body_struct{
	struct memory_body_head_struct *head;

	struct physical_block *physical_block;
	struct memory_block *memory_block;
	struct file_window *file_window;
	struct memory_domain_struct *memory_domain;
	struct memory_hash *hash;

	int (*block_hash_function)(int file,int logic_block);
	int (*file_hash_function)(
		struct kernel_file_window *kernel_file_window);

	int my_processor,my_memory_body;
	struct memory_sleep_semaphore *wait_block; 
}; 
struct install_memory_body_parameter{
	int (*block_hash_function)(int file,int logic_block);
	int (*file_hash_function)(
		struct kernel_file_window *kernel_file_window);
	int my_processor,my_memory_body; 
	int block_number,file_number,memory_domain_number,hash_number; 
	CAPABILITY capability;
}; 
#endif
