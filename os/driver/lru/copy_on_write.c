#include"../driver.h" 
int copy_on_write(int bn,int root_id)
{
	int front,back,ret_val;
	struct memory_block *b,*bb,*b_f,*b_b;
	struct file_window *f;
	struct memory_domain_struct *p;
	char *s,*d;

	b=bn+(memory_body->memory_block);
	f=(b->file_window_id)+(memory_body->file_window);
	p=(memory_body->memory_domain)+(f->memory_domain_id);

	if((ret_val=mount_physical_block(b,p))<0)
		return ret_val;
	for(;;root_id=ret_val){
		bb=root_id+(memory_body->memory_block);
		if((ret_val=bb->parent_block_id)<0)
			break;
	}
	s=map_memory_block(memory_body->my_memory_body,bb->physical_id);
	d=map_memory_block(memory_body->my_memory_body,b->physical_id);
	copy_data(s,d,PAGE_SIZE);
	mark_memory_block_unmap(
		memory_body->my_memory_body,b->physical_id);
	mark_memory_block_unmap(
		memory_body->my_memory_body,bb->physical_id);

	root_id=b->parent_block_id;
	bb=root_id+(memory_body->memory_block); 
	front=b->left_brother_block_id;
	back=b->right_brother_block_id;
	if(bn==front)
		bb->first_child_block_id=(-1);
	else{
		b_f=front+(memory_body->memory_block);
		b_b=back+(memory_body->memory_block);
		b_f->right_brother_block_id=back;
		b_b->left_brother_block_id=front;
		if(bb->first_child_block_id==bn)
			bb->first_child_block_id=back; 
	} 
	b->left_brother_block_id=bn;
	b->right_brother_block_id=bn; 
	b->parent_block_id=(-1);
	b->block_state=MODIFIED_BLOCK; 
	return bn;
}
