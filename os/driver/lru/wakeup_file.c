#include"../driver.h"
#define WRONG_FILE_NUMBER	-1
#define WRONG_FILE_STATE	-2
#define WRONG_CAPABILITY	-3
int wakeup_file_window(int file_window_id,
	int no_wait_flag,int max_number,CAPABILITY *file_capability)
{
	int i,bn,max_set_number;
	struct file_window *f;
	CAPABILITY  *c;

	if(file_window_id<0)
		return WRONG_FILE_NUMBER;
	if(file_window_id>=(memory_body->head->file_number))
		return WRONG_FILE_NUMBER;
	f=(memory_body->file_window)+file_window_id;
	if(f->state!=FILE_OPEN)
		return WRONG_FILE_STATE;
	c=&(f->kernel_file_window.file.file_capability);
	if(!KERNEL_COMPARE_CAPABILITY(*c,*file_capability))
		return WRONG_CAPABILITY;
	mark_file_accessed(file_window_id);
	if(max_number==0)
		return 0;
	if(no_wait_flag)
		f->flag|=( FILE_WINDOW_NO_WAIT_FLAG);
	else{
		f->flag&=(~FILE_WINDOW_NO_WAIT_FLAG);
		return 0;
	}
	for(i=0,max_set_number=1;i<max_set_number;i++){
		if((bn=f->block_ring)<0)
			break;
		mark_block_accessed(bn); 
		wakeup_block(bn);
		if(max_number>0)
			max_set_number=max_number;
		else if(max_set_number<f->block_number)
			max_set_number=f->block_number;
	}
	return i;
}
