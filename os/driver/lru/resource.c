#include"../driver.h"
#define WRONG_MEMORY_DOMAIN_NUMBER	-2
#define WRONG_CAPABILITY		-4 
int query_memory_resource(
	int memory_domain_id,struct memory_domain_struct *r)
{
	struct memory_domain_struct *p;
	if(memory_domain_id<0)
		return WRONG_MEMORY_DOMAIN_NUMBER;
	if(memory_domain_id>=(memory_body->head->memory_domain_number))
		return WRONG_MEMORY_DOMAIN_NUMBER;
	p=(memory_body->memory_domain)+memory_domain_id;
	if(!KERNEL_COMPARE_CAPABILITY((p->capability),(r->capability)))
		return WRONG_CAPABILITY;
	copy_data(p,r,sizeof(struct memory_domain_struct));
	RESET_CAPABILITY(r->capability);
	return 0;
}
int allocate_memory_resource(
	int memory_domain_id,struct memory_domain_struct*r)
{
	struct memory_domain_struct *p;

	if(memory_domain_id<0)
		return WRONG_MEMORY_DOMAIN_NUMBER;
	if(memory_domain_id>=(memory_body->head->memory_domain_number))
		return WRONG_MEMORY_DOMAIN_NUMBER;
	p=(memory_body->memory_domain)+memory_domain_id;
	
	if(r->max_file_number<1)
		r->max_file_number=1;
	if(r->trigger_close_file_number<1)
		r->trigger_close_file_number=1;
	if(r->max_block_number<1)
		r->max_block_number=1;
	if(r->physical_max_block_number<1)
		r->physical_max_block_number=1;

	if((r->trigger.free_block_number)>=(r->max_block_number))
		r->trigger.free_block_number=r->max_block_number-1;
	if((r->trigger.free_block_number)<0)
		r->trigger.free_block_number=0; 
	if((r->physical_trigger.free_block_number)
		>=(r->physical_max_block_number))
			r->physical_trigger.free_block_number
				=r->physical_max_block_number-1;
	if((r->physical_trigger.free_block_number)<0)
		r->physical_trigger.free_block_number=0;

	if((r->trigger.write_block_number)
		>=(r->trigger.free_block_number))
			r->trigger.write_block_number
				=r->trigger.free_block_number;
	if((r->trigger.write_block_number)<0)
		r->trigger.write_block_number=0; 
	if((r->physical_trigger.write_block_number)
		>=(r->physical_trigger.free_block_number))
			r->physical_trigger.write_block_number
				=r->physical_trigger.free_block_number;
	if((r->physical_trigger.write_block_number)<0)
		r->physical_trigger.write_block_number=0;

	if((r->trigger.system_free_block_number)
			>=(((memory_body->head->block_number))/2))
		r->trigger.system_free_block_number
			=(((memory_body->head->block_number))/2);
	if((r->trigger.system_free_block_number)<0)
		r->trigger.system_free_block_number=0; 
	if((r->physical_trigger.system_free_block_number)
		>=(((memory_body->head->physical_block_number))/2))
		r->physical_trigger.system_free_block_number
		=(((memory_body->head->physical_block_number))/2);
	if((r->physical_trigger.system_free_block_number)<0)
		r->physical_trigger.system_free_block_number=0;

	if((r->trigger.system_write_block_number)
			>=(((memory_body->head->block_number))/2))
		r->trigger.system_write_block_number
			=(((memory_body->head->block_number))/2);
	if((r->trigger.system_write_block_number)<0)
		r->trigger.system_write_block_number=0; 
	if((r->physical_trigger.system_write_block_number)
		>=(((memory_body->head->physical_block_number))/2))
		r->physical_trigger.system_write_block_number
		=(((memory_body->head->physical_block_number))/2);
	if((r->physical_trigger.system_write_block_number)<0)
		r->physical_trigger.system_write_block_number=0;

	if(KERNEL_COMPARE_CAPABILITY(
		(os->system_capability),(r->capability)))
	{
		p->flag=r->flag;
		p->max_file_number=r->max_file_number;
		p->trigger_close_file_number
			=r->trigger_close_file_number; 
		p->max_block_number=r->max_block_number;
		p->physical_max_block_number
			=r->physical_max_block_number;
		p->trigger.free_block_number
			=r->trigger.free_block_number;
		p->trigger.write_block_number
			=r->trigger.write_block_number;
		p->trigger.system_free_block_number
			=r->trigger.system_free_block_number; 
		p->trigger.system_write_block_number
			=r->trigger.system_write_block_number; 
		p->physical_trigger.free_block_number
			=r->physical_trigger.free_block_number;
		p->physical_trigger.write_block_number
			=r->physical_trigger.write_block_number;
		p->physical_trigger.system_free_block_number
			=r->physical_trigger.system_free_block_number;
		p->physical_trigger.system_write_block_number
			=r->physical_trigger.system_write_block_number;
		return 0;
	}
	if(KERNEL_COMPARE_CAPABILITY((p->capability),(r->capability))){
		p->flag=r->flag;
		if((p->max_file_number)>=(r->max_file_number))
			p->max_file_number=r->max_file_number; 
		if((p->max_block_number)>=(r->max_block_number))
			p->max_block_number=r->max_block_number;
		if((p->physical_max_block_number)
			>=(r->physical_max_block_number))
				p->physical_max_block_number
					=r->physical_max_block_number;
		if((p->trigger.write_block_number)
			>=(r->trigger.write_block_number))
				p->trigger.write_block_number
					=r->trigger.write_block_number;
		if((p->trigger.free_block_number)
			>=(r->trigger.free_block_number))
				p->trigger.free_block_number
					=r->trigger.free_block_number;
		if((p->trigger.system_free_block_number)
				<=(r->trigger.system_free_block_number))
			p->trigger.system_free_block_number
				=r->trigger.system_free_block_number; 
		if((p->trigger.system_write_block_number)
			<=(r->trigger.system_write_block_number))
			p->trigger.system_write_block_number
				=r->trigger.system_write_block_number; 
		if((p->physical_trigger.write_block_number)
			>=(r->physical_trigger.write_block_number))
				p->physical_trigger.write_block_number
				=r->physical_trigger.write_block_number;
		if((p->physical_trigger.free_block_number)
			>=(r->physical_trigger.free_block_number))
				p->physical_trigger.free_block_number
				=r->physical_trigger.free_block_number;
		if((p->physical_trigger.system_free_block_number)
		<=(r->physical_trigger.system_free_block_number))
			p->physical_trigger.system_free_block_number
			=r->physical_trigger.system_free_block_number; 
		if((p->physical_trigger.system_write_block_number)
		<=(r->physical_trigger.system_write_block_number))
			p->physical_trigger.system_write_block_number
			=r->physical_trigger.system_write_block_number; 
		return 1;
	}
	return WRONG_CAPABILITY;
}
