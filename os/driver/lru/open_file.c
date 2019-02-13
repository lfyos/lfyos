#include "../driver.h" 
#define WRONG_MEMORY_PROCESS_ID		(-1)
#define WRONG_CAPABILITY		(-2)
#define OPEN_TOO_MANY_FILE		(-3) 
static void process_trigger_close(
	struct memory_domain_struct *p,int sleep_semaphore)
{
	int file_id;
	struct file_window *f;

	for(;((p->file_number)>=(p->trigger_close_file_number));){
		if((file_id=p->file_ring)<0)
			break;
		f=(memory_body->file_window)+file_id;
		mark_file_accessed(file_id);
		close_file_window(file_id,TRUE,FALSE,
			&(f->kernel_file_window.file.file_capability),
			sleep_semaphore,(int *)0);
	}
	return;
}
int open_file_window(struct kernel_file_window *kernel_file_window,
	int flag,int memory_domain_id,CAPABILITY *domain_capability,
	int sleep_semaphore)
{
	int file_id;
	struct memory_domain_struct *p;
	struct file_window *f;

	if(memory_domain_id<0)
		return WRONG_MEMORY_PROCESS_ID;
	if(memory_domain_id>=((memory_body->head->memory_domain_number)))
		return WRONG_MEMORY_PROCESS_ID;
	p=(memory_body->memory_domain)+memory_domain_id; 
	if(!KERNEL_COMPARE_CAPABILITY(
		(p->capability),(*domain_capability)))
			return WRONG_CAPABILITY; 
	if(((p->flag)&MEMORY_PROCESS_FILE_FLUSH_FLAG)!=0)
		process_trigger_close(p,sleep_semaphore);
	if((file_id=search_open_file(
		kernel_file_window,flag,memory_domain_id))<0)
			return file_id;
	if((f=(memory_body->file_window)+file_id)->state==FILE_OPEN)
		return file_id;
	if((p->file_number)>=p->max_file_number)
		return OPEN_TOO_MANY_FILE; 

	COPY_FILE_WINDOW((*kernel_file_window),(f->kernel_file_window));
	f->kernel_file_window.file_window_id=file_id;

	f->memory_domain_id=memory_domain_id; 
	f->flag	=flag; 
	f->block_ring=-1;

	f->lock_number=0;
	f->block_number=0; 

	mount_file(file_id,p);
	f->state=FILE_OPEN;

	mount_page_table(memory_body->my_memory_body,
		file_id,f->kernel_file_window.window_length,
		&(f->kernel_file_window.file.file_capability));
	p->file_number++;

	return file_id;
}
