#include"kernel.h" 
#define READY_HEAP_EXCHANGE(s_h,d_h)				\
{								\
	s_t=os->os_cpu_set[cpu_set_id].				\
		thread_heap.ready_heap[s_h].thread_id;		\
	d_t=os->os_cpu_set[cpu_set_id].				\
		thread_heap.ready_heap[d_h].thread_id;		\
	os->os_cpu_set[cpu_set_id].				\
		thread_heap.ready_heap[s_h].thread_id=d_t;	\
	os->os_cpu_set[cpu_set_id].				\
		thread_heap.ready_heap[d_h].thread_id=s_t;	\
	os->os_thread[s_t].heap_id=(d_h);			\
	os->os_thread[d_t].heap_id=s_h;				\
}
void ready_heap_up_deal(int thread_id)
{
	int s_t,d_t,h,p,t_p,h_p,p_p,cpu_set_id;

	cpu_set_id=os->os_thread[thread_id].cpu_set_id;

	h=os->os_thread[thread_id].heap_id;
	p=os->os_thread[thread_id].priority;
	for(;;){
		if(h<=0)
			return;
		h_p=(h-1)/2;
		t_p=os->os_cpu_set[cpu_set_id].
			thread_heap.ready_heap[h_p].thread_id;
		p_p=os->os_thread[t_p].priority;

		if(p>=p_p)
			return;
		READY_HEAP_EXCHANGE(h,h_p);
		h=h_p;
	}
	return;
}
void ready_heap_down_deal(int thread_id)
{
	int s_t,d_t,h1,h2,t1,t2,p1,p2,cpu_set_id;
	int h_l,h_r,t_l,t_r,p_l,p_r;

	cpu_set_id=os->os_thread[thread_id].cpu_set_id;

	t1=thread_id;
	h1=os->os_thread[t1].heap_id;
	p1=os->os_thread[t1].priority;
	for(;;){
		h_l=2*h1+1;
		h_r=2*h1+2;
		if(h_l>=os->os_cpu_set[cpu_set_id].
				thread_heap.ready_thread_number)
		{
			if(h_r>=os->os_cpu_set[cpu_set_id].
				thread_heap.ready_thread_number)
					break;
			else 
				h2=h_r;
		}else{
			if(h_r>=os->os_cpu_set[cpu_set_id].
				thread_heap.ready_thread_number)
					h2=h_l;
			else{
				t_l=os->os_cpu_set[cpu_set_id].
					thread_heap.ready_heap[h_l].
					thread_id;
				p_l=os->os_thread[t_l].priority;
				t_r=os->os_cpu_set[cpu_set_id].
					thread_heap.ready_heap[h_r].
					thread_id;
				p_r=os->os_thread[t_r].priority;
				if(p_l<p_r)
					h2=h_l;
				else
					h2=h_r;
			}
		}
		t2=os->os_cpu_set[cpu_set_id].
			thread_heap.ready_heap[h2].thread_id;
		p2=os->os_thread[t2].priority;
		if(p1<=p2)
			break;
		READY_HEAP_EXCHANGE(h1,h2);
		h1=h2;
	}
	return;
}
void remove_from_ready_heap(int thread_id)
{
	int s_t,d_t,h1,h2,t1,t2,p1,p2,cpu_set_id;

	cpu_set_id=os->os_thread[thread_id].cpu_set_id;

	h1=os->os_thread[thread_id].heap_id;
	h2=os->os_cpu_set[cpu_set_id].thread_heap.ready_thread_number-1;
	READY_HEAP_EXCHANGE(h1,h2);

	t1=os->os_cpu_set[cpu_set_id].
		thread_heap.ready_heap[h1].thread_id;
	p1=os->os_thread[t1].priority;
	t2=os->os_cpu_set[cpu_set_id].
		thread_heap.ready_heap[h2].thread_id;
	p2=os->os_thread[t2].priority;

	os->os_cpu_set[cpu_set_id].thread_heap.ready_thread_number--;

	if(p1==p2)
		return;
	if(p1<p2)
		ready_heap_up_deal(t1);
	else
		ready_heap_down_deal(t1);
	return;
}
void insert_into_ready_heap(int thread_id)
{
	int h,cpu_set_id=os->os_thread[thread_id].cpu_set_id;

	h=os->os_cpu_set[cpu_set_id].thread_heap.ready_thread_number;
	os->os_thread[thread_id].heap_id=h;
	os->os_cpu_set[cpu_set_id].
		thread_heap.ready_heap[h].thread_id=thread_id;
	os->os_cpu_set[cpu_set_id].thread_heap.ready_thread_number++;
	ready_heap_up_deal(thread_id); 
	return;
}
