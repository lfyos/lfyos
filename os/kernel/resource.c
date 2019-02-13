#include "kernel.h"
#define WRONG_CAPABILITY 	-1
#define WRONG_OS_DOMAIN_NUMBER 	-2
int query_resource(struct processor_domain_struct *process,int domain_id)
{
	struct processor_domain_struct *p;

	if((domain_id<=0)||(domain_id>=OS_DOMAIN_NUMBER))
		return WRONG_OS_DOMAIN_NUMBER;
	p=&(os->os_domain[domain_id]);
	if(!KERNEL_COMPARE_CAPABILITY(
		(p->capability),(process->capability)))
			return WRONG_CAPABILITY;
	copy_data(p,process,sizeof(struct processor_domain_struct));
	return 0;
}
int allocate_resource(struct processor_domain_struct *process,
	int domain_id,CAPABILITY *system_capability)
{
	int i;
	struct semaphore_struct *s;
	struct processor_domain_struct *p;

	if((domain_id<=0)||(domain_id>=OS_DOMAIN_NUMBER))
		return WRONG_OS_DOMAIN_NUMBER;
	p=&(os->os_domain[domain_id]);
	if(!KERNEL_COMPARE_CAPABILITY(
		(os->system_capability),(*system_capability)))
	{
		if(!KERNEL_COMPARE_CAPABILITY(
			(p->capability),(*system_capability)))
				return WRONG_CAPABILITY;
		MODIFY_TO_USER_RUN_POINT(process->start_point);
		if((process->priority_policy)<(p->priority_policy))
			process->priority_policy=p->priority_policy;
		if((process->max_thread_number)>(p->max_thread_number))
			process->max_thread_number
				=p->max_thread_number;
		if((process->max_semaphore_number)
				>(p->max_semaphore_number))
			process->max_semaphore_number
				=p->max_semaphore_number;
		if((process->max_return_stack_number)
				>(p->max_return_stack_number))
			process->max_return_stack_number
				=p->max_return_stack_number;
	}
	COPY_RUN_POINT((process->start_point),(p->start_point));

	p->priority_policy=process->priority_policy;
	p->cpu_set_id=process->cpu_set_id;

	p->domain_id=process->domain_id;
	p->max_thread_number=process->max_thread_number;
	p->max_semaphore_number=process->max_semaphore_number;
	p->max_return_stack_number=process->max_return_stack_number;
	COPY_CAPABILITY((process->capability),(p->capability));
	COPY_CAPABILITY((process->thread_create_capability),
		(p->thread_create_capability));

	for(i=0;i<OS_USER_FILE_NUMBER;i++){
		COPY_USER_FILE_INFORMATION(process->file[i],p->file[i]);
	}
	if(p->semaphore>0){
		s=&(os->os_semaphore[p->semaphore]);
		COPY_CAPABILITY((process->capability),(s->capability));
	}
	return query_resource(process,domain_id);
}
