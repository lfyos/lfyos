#include"../driver.h" 
#define WRONG_MEMORY_DOMAIN_NUMBER	(-1)
#define WRONG_CAPABILITY		(-2) 
int wakeup_memory_domain(int domain_id,int no_wait_flag,int max_number,
	int max_file_number,CAPABILITY *domain_capability)
{
	int i,file_window_id,max_set_number,return_value;
	struct memory_domain_struct *p;
	struct file_window *f;
	
	if(domain_id<0)
		return WRONG_MEMORY_DOMAIN_NUMBER;
	if(domain_id>=(memory_body->head->memory_domain_number))
		return WRONG_MEMORY_DOMAIN_NUMBER;
	p=domain_id+(memory_body->memory_domain);
	if(!KERNEL_COMPARE_CAPABILITY(p->capability,*domain_capability))
		return WRONG_CAPABILITY;
	for(i=0,max_set_number=1;i<max_set_number;i++){
		if((file_window_id=p->file_ring)<0)
			return i;
		mark_file_accessed(file_window_id); 
		f=(memory_body->file_window)+file_window_id;
		if((return_value=wakeup_file_window(file_window_id,
			no_wait_flag,max_file_number,
			&(f->kernel_file_window.file.file_capability)))<0)
				return return_value;
		if(max_number>0)
			max_set_number=max_number;
		else if(max_set_number<p->file_number)
			max_set_number=p->file_number;
	}
	return i;
}
