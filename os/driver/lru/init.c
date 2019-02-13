#include"../driver.h"
static void init_memory_body_subroutine(
	struct memory_body_struct *m_b,int first_block)
{
	int i;
	struct memory_domain_struct *p;

	m_b->head->free_block_number=m_b->head->block_number;

	m_b->wait_block->processor=(-1);
	m_b->wait_block->semaphore=(-1);

	m_b->head->free_physical_block=m_b->head->physical_block_number;
	for(i=0;i<m_b->head->physical_block_number;i++)
		(m_b->physical_block+i)->physical_id=i+first_block;
	for(i=0;i<m_b->head->block_number;i++){
		(m_b->memory_block+i)->block_state=FREE_BLOCK;
		(m_b->memory_block+i)->buf_back=i+1;
		(m_b->memory_block+i)->physical_id=(-1);
		(m_b->memory_block+i)->sleep_semaphore.processor=-1;
		(m_b->memory_block+i)->sleep_semaphore.semaphore=-1;
	} 
	(m_b->memory_block+(m_b->head->block_number-1))->buf_back=(-1);
	m_b->head->free_block=0;

	for(i=0;i<m_b->head->file_number;i++){
		(m_b->file_window+i)->state=FILE_CLOSE;
		(m_b->file_window+i)->file_front=i-1;
		(m_b->file_window+i)->file_back =i+1;
	}
	(m_b->file_window+(m_b->head->file_number-1))->file_back=0;
	m_b->file_window->file_front=m_b->head->file_number-1;
	m_b->head->free_file=0;

	for(i=0;i<m_b->head->hash_number;i++){
		(m_b->hash+i)->block_hash_ring=(-1);
		(m_b->hash+i)->file_hash_ring=(-1);
	} 
	for(i=0;i<m_b->head->memory_domain_number;i++){
		p=m_b->memory_domain+i;
		RESET_CAPABILITY(p->capability);

		p->flag=MEMORY_PROCESS_DEFAULT_FLAG;

		p->file_number=0;
		p->max_file_number=0;
		p->file_ring=-1;

		p->block_number=0;
		p->read_block_number=0;
		p->write_block_number=0;
		p->max_block_number=0;
		p->trigger.write_block_number=0;
		p->trigger.free_block_number=0;
		p->trigger.system_write_block_number=0;
		p->trigger.system_free_block_number=0;

		p->physical_block_number=0;
		p->physical_max_block_number=0;
		p->physical_trigger.write_block_number=0;
		p->physical_trigger.free_block_number=0;
		p->physical_trigger.system_write_block_number=0;
		p->physical_trigger.system_free_block_number=0;

		p->block_ring=-1;
		p->read_block_ring=-1;
		p->write_block_ring=-1;

	}
	return;
}
int init_memory_body_routine(struct memory_body_struct *m_b,int flag,
	struct install_memory_body_parameter*par,char*base,int capacity)
{
	char *pointer=base;
	unsigned int x,y;

	m_b->head=(struct memory_body_head_struct *)pointer;
	pointer+=sizeof(*(m_b->head));
	
	if(flag==2){
		par->block_number=m_b->head->block_number;
		par->file_number=m_b->head->file_number;
		par->memory_domain_number=m_b->head->memory_domain_number;
		par->hash_number=m_b->head->hash_number;
	} 
	m_b->memory_block=(struct memory_block *)pointer;
	pointer+=sizeof(struct memory_block)*(par->block_number);

	m_b->file_window=(struct file_window *)pointer;
	pointer+=sizeof(struct file_window)*(par->file_number);

	m_b->memory_domain=(struct memory_domain_struct *)pointer;
	pointer+=sizeof(struct memory_domain_struct)*(par->memory_domain_number);

	m_b->hash=(struct memory_hash *)pointer;
	pointer+=sizeof(struct memory_hash)*(par->hash_number);

	m_b->wait_block=(struct memory_sleep_semaphore *)pointer;
	pointer+=sizeof(struct memory_sleep_semaphore);

	m_b->block_hash_function=par->block_hash_function;
	m_b->file_hash_function=par->file_hash_function;
	m_b->my_processor=par->my_processor;
	m_b->my_memory_body=par->my_memory_body;

	m_b->physical_block=(struct physical_block *)pointer;
	
	if(flag==1){
		m_b->head->block_number		=par->block_number;
		m_b->head->file_number		=par->file_number;
		m_b->head->memory_domain_number	=par->memory_domain_number;
		m_b->head->hash_number		=par->hash_number;

		x=PAGE_SIZE*(unsigned int)capacity;
		y=(unsigned int)(pointer-base); 
		x=x-y; 
		y=PAGE_SIZE+sizeof(struct physical_block);
		x=x/y;
		m_b->head->physical_block_number=x-2;

		pointer+=(sizeof(struct physical_block)
			*(m_b->head->physical_block_number));

		init_memory_body_subroutine(
			m_b,1+(pointer-base)/PAGE_SIZE);
	}
	return (m_b->head->physical_block_number);
}
