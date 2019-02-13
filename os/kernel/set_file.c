#include"kernel.h"
#define WRONG_THREAD_ID			(-1)
#define WRONG_THREAD_FILE_ID		(-2)
#define WRONG_SYSTEM_FILE_ID		(-5)
#define WRONG_RETURN_STACK_ID		(-6)
#define SET_FILE_CAPABILITY					\
{								\
	u_i->memory_body_id=memory_body_id;			\
	u_i->file_window_id=file_window_id;			\
	u_i->map_length=map_length;				\
	u_i->right=right;					\
	COPY_CAPABILITY((*file_capability),(u_i->file_capability));\
}
int set_return_stack_file_capability(int thread_id,int thread_file_id,
	int map_length,int right,CAPABILITY *file_capability,
	int memory_body_id,int file_window_id)
{
	int rt_id;
	struct return_stack *rt;
	struct user_file_information *u_i;

	if((thread_id<0)||(thread_id>=OS_THREAD_NUMBER))
		return WRONG_THREAD_ID;
	if((thread_file_id<0)||(thread_file_id>=OS_USER_FILE_NUMBER))
		return WRONG_THREAD_FILE_ID;
	rt_id=os->os_thread[thread_id].return_stack_pointer;
	if((rt_id<0)||(rt_id>=OS_RETURN_STACK_NUMBER))
		return WRONG_RETURN_STACK_ID;
	rt=&(os->os_return_stack[rt_id]);
	u_i=&(rt->file[thread_file_id]);
	SET_FILE_CAPABILITY;

	return file_window_id;
}
int set_system_file_capability(int system_file_id,int map_length,
	int right,CAPABILITY *file_capability,
	int memory_body_id,int file_window_id)
{
	struct user_file_information *u_i;

	if((system_file_id<0)||(system_file_id>=OS_USER_FILE_NUMBER))
		return WRONG_SYSTEM_FILE_ID;
	u_i=&(os->os_file[system_file_id]);
	SET_FILE_CAPABILITY; 
	return file_window_id;
}
int set_all_return_stack_file_capability(int thread_id,
	int *set_flag_and_result,struct user_file_information *par)
{
	int i,return_value;

	for(i=0,return_value=0;i<OS_USER_FILE_NUMBER;i++){
		if(*set_flag_and_result)
			*set_flag_and_result
			=set_return_stack_file_capability(thread_id,i,
			par->map_length,par->right,&(par->file_capability),
			par->memory_body_id,par->file_window_id);
		else
			*set_flag_and_result=0;

		if((*set_flag_and_result)<0)
			return_value=(*set_flag_and_result);
		set_flag_and_result++;
		par++;
	}
	return return_value;
}
