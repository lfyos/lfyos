#include"kernel.h"
void move_semaphore(int semaphore_id,int destination_process)
{
	int front,back;
	struct processor_domain_struct *p;
	struct semaphore_struct *s;

	s=&(os->os_semaphore[semaphore_id]);
	p=&(os->os_domain[s->owner_domain_id]);
	p->semaphore_number--;
	if((p->semaphore_ring)==semaphore_id)
		p->semaphore_ring=s->semaphore_back;
	if((p->semaphore_ring)==semaphore_id)
		p->semaphore_ring=(-1);
	else{
		front=s->semaphore_front;
		back=s->semaphore_back;
		os->os_semaphore[front].semaphore_back=back;
		os->os_semaphore[back].semaphore_front=front;
	}
	p=&(os->os_domain[destination_process]);
	p->semaphore_number++;
	if((p->semaphore_ring)==(-1)){
		p->semaphore_ring=semaphore_id;	
		s->semaphore_front=semaphore_id;
		s->semaphore_back=semaphore_id;
	}else{
		front=p->semaphore_ring;
		back=os->os_semaphore[front].semaphore_back;
		os->os_semaphore[semaphore_id].semaphore_front=front;
		os->os_semaphore[semaphore_id].semaphore_back=back;
		os->os_semaphore[front].semaphore_back=semaphore_id;
		os->os_semaphore[back].semaphore_front=semaphore_id;
	}
	s->owner_domain_id=destination_process; 
	return;
}
