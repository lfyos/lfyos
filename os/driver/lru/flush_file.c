#include"../driver.h"
#define WRONG_FILE_NUMBER	(-1)
#define WRONG_CAPABILITY	(-2)
#define WRONG_UPDATE_CAPABILITY	(-3)
#define WRONG_FILE_STATE	(-4)
#define CAN_NOT_UPDATE		(-5) 
#define NO_WRITE_RIGHT		(-6)
static int flush_file_window_test(
	int file_window_id,int update_file_window_id,
	CAPABILITY *file_capability,CAPABILITY *update_capability)
{
	CAPABILITY  *c; 
	struct file_window *f,*fp;
		
	if(file_window_id<0)
		return WRONG_FILE_NUMBER;	
	if(file_window_id>=(memory_body->head->file_number))
		return WRONG_FILE_NUMBER;	
	f=file_window_id+(memory_body->file_window);
	c=&(f->kernel_file_window.file.file_capability);
	if(!KERNEL_COMPARE_CAPABILITY(*c,*file_capability))
		return WRONG_CAPABILITY;
	if(f->state!=FILE_OPEN)
		return WRONG_FILE_STATE;
	if(update_file_window_id>=(memory_body->head->file_number))
		return CAN_NOT_UPDATE;
	if(update_file_window_id>=0){
		fp=update_file_window_id+(memory_body->file_window);
		if(fp->state!=FILE_OPEN)
			return CAN_NOT_UPDATE;
		c=&(fp->kernel_file_window.file.file_capability);
		if(!KERNEL_COMPARE_CAPABILITY(*c,*update_capability))
			return WRONG_UPDATE_CAPABILITY;
		if(!(IS_READ_WRITE_RIGHT(fp->kernel_file_window.right)))
			return NO_WRITE_RIGHT;
	}
	mark_file_accessed(file_window_id);
	if(update_file_window_id>=0)
		mark_file_accessed(update_file_window_id);
	return 0;
}
int flush_file_window(int file_window_id,int update_file_window_id,
	int max_flush_number, int giveup_flag, int free_block_flag,
	int wait_flag,int sleep_semaphore,CAPABILITY *file_capability,
	CAPABILITY *update_file_capability,int *flush_number)
{
	int i,bn,parent_bn,return_value,max_number,logic_block_id;
	struct memory_block *b;
	struct file_window *f;
		
	if((return_value=flush_file_window_test(file_window_id,
		update_file_window_id,file_capability,
		update_file_capability))<0)
			return return_value;
	if(((int)flush_number)!=0)
		for(i=0;i<MAX_FLUSH_RESULT_NUMBER;i++)
			(*(flush_number+i))=0; 
	f=file_window_id+(memory_body->file_window);
	f->lock_number++; 
	for(i=0,return_value=0,max_number=1;i<max_number;i++){
		if(max_flush_number>=0)
			max_number=max_flush_number;
		else if(max_number<(f->block_number))
			max_number=f->block_number;
		if((bn=f->block_ring)<0)
			break;
		mark_block_accessed(bn); 
		b=bn+(memory_body->memory_block);
		logic_block_id=b->logic_block_id; 
		if((update_file_window_id<0)||(b->parent_block_id>=0))
			return_value=flush_block(bn,sleep_semaphore,
			      giveup_flag,free_block_flag,wait_flag,-1);
		else{
			if((return_value=search_read_block(
			update_file_window_id,logic_block_id*PAGE_SIZE,
			sleep_semaphore,FALSE,&parent_bn))<0)
				break;
			return_value=4;
			if((b->logic_block_id)==logic_block_id)
			if((b->file_window_id)==file_window_id)
			if(b->parent_block_id<0)
				return_value=flush_block(bn,
					sleep_semaphore,giveup_flag,
					free_block_flag,wait_flag,
					parent_bn);
		}
		if(return_value<0)
			break;
		return_value=return_value%MAX_FLUSH_RESULT_NUMBER;
		if(((int)flush_number)!=0)
			(*(return_value+flush_number))++;
	}
	f->lock_number--;
	if((((int)flush_number)!=0)&&(return_value>=0))
		for(i=0,return_value=0;i<MAX_FLUSH_RESULT_NUMBER;i++)
			return_value=return_value+(*(flush_number+i)); 
	return return_value;
}
