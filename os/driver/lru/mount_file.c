#include "../driver.h" 
void mount_file(int file_id,struct memory_domain_struct *p)
{
	int hash_id,front,back;
	struct file_window *f,*f_f,*f_b;
	struct memory_hash *h;
	struct kernel_file_window *k_f;

	f=(memory_body->file_window)+file_id;
	k_f=&(f->kernel_file_window);

	front=f->file_front;	f_f=(memory_body->file_window)+front;
	back=f->file_back;	f_b=(memory_body->file_window)+back;

	if(file_id==back)
		(memory_body->head->free_file)=-1;
	else{
		if((memory_body->head->free_file)==file_id)
			(memory_body->head->free_file)=back;
		f_f->file_back=back;
		f_b->file_front=front;
	}
	if(p->file_ring<0){
		f->file_front=file_id;
		f->file_back=file_id;
		p->file_ring=file_id;
	}else{
		front=p->file_ring;
		f_f=(memory_body->file_window)+front;
		back=f_f->file_back;
		f_b=(memory_body->file_window)+back;

		f->file_front=front;
		f->file_back=back;
		f_f->file_back=file_id;
		f_b->file_front=file_id;
	}
	hash_id=compute_file_hash_function(k_f); 
	h=hash_id+(memory_body->hash);

	if(h->file_hash_ring<0){
		f->hash_front=file_id;
		f->hash_back=file_id;
		h->file_hash_ring=file_id;
	}else{
		front=h->file_hash_ring;
		f_f=(memory_body->file_window)+front;
		back=f_f->hash_back;
		f_b=(memory_body->file_window)+back;

		f->hash_front=front;
		f->hash_back=back;
		f_f->hash_back=file_id;
		f_b->hash_front=file_id;
	}
	return;
} 
