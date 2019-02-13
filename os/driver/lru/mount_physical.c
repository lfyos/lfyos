#include "../driver.h" 
#define NO_FREE_PHYSICAL_BLOCK	(-1)
int request_physical_id()
{
	int pbn; 
	if((pbn=memory_body->head->free_physical_block)<=0)
		return NO_FREE_PHYSICAL_BLOCK;
	pbn=(memory_body->physical_block+pbn-1)->physical_id;
	memory_body->head->free_physical_block--;
	return pbn;
}
void release_physical_id(int physical_id)
{
	int pbn;
	struct physical_block *pb; 
	pbn=memory_body->head->free_physical_block;
	pb=pbn+(memory_body->physical_block);
	pb->physical_id=physical_id;
	memory_body->head->free_physical_block++; 
	return;
}
int mount_physical_block(
	struct memory_block *b,struct memory_domain_struct *p)
{
	int pbn;

	if((pbn=b->physical_id)>=0)
		return pbn;
	if((pbn=request_physical_id())<=0)
		return pbn; 
	b->physical_id=pbn; 
	p->physical_block_number++; 
	return pbn;
}
void dismount_physical_block(
	struct memory_block *b,struct memory_domain_struct *p)
{
	if(b->physical_id>=0){
		release_physical_id(b->physical_id);
		b->physical_id=(-1); 
		p->physical_block_number--; 
	}
	return;
}
