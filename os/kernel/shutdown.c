#include"kernel.h"
int shutdown_process(int *success_number,int *fail_number)
{
	int thread_id,domain_id,rt_id;
	struct thread_struct *t;
	struct return_stack *rt;

	*fail_number=0;*success_number=0;
	for(domain_id=1;domain_id<OS_DOMAIN_NUMBER;domain_id++){
		thread_id=create_thread(
			(struct user_file_information *)0,
			domain_id,domain_id,
			shutdown_create_thread_priority(domain_id), 
			os->os_domain[domain_id].cpu_set_id,
			(struct thread_environment *)0,
			&(os->system_capability));
		if(thread_id<0)
			(*fail_number)++;
		else{ 
			(*success_number)++;
			t=&(os->os_thread[thread_id]);
			rt_id=t->return_stack_pointer;
			rt=&(os->os_return_stack[rt_id]);
			COPY_RUN_POINT(os->os_domain[domain_id].
				start_point,rt->environment.point);
			rt->environment.reg.system_call
				=SHUTDOWN_REQUEST;
		}
	}
	return (*success_number)+(*fail_number);
}
