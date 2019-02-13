#include"../driver.h"
#define FIND_NO_WAIT_FILE_WINDOW	-1
int search_read_block(int file_window_id,int logic_address,
	int sleep_semaphore,int read_only_flag,int *root_block_id)
{
	int bn,bbn,ret_val,wait_queue;
	int search_file_window_id,search_logic_address;
	struct memory_block *b;
	struct file_window *f;

	for(;;){
		search_file_window_id=file_window_id;
		search_logic_address=logic_address;
		for(wait_queue=-1,ret_val=TRUE;ret_val;){
			*root_block_id=read_one_block(sleep_semaphore,
				&ret_val,&search_file_window_id,
				&search_logic_address,&wait_queue);
		}
		if((bn=mount_wait_reading_block(
			*root_block_id,wait_queue))<0)
				break;
		for(;;){
			b=(*root_block_id)+(memory_body->memory_block);
			if(b->parent_block_id<0)
				break; 
			*root_block_id=b->parent_block_id;
		} 
		if(read_only_flag){
			b=bn+(memory_body->memory_block);
			f=(b->file_window_id)+(memory_body->file_window);
			if((f->flag&FILE_WINDOW_COPY_ON_WRITE_FLAG)!=0)
				break;
		}
		ret_val=PRIVATE_SHARE_MEMORY_BLOCK_FLAG;
		for(bbn=bn;;bbn=b->parent_block_id){
			b=bbn+(memory_body->memory_block);
			if((b->block_flag&ret_val)!=0)
				break;
			if(b->parent_block_id<0)
				break;
		}
		if(bbn==(*root_block_id))
			break;
		clear_block_number(bbn);
		b=bbn+(memory_body->memory_block);
		if((b->lock_number)<=0){
			ret_val=copy_on_write(bbn,*root_block_id);
			if(ret_val<0)
				bn=ret_val;
			*root_block_id=bbn;
			break;
		}
		if(wait_at_block(bbn,sleep_semaphore)){
			bn=FIND_NO_WAIT_FILE_WINDOW;
			break;
		}
	}
	return bn;
}
