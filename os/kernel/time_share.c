#include "kernel.h" 
#define MAX_TICKS	(124*1024) 
void time_share_priority(int tid)
{
	static int t_number=0;
	t_number++;
	if(t_number<0)
		t_number=0;
	if(t_number<MAX_TICKS){
		if((tid>=0)&&(tid<OS_THREAD_NUMBER))
			modify_thread_priority(tid,t_number,-1);
	}else if((tid=t_number-MAX_TICKS)>=OS_THREAD_NUMBER)
		t_number=0; 
	else
		modify_thread_priority(tid,OS_THREAD_NUMBER-tid,-1);
	return;
}
