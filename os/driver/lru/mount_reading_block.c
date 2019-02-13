#include"../driver.h"
int mount_reading_block(int file_window_id,
	int *logic_address,int read_flag)
{
	int hash_id,bn,front,back,logic_block_id;
	struct memory_block *b,*b_f,*b_b;
	struct file_window *f;
	struct memory_domain_struct *p;
	struct memory_hash *h;
	
	logic_block_id=(*logic_address)/PAGE_SIZE; 
	*logic_address=logic_block_id*PAGE_SIZE;
	f=memory_body->file_window+file_window_id;
	if((bn=get_free_block(f->memory_domain_id))<0)
		return bn;
	f->block_number++; 
	b=memory_body->memory_block+bn;
	b->lock_number=0; 
	b->file_window_id=file_window_id;
	b->logic_block_id=logic_block_id;

	hash_id=compute_block_hash_function(
		file_window_id,logic_block_id);
	h=&(memory_body->hash[hash_id]);
	p=(memory_body->memory_domain)+(f->memory_domain_id);

	MOUNT_BLOCK(bn,b,file_front,file_back,f->block_ring);
	MOUNT_BLOCK(bn,b,hash_front,hash_back,h->block_hash_ring);

	if(read_flag){
		b->block_state=READING_BLOCK;
		MOUNT_BLOCK(bn,b,buf_front,buf_back,p->read_block_ring);
		p->read_block_number++; 
	}else{
		b->block_state=NOT_MODIFIED_BLOCK;
		MOUNT_BLOCK(bn,b,buf_front,buf_back,p->block_ring);
	} 
	b->sleep_semaphore.processor=-1;
	b->sleep_semaphore.semaphore=-1; 
	return bn;
}
void unmount_reading_block(int bn,int read_fail_flag)
{
	int front,back;
	struct memory_block *b,*b_f,*b_b;
	struct file_window *f;
	struct memory_domain_struct *p;

	b=(memory_body->memory_block)+bn;
	if(b->block_state!=READING_BLOCK)
		return;
	f=(memory_body->file_window)+(b->file_window_id);
	p=(memory_body->memory_domain)+(f->memory_domain_id);
	p->read_block_number--; 
	
	wakeup_block(bn); 
	UN_MOUNT_BLOCK(bn,b,buf_front,buf_back,p->read_block_ring);
	   MOUNT_BLOCK(bn,b,buf_front,buf_back,p->block_ring);
	
	b->block_state=NOT_MODIFIED_BLOCK;
	if(read_fail_flag)
		free_one_block(bn); 
	return;
}
