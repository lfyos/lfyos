#include"kernel.h"
void leave_run_state(int thread_id)
{
	int cpu_set_id,cpu_id,stack_location;

	cpu_set_id=os->os_thread[thread_id].cpu_set_id;
	cpu_id=os->os_thread[thread_id].cpu_id;

	os->os_thread[thread_id].cpu_id=(-1);

	stack_location=os->os_cpu_set[cpu_set_id].free_cpu.free_cpu_top;

	os->os_cpu[cpu_id].thread_id=(-1);
	os->os_cpu[cpu_id].stack_location=stack_location;
	os->os_cpu[cpu_id].cpu_set_id=cpu_set_id;

	os->os_cpu_set[cpu_set_id].free_cpu.
		free_cpu[stack_location].cpu_id=cpu_id;
	os->os_cpu_set[cpu_set_id].free_cpu.free_cpu_top++;

	return;
}
void enter_run_state(int thread_id)
{
	int cpu_set_id,cpu_id,top;

	cpu_set_id=os->os_thread[thread_id].cpu_set_id;

	os->os_cpu_set[cpu_set_id].free_cpu.free_cpu_top--;
	top=os->os_cpu_set[cpu_set_id].free_cpu.free_cpu_top;
	cpu_id=os->os_cpu_set[cpu_set_id].free_cpu.free_cpu[top].cpu_id;

	os->os_cpu[cpu_id].thread_id=thread_id;
	os->os_cpu[cpu_id].stack_location=(-1);
	os->os_cpu[cpu_id].cpu_set_id=cpu_set_id;

	os->os_thread[thread_id].cpu_id=cpu_id;

	return;
}
