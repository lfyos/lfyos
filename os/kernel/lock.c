#include"kernel.h"
int lock_os_kernel()
{
	if(operate_spinlock(&(os->spinlock.os_lock),-1)>0)
		return TRUE;
	operate_spinlock(&(os->spinlock.os_lock),1);
	return FALSE;
}
void unlock_os_kernel()
{
	operate_spinlock(&(os->spinlock.os_lock),1);
}
int lock_os_thread(int thread_id)
{
	int *pointer;
	pointer=&(os->spinlock.thread_lock[thread_id]);
	if(operate_spinlock(pointer,-1)>0)
		return TRUE;
	operate_spinlock(pointer,1);
	return FALSE;
}
void unlock_os_thread(int thread_id)
{
	int *pointer;
	pointer=&(os->spinlock.thread_lock[thread_id]);
	operate_spinlock(pointer,1);
}
