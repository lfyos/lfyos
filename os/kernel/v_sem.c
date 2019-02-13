#include"kernel.h"
#define WRONG_SEMAPHORE_ID	(-1)
#define WRONG_THREAD_STATE	(-2)
#define WRONG_CAPABILITY	(-3)
static int v(int semaphore_id,int *steps,CAPABILITY *cap)
{
	struct semaphore_struct *s;
	struct thread_struct *t;
	int i,thread_id,fr_th,ba_th,my_step,priority;

	if((semaphore_id<0)||(semaphore_id>=OS_SEMAPHORE_NUMBER))
		return WRONG_SEMAPHORE_ID;
	s=&(os->os_semaphore[semaphore_id]);
	if(!KERNEL_COMPARE_CAPABILITY(s->capability,(*cap)))
		return WRONG_CAPABILITY;
	if((my_step=(*steps))==0){
		*steps=s->value;
		return 0;
	};
	for(i=0;;i++){
		thread_id=s->thread_ring;
		if(my_step>0){
			if((thread_id<0)||(i>=my_step)){
				s->value=s->value+my_step-i;
				break;
			}
		}else{
			if(thread_id<0)
				break;
		};
		t=&(os->os_thread[thread_id]);
		thread_begin_v_semaphore_priority(thread_id,semaphore_id);
		s->value++;
		fr_th=t->semaphore_front;
		ba_th=t->semaphore_back;
		if(fr_th==thread_id)
			s->thread_ring=-1;
		else{
			s->thread_ring=fr_th;
			os->os_thread[fr_th].semaphore_back=ba_th;
			os->os_thread[ba_th].semaphore_front=fr_th;
		}
		t->sleep_semaphore_id=(-1);
		if(((t->flag)&THREAD_SET_V_OPERATION_RESULT_FLAG)!=0)
			os->os_return_stack[t->return_stack_pointer].
				environment.reg.system_call=i+2;
		insert_into_heap(thread_id);
		priority=thread_end_v_semaphore_priority(
			thread_id,semaphore_id);
		modify_thread_priority(thread_id,
			priority,t->cpu_set_id);
	}
	*steps=s->value;

	return i;
}
int v_thread(int thread_id,int time_out_flag)
{
	int value;
	struct thread_struct *t;

	if((t=&(os->os_thread[thread_id]))->state!=THREAD_SLEEP_STATE)
		return WRONG_THREAD_STATE;
	os->os_semaphore[t->sleep_semaphore_id].thread_ring=thread_id;
	value=1;
	return(v(t->sleep_semaphore_id,&value,&(os->system_capability)));
}
int system_call_v(int semaphore_id,int *steps,CAPABILITY *cap)
{
	if(semaphore_id==0)
		return WRONG_SEMAPHORE_ID;
	else
		return v(semaphore_id,steps,cap);
}
