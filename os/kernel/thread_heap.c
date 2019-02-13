#include"kernel.h"
static void remind_thread_do_switch(int thread_id)
{
	int physical_cpu_id;

	physical_cpu_id=os->os_thread[thread_id].physical_cpu_id;
	if(physical_cpu_id>=0)
		if(physical_cpu_id!=get_physical_cpu())
			remind_physical_cpu_do_switch(physical_cpu_id);
	return;
}
void insert_into_heap(int thread_id)
{
	int t_run,p,p_run,cpu_set_id;

	cpu_set_id=os->os_thread[thread_id].cpu_set_id;
	if(os->os_thread[thread_id].state!=THREAD_SLEEP_STATE)
		return;	
	if((os->os_cpu_set[cpu_set_id].free_cpu.free_cpu_top)>0){
		os->os_thread[thread_id].state=THREAD_RUN_STATE;
		insert_into_run_heap(thread_id);
		enter_run_state(thread_id);
		return;
	}
	if((os->os_cpu_set[cpu_set_id].thread_heap.
		run_thread_number)<=0)
	{
		os->os_thread[thread_id].state=THREAD_READY_STATE;
		insert_into_ready_heap(thread_id);
		return;
	}
	p=os->os_thread[thread_id].priority;
	t_run=os->os_cpu_set[cpu_set_id].
		thread_heap.run_heap[0].thread_id;
	p_run=os->os_thread[t_run].priority;

	if(p>=p_run){
		os->os_thread[thread_id].state=THREAD_READY_STATE;
		insert_into_ready_heap(thread_id);
		return;
	}
	os->os_thread[thread_id].state=THREAD_RUN_STATE;
	os->os_thread[thread_id].heap_id=0;
	os->os_cpu_set[cpu_set_id].
		thread_heap.run_heap[0].thread_id=thread_id;
	run_heap_down_deal(thread_id);
	os->os_thread[t_run].state=THREAD_READY_STATE;
	insert_into_ready_heap(t_run);
	leave_run_state(t_run);
	enter_run_state(thread_id);
	remind_thread_do_switch(t_run);
	return;
}
void remove_from_heap(int thread_id)
{
	int t_run,h_run,t_ready,h_ready,cpu_set_id;

	switch(os->os_thread[thread_id].state){
	case THREAD_RUN_STATE:
		cpu_set_id=os->os_thread[thread_id].cpu_set_id;
		if(os->os_cpu_set[cpu_set_id].
				thread_heap.ready_thread_number<=0)
		{
			remove_from_run_heap(thread_id);
			os->os_thread[thread_id].state
				=THREAD_SLEEP_STATE;
			leave_run_state(thread_id);
			remind_thread_do_switch(thread_id);
			break;
		}
		t_run=thread_id;
		h_run=os->os_thread[t_run].heap_id;
		t_ready=os->os_cpu_set[cpu_set_id].
			thread_heap.ready_heap[0].thread_id;
		h_ready=0;
		os->os_cpu_set[cpu_set_id].
			thread_heap.run_heap[h_run].thread_id=t_ready;
		os->os_cpu_set[cpu_set_id].
			thread_heap.ready_heap[h_ready].thread_id=t_run;
		os->os_thread[t_run].heap_id=h_ready;
		os->os_thread[t_run].state=THREAD_READY_STATE;
		os->os_thread[t_ready].heap_id=h_run;
		os->os_thread[t_ready].state=THREAD_RUN_STATE;
		run_heap_up_deal(t_ready);
		remove_from_ready_heap(t_run);
		os->os_thread[thread_id].state=THREAD_SLEEP_STATE;
		leave_run_state(t_run);
		enter_run_state(t_ready);
		remind_thread_do_switch(t_run);
		break;
	case THREAD_READY_STATE:
		remove_from_ready_heap(thread_id);
		os->os_thread[thread_id].state=THREAD_SLEEP_STATE;
		break;
	default:
		break;
	}
	return;
}
