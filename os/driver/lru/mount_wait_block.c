#include"../driver.h" 
int mount_wait_reading_block(int block_id,int wait_queue)
{
	int bn,front,back;
	struct memory_block *b,*bb,*b_f,*b_b;
	struct file_window *f;
	struct memory_domain_struct *p;

	if(block_id<0){
		for(;wait_queue>=0;){
			bn=wait_queue;
			b=bn+(memory_body->memory_block);
			wait_queue=b->next_block_id;
			b->next_block_id=(-1);
			unmount_reading_block(bn,TRUE);
		}
		return block_id;
	}
	if(wait_queue<0)
		return block_id;
	for(;wait_queue>=0;block_id=bn){
		bn=wait_queue;
		b=bn+(memory_body->memory_block);
		wait_queue=b->next_block_id;
		b->next_block_id=(-1);

		f=(b->file_window_id)+(memory_body->file_window);
		p=(memory_body->memory_domain)+(f->memory_domain_id); 
		dismount_physical_block(b,p);
		unmount_reading_block(bn,FALSE); 
		
		bb=block_id+(memory_body->memory_block);
		front=bb->first_child_block_id;
		if(front<0){
			bb->first_child_block_id=bn;
			b->left_brother_block_id=bn;
			b->right_brother_block_id=bn;
		}else{
			b_f=front+(memory_body->memory_block);
			back=b_f->right_brother_block_id;
			b_b=back+(memory_body->memory_block);
			
			b_f->right_brother_block_id=bn;
			b_b->left_brother_block_id=bn;

			b->right_brother_block_id=back;
			b->left_brother_block_id=front;
		} 
		b->parent_block_id=block_id;
		b->first_child_block_id=-1;
	}
	return bn;
}
