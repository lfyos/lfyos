#include"../driver.h" 
void mark_file_accessed(int file_id)
{
	int front,back;
	struct memory_domain_struct *p;
	struct file_window *f,*f_f,*f_b;

	if(((f=(memory_body->file_window)+file_id)->state)!=FILE_OPEN)
		return;
	p=(memory_body->memory_domain)+(f->memory_domain_id); 
	if(file_id==(front=f->file_front))
		p->file_ring=(-1);
	else{
		f_f=(memory_body->file_window)+front;
		back=f->file_back;
		f_b=(memory_body->file_window)+back;
		f_f->file_back=back;
		f_b->file_front=front;
		if((p->file_ring)==file_id)
			p->file_ring=front;
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
	return;
}
void mark_block_accessed(int bn)
{
	int front,back;
	struct memory_block *b,*b_f,*b_b;
	struct file_window *f;
	struct memory_domain_struct *p;
		
	for(;;){
		b=bn+(memory_body->memory_block);
		switch(b->block_state){
		case MODIFIED_BLOCK:
		case NOT_MODIFIED_BLOCK:
			f=memory_body->file_window+(b->file_window_id);
			p=memory_body->memory_domain;
			p+=f->memory_domain_id;

			UN_MOUNT_BLOCK(bn,b,
				buf_front,buf_back,p->block_ring);
			   MOUNT_BLOCK(bn,b,
				buf_front,buf_back,p->block_ring);
			UN_MOUNT_BLOCK(bn,b,
				file_front,file_back,f->block_ring);
			   MOUNT_BLOCK(bn,b,
				file_front,file_back,f->block_ring);
			break;
		case READING_BLOCK:
		case WRITING_BLOCK:
			f=memory_body->file_window+(b->file_window_id);
			UN_MOUNT_BLOCK(bn,b,
				file_front,file_back,f->block_ring);
			   MOUNT_BLOCK(bn,b,
				file_front,file_back,f->block_ring);
			break;
		default:
			return ;
		}
		if((bn=b->parent_block_id)<0)
			break;
	}
	return;
}
