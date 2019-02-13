#include"kernel.h"
#define WRONG_THREAD_ID 	(-1)
#define WRONG_CAPABILITY 	(-2)
#define NO_RETURN_STACK		(-3)
#define WRONG_RETURN_STACK	(-4) 
int get_thread_attribute(int thread_id,struct thread_get_attribute*attr)
{
	int rt_id;
	struct thread_struct *t;
	struct return_stack *rt;
	CAPABILITY *cap_s,*cap_d;

	if((thread_id<0)||(thread_id>=OS_THREAD_NUMBER))
		return WRONG_THREAD_ID;
	t=&(os->os_thread[thread_id]);
	if((rt_id=t->return_stack_pointer)<0)
		return NO_RETURN_STACK;
	rt=&(os->os_return_stack[rt_id]);
	cap_s=&(os->os_domain[rt->current_domain_id].capability);
	cap_d=&(attr->current_domain_capability);
	if(!KERNEL_COMPARE_CAPABILITY((*cap_s),(*cap_d)))
		return WRONG_CAPABILITY;
	copy_data(rt,&(attr->return_stack_data),sizeof(*rt));
	copy_data(t, &(attr->thread_data),sizeof(*t));
	cap_s=&(os->os_semaphore[rt->user_semaphore_id].capability);
	cap_d=&(attr->user_semaphore_capability);
	COPY_CAPABILITY((*cap_s),(*cap_d));
	return thread_id;
} 
int set_thread_attribute(int thread_id,struct thread_set_attribute*attr)
{
	int i,n,return_value,rt_id,pid,old_signal_bitmap;
	struct thread_struct *t;
	struct return_stack *rt;
	CAPABILITY *cap_s,*cap_d;

	if((thread_id<0)||(thread_id>=OS_THREAD_NUMBER))
		return WRONG_THREAD_ID;
	t=&(os->os_thread[thread_id]);
	if((rt_id=t->return_stack_pointer)<0)
		return NO_RETURN_STACK;
	rt=&(os->os_return_stack[rt_id]);

	pid=rt->current_domain_id;
	cap_s=&(os->os_domain[pid].capability);
	cap_d=&(attr->current_domain_capability);
	if(!KERNEL_COMPARE_CAPABILITY(*cap_s,*cap_d))
		return WRONG_CAPABILITY;
	if(attr->priority<(n=min_thread_priority(thread_id,pid,cap_d)))
		attr->priority=n;
	modify_thread_priority(thread_id,
		attr->priority,attr->cpu_set_id);

	old_signal_bitmap=rt->signal_bitmap;
	rt->signal_bitmap|=attr->signal_bitmap;
	attr->signal_bitmap=old_signal_bitmap;
	
	for(i=0,n=0;;i++,n++){
		if((i>=attr->mandatory_exit_number)||(rt_id<0))
			break;
		rt=&(os->os_return_stack[rt_id]);
		pid=rt->current_domain_id;
		cap_s=&(os->os_domain[pid].capability);
		cap_d=&(attr->current_domain_capability);
		if(!KERNEL_COMPARE_CAPABILITY(*cap_s,*cap_d))
			break;
		if(t->state==THREAD_SLEEP_STATE)
			if((return_value=v_thread(thread_id,FALSE))<0)
				return return_value;
		rt->flag|=RETURN_STACK_TIME_OUT_FLAG;
		rt_id=rt->next_return_stack;
	}
	attr->mandatory_exit_number=n;
	return n;
}
int send_signal_to_thread(int sender_id,int receiver_id,
	int *signal_bitmap,CAPABILITY *cap)
{
	int rt_id,old_signal_bitmap,rpid;
	struct thread_struct *sender_t,*receiver_t;
	struct return_stack *sender_rt,*receiver_rt;

	if((sender_id<0)||(sender_id>=OS_THREAD_NUMBER))
		return WRONG_THREAD_ID;
	sender_t=&(os->os_thread[sender_id]); 
	rt_id=sender_t->return_stack_pointer;
	if((rt_id<0)||(rt_id>=OS_RETURN_STACK_NUMBER))
		return WRONG_RETURN_STACK;
	sender_rt=&(os->os_return_stack[rt_id]);

	if((receiver_id<0)||(receiver_id>=OS_THREAD_NUMBER))
		return WRONG_THREAD_ID;
	receiver_t=&(os->os_thread[receiver_id]); 
	rt_id=receiver_t->return_stack_pointer;
	if((rt_id<0)||(rt_id>=OS_RETURN_STACK_NUMBER))
		return WRONG_RETURN_STACK;
	receiver_rt=&(os->os_return_stack[rt_id]);
	
	rpid=receiver_rt->current_domain_id;
	if(rpid!=(sender_rt->current_domain_id))
		if(!KERNEL_COMPARE_CAPABILITY(
			(os->os_domain[rpid].capability),(*cap)))
				return WRONG_CAPABILITY;
	
	old_signal_bitmap=receiver_rt->signal_bitmap;
	receiver_rt->signal_bitmap|=(*signal_bitmap);
	*signal_bitmap=old_signal_bitmap;
	return 0;
}
