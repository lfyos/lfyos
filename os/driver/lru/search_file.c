#include "../driver.h" 
#define NO_FILE_TO_OPEN			(-1)
int search_open_file(struct kernel_file_window *f,
	int flag,int memory_domain_id)
{
	int file_id,hash_id;
	struct file_window *f_f;
	struct memory_hash *h;
	struct kernel_file_window *k_f;

	hash_id=compute_file_hash_function(f); 
	h=hash_id+(memory_body->hash);
	if((file_id=h->file_hash_ring)>=0){
		for(;;){
			f_f=file_id+(memory_body->file_window);
			k_f=&(f_f->kernel_file_window); 
			if(f_f->memory_domain_id==memory_domain_id)
			if(f_f->flag==flag)
			if(IS_SAME_KERNEL_FILE_WINDOW(*f,*k_f)){
				mark_file_accessed(file_id);
				return file_id;
			}
			file_id=f_f->hash_back;
			if(file_id==(h->file_hash_ring))
				break;
		}
	}
	if((file_id=((memory_body->head->free_file)))<0)
		file_id=NO_FILE_TO_OPEN;
	return file_id;
}
