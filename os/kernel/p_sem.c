#include"kernel.h"
#define WRONG_THREAD_ID		(-1)
#define WRONG_SEMAPHORE_ID	(-2)
#define WRONG_THREAD_STATE	(-3)
#define WRONG_CAPABILITY	(-4) 
int p(int thread_id,int semaphore_id,int sleep_flag,
	int result_flag,CAPABILITY *cap)
{
	int priority;
	struct thread_struct *t;
	struct semaphore_struct *s;

	if((thread_id<0)||(thread_id>=OS_THREAD_NUMBER))
		return(WRONG_THREAD_ID);
	t=&(os->os_thread[thread_id]);
	if((t->state!=THREAD_RUN_STATE)&&(t->state!=THREAD_READY_STATE))
		return WRONG_THREAD_STATE;
	if((semaphore_id<0)||(semaphore_id>=OS_SEMAPHORE_NUMBER))
		return WRONG_SEMAPHORE_ID;
	s=&(os->os_semaphore[semaphore_id]);
	if(!KERNEL_COMPARE_CAPABILITY(s->capability,(*cap)))
		return WRONG_CAPABILITY;

	thread_begin_p_semaphore_priority(thread_id,semaphore_id); 
	s->value--;
	if(s->value>=0){
		thread_end_p_semaphore_priority(thread_id,semaphore_id);
		return 1;
	}
	if(!sleep_flag){
		s->value++;
		thread_end_p_semaphore_priority(thread_id,semaphore_id);
		return 0;
	}
	if(s->thread_ring<0){
		t->semaphore_front=thread_id;
		t->semaphore_back=thread_id;
		s->thread_ring=thread_id;
	}else{
		t->semaphore_front=s->thread_ring;
		t->semaphore_back
			=os->os_thread[s->thread_ring].semaphore_back;
		os->os_thread[t->semaphore_front].semaphore_back
			=thread_id;
		os->os_thread[t->semaphore_back].semaphore_front
			=thread_id;
	}
	t->sleep_semaphore_id=semaphore_id;
	remove_from_heap(thread_id);
	if(result_flag)
		t->flag|=THREAD_SET_V_OPERATION_RESULT_FLAG;
	else
		t->flag&=(~THREAD_SET_V_OPERATION_RESULT_FLAG);
	priority=thread_end_p_semaphore_priority(
		thread_id,semaphore_id);
	modify_thread_priority(thread_id,priority,t->cpu_set_id);

	return 0;
}
