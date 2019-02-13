#include"../../kernel/kernel.h"
#include"memory.h"
int compute_block_hash_function(int file,int logic_block)
{
	int hash_id=memory_body->block_hash_function(file,logic_block);
	int max_id=memory_body->head->hash_number;
	return (hash_id%max_id+max_id+max_id)%max_id;
}
int compute_file_hash_function(struct kernel_file_window *k_f_w)
{
	int hash_id=memory_body->file_hash_function(k_f_w);
	int max_id=memory_body->head->hash_number;
	return (hash_id%max_id+max_id+max_id)%max_id;
}
