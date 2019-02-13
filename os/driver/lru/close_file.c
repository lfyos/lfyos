#include "../driver.h" 
#define EXIST_BLOCK			(-1)
#define FILE_IS_LOCKED			(-2) 
int close_file_window(int file_window_id,int giveup_flag,
	int wait_flag,CAPABILITY *file_capability,
	int sleep_semaphore,int *flush_number)
{
	int return_value,hash_id,front,back;
	struct memory_domain_struct *p;
	struct file_window *f,*f_f,*f_b;
	struct memory_hash *h;

	if((return_value=flush_file_window(file_window_id,-1,-1,
		giveup_flag,TRUE,wait_flag,sleep_semaphore,
		file_capability,file_capability,flush_number))<0)
			return return_value;
	f=(memory_body->file_window)+file_window_id;
	p=(memory_body->memory_domain)+(f->memory_domain_id);

	if(f->block_ring>=0)
		return EXIST_BLOCK;
	if((f->lock_number)>0)
		return FILE_IS_LOCKED;

	hash_id=compute_file_hash_function(&(f->kernel_file_window));
	h=(memory_body->hash)+hash_id;

	front=f->hash_front;
	back=f->hash_back;
	if(front==file_window_id)
		h->file_hash_ring=-1;
	else{
		f_f=(memory_body->file_window)+front;
		f_b=(memory_body->file_window)+back;
		f_f->hash_back=back;
		f_b->hash_front=front;
		if((h->file_hash_ring)==file_window_id)
			h->file_hash_ring=back;
	} 
	front=f->file_front;
	back=f->file_back;
	if(front==file_window_id)
		p->file_ring=-1;
	else{
		f_f=(memory_body->file_window)+front;
		f_b=(memory_body->file_window)+back;
		f_f->file_back=back;
		f_b->file_front=front;
		if((p->file_ring)==file_window_id)
			p->file_ring=back;
	}
	back=((memory_body->head->free_file));
	if(back<0){
		f->file_front=file_window_id; 
		f->file_back=file_window_id;
	}else{
		f_b=(memory_body->file_window+back);
		front=f_b->file_front;
		f_f=(memory_body->file_window+front);

		f_f->file_back=file_window_id;
		f_b->file_front=file_window_id;
		f->file_front=front;
		f->file_back=back;
	}
	(memory_body->head->free_file)=file_window_id;

	unmount_page_table(memory_body->my_memory_body,file_window_id);

	f->state=FILE_CLOSE;
	p->file_number--;

	return file_window_id;
}
