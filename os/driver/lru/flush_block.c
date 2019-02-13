#include "../driver.h"
static int write_one_block(int bn,int sleep_semaphore)
{
	int front,back,ret_val,logic_address;
	struct memory_block *b,*b_f,*b_b;
	struct file_window *f;
	struct memory_domain_struct *p;
	
	b=memory_body->memory_block+bn;
	f=(memory_body->file_window)+(b->file_window_id);
	p=(memory_body->memory_domain)+(f->memory_domain_id);
	logic_address=(b->logic_block_id)*PAGE_SIZE;

	b->block_state=WRITING_BLOCK;

	UN_MOUNT_BLOCK(bn,b,buf_front,buf_back,p->block_ring);
	MOUNT_BLOCK(bn,b,buf_front,buf_back,p->write_block_ring); 
	p->write_block_number++; 
	ret_val=read_write_file_system(
		WRITE_FILE,b,&logic_address,sleep_semaphore); 
	p->write_block_number--; 
	UN_MOUNT_BLOCK(bn,b,buf_front,buf_back,p->write_block_ring);
	MOUNT_BLOCK(bn,b,buf_front,buf_back,p->block_ring);
	
	b->block_state=(ret_val>=0)?NOT_MODIFIED_BLOCK:MODIFIED_BLOCK; 
	wakeup_block(bn);
	return ret_val;
} 
static void mount_update_block(int bn,int parent_bn)
{
	int front,back;
	struct memory_block *b,*b_f,*b_b,*bp;
	struct file_window *f;

	b=(memory_body->memory_block)+bn;
	b->block_flag=(b->block_flag)|PRIVATE_SHARE_MEMORY_BLOCK_FLAG;
	bp=(memory_body->memory_block)+parent_bn;
	f=(memory_body->file_window)+(b->file_window_id);

	dismount_physical_block(b,
		(memory_body->memory_domain)+(f->memory_domain_id));

	b->parent_block_id=parent_bn;
	if((front=bp->first_child_block_id)<0){
		bp->first_child_block_id=bn;
		b->left_brother_block_id=bn;
		b->right_brother_block_id=bn;
	}else{
		b_f=(memory_body->memory_block)+front;
		back=b_f->right_brother_block_id;
		b_b=(memory_body->memory_block)+back;

		b_f->right_brother_block_id=bn;
		b_b->left_brother_block_id=bn;

		b->left_brother_block_id=front;
		b->right_brother_block_id=back;
	}
	return;
}
int flush_block(int block_id,int sleep_semaphore,
	int giveup_flag,int free_block_flag,int wait_flag,int parent_bn)
{
	int return_value;
	struct memory_block *b,*pb;

	b=block_id+(memory_body->memory_block);
	switch(b->block_state){
	case READING_BLOCK:
	case WRITING_BLOCK:
		return_value=(b->block_state==READING_BLOCK)?0:1;
		if(wait_flag)
			wait_at_block(block_id,sleep_semaphore);
		return return_value;
	case MODIFIED_BLOCK:
	case NOT_MODIFIED_BLOCK:
		break;
	default:
		return 2;
	}
	clear_block_number(block_id);
	if((b->lock_number)>0)
		return 3;
	if(b->parent_block_id>=0){
		if(b->block_state==MODIFIED_BLOCK)
			b->block_state=NOT_MODIFIED_BLOCK;
		parent_bn=-1;
	} 
	if(parent_bn>=0){
		for(;;parent_bn=return_value){
			pb=parent_bn+(memory_body->memory_block);
			if((return_value=pb->parent_block_id)<0)
				break;
		}
		clear_block_number(parent_bn);
		if((pb->lock_number)>0)
			return 5;
		if(pb->block_state==NOT_MODIFIED_BLOCK)
			pb->block_state=MODIFIED_BLOCK;
		if(b->block_state==MODIFIED_BLOCK)
			b->block_state=NOT_MODIFIED_BLOCK;
		return_value=b->physical_id;
		b->physical_id=pb->physical_id;
		pb->physical_id=return_value;
		mount_update_block(block_id,parent_bn);
		return_value=6;
	}else if(((b->block_state)!=MODIFIED_BLOCK)||giveup_flag){
		b->block_state=NOT_MODIFIED_BLOCK;
		return_value=7;
	}else if((return_value=write_one_block(
		block_id,sleep_semaphore))<0)
			return return_value;
	else 
		return_value=8;
	if(free_block_flag)
	if((b->block_state==NOT_MODIFIED_BLOCK)&&(b->lock_number<=0)){
		return_value+=3;
		free_one_block(block_id); 
	} 
	return return_value;
}
