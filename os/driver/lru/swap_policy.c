#include "../driver.h" 
static int logic_should_prepare_write(struct memory_domain_struct *p,
	int *can_write,int *should_free)
{
	int should_write,system_free_number;
	struct block_trigger *t=&(p->trigger);

	system_free_number=((memory_body->head->free_block_number)); 
	if(system_free_number<(t->system_free_block_number)){
		*should_free=TRUE;
		should_write=TRUE;
	}else if((p->block_number)>=(t->free_block_number)){
		*should_free=TRUE;
		should_write=TRUE;
	}else if(system_free_number<(t->system_write_block_number)){
		*should_free=FALSE;
		should_write=TRUE;
	}else if((p->block_number)>=(t->write_block_number)){
		*should_free=FALSE;
		should_write=TRUE;
	}else{
		*should_free=FALSE;
		should_write=FALSE;
	}
	*can_write=FALSE;
	if((p->block_number)<(p->max_block_number))
		if((system_free_number>=8)&&((p->block_ring)>=0))
			*can_write=TRUE;
	return should_write;
}
static int physical_should_prepare_write(struct memory_domain_struct *p,
	int *can_write,int *should_free)
{
	int should_write,system_free_number;
	struct block_trigger *t=&(p->physical_trigger);

	system_free_number=((memory_body->head->free_physical_block));

	if(system_free_number<(t->system_free_block_number)){
		*should_free=TRUE;
		should_write=TRUE;
	}else if((p->physical_block_number)>=(t->free_block_number)){
		*should_free=TRUE;
		should_write=TRUE;
	}else if(system_free_number<(t->system_write_block_number)){
		*should_free=FALSE;
		should_write=TRUE;
	}else if((p->physical_block_number)>=(t->write_block_number)){
		*should_free=FALSE;
		should_write=TRUE;
	}else{
		*should_free=FALSE;
		should_write=FALSE;
	}
	*can_write=FALSE;
	if((p->physical_block_number)<(p->physical_max_block_number))
		if((system_free_number>=8)&&((p->block_ring)>=0))
			*can_write=TRUE; 
	return should_write;
}
int should_prepare_write(struct memory_domain_struct *p,
	int *can_write,int *should_free)
{
	int logic_should_write,logic_should_free,logic_can_write;
	int physical_should_write,physical_should_free;
	int physical_can_write;

	logic_should_write=logic_should_prepare_write(p,
		&logic_can_write,&logic_should_free);
	physical_should_write=physical_should_prepare_write(p,
		&physical_can_write,&physical_should_free);

	*can_write=logic_can_write&&physical_can_write;
	*should_free=logic_should_free||physical_should_free;
	return (logic_should_write||physical_should_write);
}
