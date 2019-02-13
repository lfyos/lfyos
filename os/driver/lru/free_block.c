#include "../driver.h" 
static void free_one_block_routine(int bn)
{
	int hash_id,front,back;
	struct memory_block *b,*b_f,*b_b,*b_p;
	struct file_window *f;
	struct memory_domain_struct *p;
	struct memory_hash *h;
	
	b=(memory_body->memory_block)+bn;
	f=(memory_body->file_window)+(b->file_window_id);
	f->block_number--; 
	p=(memory_body->memory_domain)+(f->memory_domain_id);

	hash_id=compute_block_hash_function(
		b->file_window_id,b->logic_block_id);
	h=&(memory_body->hash[hash_id]);

	UN_MOUNT_BLOCK(bn,b,buf_front, buf_back, p->block_ring);
	UN_MOUNT_BLOCK(bn,b,file_front,file_back,f->block_ring);
	UN_MOUNT_BLOCK(bn,b,hash_front,hash_back,h->block_hash_ring);

	if(b->parent_block_id>=0){
		b_p=(memory_body->memory_block)+(b->parent_block_id);
		front=b->left_brother_block_id;
		if(bn==front)
			b_p->first_child_block_id=(-1);
		else{
			b_f=(memory_body->memory_block)+front;
			back=b->right_brother_block_id;
			b_b=(memory_body->memory_block)+back;
			b_f->right_brother_block_id=back;
			b_b->left_brother_block_id=front;

			if(b_p->first_child_block_id==bn)
				b_p->first_child_block_id=front; 
		}
	}
	free_free_block(f->memory_domain_id,bn); 
	return;
}
void free_one_block(int bn)
{
	int i,first_block_id;
	struct memory_block *b;

	b=(memory_body->memory_block)+bn;
	b->next_block_id=-1;
	for(;bn>=0;){
		b=(memory_body->memory_block)+bn;
		if((first_block_id=b->first_child_block_id)<0){
			i=bn;
			bn=b->next_block_id;
			b->next_block_id=(-1);
			free_one_block_routine(i);
		}else{
			for(i=first_block_id;;){
				b=(memory_body->memory_block)+i;
				b->next_block_id=bn;
				bn=i;
				i=b->right_brother_block_id;
				if(i==first_block_id)
					break; 
			}
		}
	}
	return;
}
