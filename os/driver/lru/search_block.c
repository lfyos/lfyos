#include"../driver.h" 
#define NOT_FIND_PHYSICAL_BLOCK		(-1) 
int search_block(int file_window_id,int logic_block_id)
{
	int i,first_block,hash_id;
	struct memory_block *pb;

	hash_id=compute_block_hash_function(
		file_window_id,logic_block_id);
	first_block=(memory_body->hash[hash_id].block_hash_ring);
	if(first_block>=0){
		for(i=first_block;;){
			pb=i+(memory_body->memory_block);
			if(pb->file_window_id==file_window_id)
				if(pb->logic_block_id==logic_block_id)
					return i;
			if((i=pb->hash_back)==first_block)
				break;
		}
	}
	return NOT_FIND_PHYSICAL_BLOCK;
} 
