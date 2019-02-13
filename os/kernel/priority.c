#include "kernel.h"
int local_create_thread_priority(int thread_id,int domain_id)
{
	return (os->os_thread[thread_id].priority);
}
int remote_create_thread_priority(int priority,int domain_id)
{
	return priority;
}
int timeout_create_thread_priority(int domain_id)
{
	return (os->os_domain[domain_id].priority_policy);
}
int shutdown_create_thread_priority(int domain_id)
{
	return (os->os_domain[domain_id].priority_policy);
}
int min_thread_priority(int thread_id,int domain_id,CAPABILITY *cap)
{
	return os->os_thread[thread_id].priority;
}
void begin_thread_migration_priority(
	int thread_id,int current_domain_id,int domain_id)
{
	return ;
}
int end_thread_migration_priority(
	int thread_id,int current_domain_id,int domain_id)
{
	return os->os_thread[thread_id].priority;
}
void begin_thread_return_priority(int thread_id)
{
	return ;
}
void thread_return_priority(int thread_id)
{
	return ;
}
int end_thread_return_priority(int thread_id)
{
	return os->os_thread[thread_id].priority;
}
void thread_begin_p_semaphore_priority(int thread_id,int semaphore_id)
{
	return;
}
int thread_end_p_semaphore_priority(int thread_id,int semaphore_id)
{
	return os->os_thread[thread_id].priority;
}
void thread_begin_v_semaphore_priority(int thread_id,int semaphore_id)
{
	return;
}
int thread_end_v_semaphore_priority(int thread_id,int semaphore_id)
{
	return os->os_thread[thread_id].priority;
}
