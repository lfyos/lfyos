#include"kernel.h"
#define WRONG_OS_DOMAIN_NUMBER		-1
#define WRONG_CAPABILITY		-2
#define CREATE_TOO_MANY_SEMAPHORE	-3
#define NO_SEMAPHORE			-4
#define WRONG_SEMAPHORE 		-5
#define IS_ENTITY_SEMAPHORE		-6 
int allocate_semaphore(int domain_id,int value,
	CAPABILITY *domain_capability,CAPABILITY *semaphore_capability)
{
	int sem_id;
	struct processor_domain_struct *p;
	struct semaphore_struct *s;
	struct kernel_time my_time;

	if((domain_id<=0)||(domain_id>=OS_DOMAIN_NUMBER))
		return WRONG_OS_DOMAIN_NUMBER;
	p=&(os->os_domain[domain_id]);
	if(!KERNEL_COMPARE_CAPABILITY(
		p->capability,(*domain_capability)))
			return WRONG_CAPABILITY;
	if(p->semaphore_number>=p->max_semaphore_number)
		return CREATE_TOO_MANY_SEMAPHORE;
	if((sem_id=os->os_domain[0].semaphore_ring)<0)
		return NO_SEMAPHORE;
	s=&(os->os_semaphore[sem_id]);
	s->value=value;
	s->thread_ring=(-1);
	s->entity_type_id=(-1);
	s->entity_id=(-1); 

	COPY_CAPABILITY((*semaphore_capability),(s->capability));
	move_semaphore(sem_id,domain_id);

	SET_MOST_TIME(my_time);
	SET_MINIMAL_STEP(s->step);
	s->v_value=(-1);
	set_semaphore_time(sem_id,&my_time,-1);

	return sem_id;
}
int release_semaphore(int sem_id,CAPABILITY *semaphore_capability)
{
	int value=(-1),return_value;
	struct semaphore_struct *s;
	struct kernel_time my_time;

	if((sem_id<=0)||(sem_id>=OS_SEMAPHORE_NUMBER))
		return WRONG_SEMAPHORE;
	s=&(os->os_semaphore[sem_id]);
	if((s->entity_type_id>=0)||(s->entity_id>=0))
		return IS_ENTITY_SEMAPHORE;
	if((s->owner_domain_id<=0)
		||(s->owner_domain_id>=OS_DOMAIN_NUMBER))
			return WRONG_OS_DOMAIN_NUMBER;
	return_value=system_call_v(
		sem_id,&value,semaphore_capability);
	if(return_value<0)
		return return_value;
	move_semaphore(sem_id,0);
	SET_MOST_TIME(my_time);
	SET_MINIMAL_STEP(s->step);
	s->v_value=0;
	set_semaphore_time(sem_id,&my_time,-1);

	return sem_id;
}
int reset_semaphore_value(int sem_id,CAPABILITY *cap)
{
	int value;
	struct semaphore_struct *s;
	struct kernel_time new_time,new_step;

	if((sem_id<=0)||(sem_id>=OS_SEMAPHORE_NUMBER))
		return WRONG_SEMAPHORE;
	s=&(os->os_semaphore[sem_id]);
	if(!KERNEL_COMPARE_CAPABILITY(s->capability,(*cap)))
		return WRONG_CAPABILITY;
	if(s->value<0){
		value=(-1);
		system_call_v(sem_id,&value,&(os->system_capability));
	}
	if(s->value>0)
		s->value=0;
	SET_MOST_TIME(new_time);
	SET_MINIMAL_STEP(new_step);
	set_semaphore_time_system_call(sem_id,0,-1,
		&new_time,&new_step,&(s->capability));

	return sem_id;
}
