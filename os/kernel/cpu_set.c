#include"kernel.h"
#define WRONG_CPU_ID		(-1)
#define WRONG_NEW_CPU_SET_ID	(-2)
#define WRONG_SYSTEM_CAPABILITY	(-3)
#define WRONG_STACK_LOCATION	(-4)
#define WRONG_TOP		(-5) 
int change_cpu_set(int cpu_id,int new_cpu_set_id,
	CAPABILITY *system_capability)
{
	int old_cpu_set_id,thread_id,stack_location,top,top_cpu_id;

	if((cpu_id<0)||(cpu_id>=OS_CPU_NUMBER))
		return WRONG_CPU_ID;
	if((new_cpu_set_id<0)||(new_cpu_set_id>=OS_CPU_SET_NUMBER))
		return WRONG_NEW_CPU_SET_ID;
	if(!KERNEL_COMPARE_CAPABILITY(
		(os->system_capability),(*system_capability)))
			return WRONG_SYSTEM_CAPABILITY;
	old_cpu_set_id=os->os_cpu[cpu_id].cpu_set_id;
	if(old_cpu_set_id==new_cpu_set_id)
		return new_cpu_set_id;
	if((thread_id=(os->os_cpu[cpu_id].thread_id))>=0)
		remove_from_heap(thread_id);
	if((stack_location=(os->os_cpu[cpu_id].stack_location))<0){
		if(thread_id>=0)
			insert_into_heap(thread_id);
		return WRONG_STACK_LOCATION;
	}
	top=os->os_cpu_set[old_cpu_set_id].free_cpu.free_cpu_top-1;
	if(top<0){
		if(thread_id>=0)
			insert_into_heap(thread_id);
		return WRONG_TOP;
	}
	top_cpu_id=os->os_cpu_set[old_cpu_set_id].
		free_cpu.free_cpu[top].cpu_id;
	os->os_cpu[top_cpu_id].thread_id=(-1);
	os->os_cpu[top_cpu_id].cpu_set_id=old_cpu_set_id;
	os->os_cpu[top_cpu_id].stack_location=stack_location;
	os->os_cpu_set[old_cpu_set_id].free_cpu.
		free_cpu[stack_location].cpu_id=top_cpu_id;
	os->os_cpu_set[old_cpu_set_id].free_cpu.free_cpu_top--;
	top=os->os_cpu_set[new_cpu_set_id].free_cpu.free_cpu_top;
	os->os_cpu[cpu_id].thread_id=(-1);
	os->os_cpu[cpu_id].cpu_set_id=new_cpu_set_id;
	os->os_cpu[cpu_id].stack_location=top;
	os->os_cpu_set[new_cpu_set_id].free_cpu.
		free_cpu[top].cpu_id=cpu_id;
	os->os_cpu_set[new_cpu_set_id].free_cpu.free_cpu_top++;
	if(thread_id>=0)
		insert_into_heap(thread_id);
	if(os->os_cpu_set[new_cpu_set_id].
		thread_heap.ready_thread_number>0)
	{
		thread_id=os->os_cpu_set[new_cpu_set_id].
			thread_heap.ready_heap[0].thread_id;
		remove_from_heap(thread_id);
		insert_into_heap(thread_id);
	}
	return new_cpu_set_id;
}
