#include"../driver.h" 
#define WRONG_MEMORY_DOMAIN_NUMBER	(-1)
#define WRONG_CAPABILITY		(-2) 
int flush_memory_domain(int max_flush_number,int giveup_flag,
	int free_block_flag,int wait_flag,int domain_id,
	int sleep_semaphore,CAPABILITY *domain_capability,
	int *flush_number)
{
	int i,bn,return_value,max_number;
	struct memory_domain_struct *p;
	
	if(((int)flush_number)!=0)
		for(i=0;i<MAX_FLUSH_RESULT_NUMBER;i++)
			(*(flush_number+i))=0;
	max_number=memory_body->head->memory_domain_number;
	if((domain_id<0)||(domain_id>=max_number))
		return WRONG_MEMORY_DOMAIN_NUMBER;
	p=domain_id+(memory_body->memory_domain);
	if(!KERNEL_COMPARE_CAPABILITY(p->capability,*domain_capability))
		return WRONG_CAPABILITY;
	if((max_flush_number<0)||(max_flush_number>p->block_number))
		max_flush_number=p->block_number;
	for(i=0,return_value=0,max_number=1;i<max_number;i++){
		if(max_flush_number>=0)
			max_number=max_flush_number;
		else if(max_number<(p->block_number))
			max_number=p->block_number;
		if((return_value=prepare_free_block(
			domain_id,sleep_semaphore))<0)
				break;
		if((bn=p->block_ring)<0)
			break;
		mark_block_accessed(bn);
		if((return_value=flush_block(bn,sleep_semaphore,
			giveup_flag,free_block_flag,wait_flag,-1))<0)
				break;
		return_value=return_value%MAX_FLUSH_RESULT_NUMBER;
		if(((int)flush_number)!=0)
			(*(return_value+flush_number))++;
	}
	if((((int)flush_number)!=0)&&(return_value>=0))
		for(i=0,return_value=0;i<MAX_FLUSH_RESULT_NUMBER;i++)
			return_value=return_value+(*(flush_number+i)); 
	return return_value;
}
