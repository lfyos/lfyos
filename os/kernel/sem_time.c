#include"kernel.h"
static void semaphore_go_up(
	int semaphore_id,struct kernel_time *new_time)
{
	int s_id,h_id,s_parent,h_parent;
	struct kernel_time *current_time,*parent;

	s_id=semaphore_id;
	h_id=os->os_semaphore[s_id].heap_id;
	current_time=&(os->os_semaphore[s_id].first);
	COPY_TIME((*new_time),(*current_time));
	for(;;){
		if(h_id<=0)
			break;
		h_parent=((h_id-1)/2);
		s_parent=os->os_semaphore_heap[h_parent].semaphore_id;
		parent=&(os->os_semaphore[s_parent].first);
		if(TIME_GREAT_EQUAL((*current_time),(*parent)))
			break;
		os->os_semaphore_heap[h_parent].semaphore_id=s_id;
		os->os_semaphore_heap[h_id].semaphore_id=s_parent;
		os->os_semaphore[s_id].heap_id=h_parent;
		os->os_semaphore[s_parent].heap_id=h_id;
		h_id=h_parent;
	}
	return;
}
static void semaphore_go_down(
	int semaphore_id,struct kernel_time *new_time)
{
	int h_id,h_left,h_right,h_child,s_id,s_left,s_right,s_child;
	struct kernel_time *current_time,*left,*right,*child;

	s_id=semaphore_id;
	h_id=os->os_semaphore[s_id].heap_id;
	current_time=&(os->os_semaphore[s_id].first);
	COPY_TIME((*new_time),(*current_time));
	for(;;){
		h_left =(h_id+h_id+1);
		h_right=(h_id+h_id+2);
		if(h_left>=OS_SEMAPHORE_NUMBER)
			break;
		if(h_right>=OS_SEMAPHORE_NUMBER){
			h_child=h_left;
			s_child=os->os_semaphore_heap[h_left].
				semaphore_id;
			child=&(os->os_semaphore[s_child].first);
		}else{
			s_left=os->os_semaphore_heap[h_left].
				semaphore_id;
			s_right=os->os_semaphore_heap[h_right].
				semaphore_id;
			left=&(os->os_semaphore[s_left].first);
			right=&(os->os_semaphore[s_right].first);
			if(TIME_GREAT_EQUAL((*left),(*right))){
				s_child=s_right;
				h_child=h_right;
				child=right;
			}else{
				s_child=s_left;
				h_child=h_left;
				child=left;
			}
		}
		if(TIME_GREAT_EQUAL((*child),(*current_time)))
			break;
		os->os_semaphore_heap[h_child].semaphore_id=s_id;
		os->os_semaphore_heap[h_id].semaphore_id=s_child;
		os->os_semaphore[s_id].heap_id=h_child;
		os->os_semaphore[s_child].heap_id=h_id;

		h_id=h_child;
	}
	return;
}
void set_semaphore_time(int semaphore_id,
	struct kernel_time *new_time,int semaphore_action_flag)
{
	struct kernel_time *current_time;

	if((semaphore_id<=0)||(semaphore_id>=OS_SEMAPHORE_NUMBER))
		return;
	current_time=&(os->os_semaphore[semaphore_id].first);
	if(TIME_LESS((*new_time),(*current_time)))
		semaphore_go_up(semaphore_id,new_time);
	else if(TIME_GREAT((*new_time),(*current_time)))
		semaphore_go_down(semaphore_id,new_time);
	os->os_semaphore[semaphore_id].timer_action_flag
		=semaphore_action_flag;
	return;
}
