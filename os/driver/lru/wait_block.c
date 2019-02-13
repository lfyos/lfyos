#include "../driver.h" 
int  wait_at_block(int block_id,int sleep_semaphore)
{
	int next_proc,next_sema;
	struct memory_block *b;
	struct file_window *f;

	b=(memory_body->memory_block)+block_id;
	f=(b->file_window_id)+(memory_body->file_window);
	if((f->flag&FILE_WINDOW_NO_WAIT_FLAG)!=0)
		return TRUE; 
	switch(b->block_state){
	case READING_BLOCK:
	case WRITING_BLOCK:
		next_proc=b->sleep_semaphore.processor;
		next_sema=b->sleep_semaphore.semaphore;
		b->sleep_semaphore.processor=memory_body->my_processor;
		b->sleep_semaphore.semaphore=sleep_semaphore;
		memory_wait(sleep_semaphore,next_proc,next_sema);
		break;
	default:
		break ;
	}
	return ((f->flag&FILE_WINDOW_NO_WAIT_FLAG)!=0)?TRUE:FALSE;
} 
