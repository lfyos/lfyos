#include "kernel.h"

#define NOT_KERNEL_MODE			(-1)
#define NOT_SYSTEM_CAPABILITY		(-2)
#define SEMAPHORE_OPERATION_FAIL	(-3)
#define NOT_IN_MEMORY_BODY		(-4)
#define WRONG_SEMAPHORE_ID		(-5)
#define WRONG_CAPABILITY		(-6)

#define RT	(current->rt)
#define ENV	(current->env)
#define T_ID	(current->thread_id)
#define C_P_ID	(RT->current_domain_id)

#define RR	(ENV->reg.system_call)
#define R1	(ENV->reg.r1)
#define R2	(ENV->reg.r2)
#define R3	(ENV->reg.r3)
#define R4	(ENV->reg.r4)
#define R5	(ENV->reg.r5)
#define R6	(ENV->reg.r6)

#define CAP_1	(par->capability.capability_1)
#define CAP_2	(par->capability.capability_2) 
#define FLAG_1	((R1>=0)?TRUE:FALSE)
#define ABS_1	((R1>=0)?(R1):(-R1))

int time_call_process(union system_call_parameter *par)
{
	time_share_priority(current->thread_id);
	kernel_semaphore_time_process();
	return TRUE;
} 
int system_call_process(union system_call_parameter *par)
{
	int return_value=FALSE;
	switch(RR){
	default:
		RR=T_ID;
		R1=RT->user_semaphore_id;
		R2=C_P_ID;
		R3=current->t->priority;
		R4=current->t->cpu_set_id;
		R5=current->current_process->enter_thread_number;
		return_value=time_call_process(par);
		break;
	case THREAD_CALL_SYSTEM_CALL:
		RR=thread_migrate(T_ID,ABS_1,TRUE,FLAG_1,
			&(RT->physical_block));
		break;
	case THREAD_JUMP_SYSTEM_CALL:
		RR=thread_migrate(T_ID,ABS_1,FALSE,FLAG_1,
			&(RT->physical_block));
		break;
	case WITH_BLOCK_THREAD_CALL_SYSTEM_CALL:
		if(give_up_memory_body(RT)<0)
			RR=NOT_IN_MEMORY_BODY;
		else 
			RR=thread_migrate(T_ID,R1,TRUE,FALSE,
				&(par->thread_physical_block));
		break;
	case THREAD_RETURN_SYSTEM_CALL:
		RR=thread_return(T_ID,R1);
		break;
	case SEMAPHORE_INFORMATION_SYSTEM_CALL: 
		if((R1<=0)||(R1>=OS_SEMAPHORE_NUMBER))
			RR=WRONG_SEMAPHORE_ID; 
		else if(!KERNEL_COMPARE_CAPABILITY(
			(os->os_semaphore[R1].capability),CAP_1))
				RR=WRONG_CAPABILITY;
		else{
			copy_data(&(os->os_semaphore[R1]),
				&(par->semaphore_attribute),
				sizeof(struct semaphore_struct));
			RR=R1;
		}
		break;
	case SEMAPHORE_OPERATION_SYSTEM_CALL:
		if(R6)
			R6=give_up_memory_body(RT);
		if(R1!=0){
			if(R1>0)
				return_value=0;
			else{
				R1=0-R1;
				return_value=reset_semaphore_value(
					R1,&(CAP_1));
			}
			if(return_value<0)
				R1=return_value;
			else
				R1=system_call_v(R1,&(R2),&(CAP_1));
		}
		if(R3!=0){
			if(R3>0)
				return_value=0;
			else{
				R3=0-R3;
				return_value=reset_semaphore_value(
					R3,&(CAP_2));
			}
			if(return_value<0)
				R3=return_value;
			else
				R3=system_call_p(
					T_ID,R3,R4,R5,&(CAP_2));
		}
		R2=R3;
		R3=R6;
		if((R1<0)||(R2<0))
			RR=SEMAPHORE_OPERATION_FAIL;
		return_value=FALSE;
		break;
	case SEMAPHORE_TIME_SYSTEM_CALL:
		RR=set_semaphore_capability_time_and_p_operation(
			T_ID,ABS_1,FLAG_1,R2,R3,R4,R5,
			&(par->set_semaphore_time.first_time),
			&(par->set_semaphore_time.step_time),
			&(par->set_semaphore_time.
				old_semaphore_capability),
			&(par->set_semaphore_time.
				new_semaphore_capability));
		break;
	case CREATE_THREAD_SYSTEM_CALL:
		if(R1<0){
			R1=C_P_ID;
			COPY_CAPABILITY(
				(os->os_domain[R1].capability),CAP_1);
		} 
		RR=create_thread(&(RT->file[0]),R1,C_P_ID,
			local_create_thread_priority(T_ID,R1),
			current->t->cpu_set_id,ENV,&(CAP_1));
		break;
	case CREATE_REQUEST_THREAD_SYSTEM_CALL:
		if(IS_USER_MODE(RT->environment.point))
			RR=NOT_KERNEL_MODE;
		else
			RR=create_request_thread(
				&(par->thread_physical_block));
		break;
	case CPU_SET_SYSTEM_CALL:
		RR=change_cpu_set(R1,R2,&(CAP_1));
		break;
	case SEND_SIGNAL_SYSTEM_CALL:
		RR=send_signal_to_thread(T_ID,
			((R1<0)?current->thread_id:R1),&(R2),&(CAP_1));
		break;
	case RESOURCE_ALLOCATION_SYSTEM_CALL:
		if(R1<=0){
			R1=C_P_ID;
			COPY_CAPABILITY((os->os_domain[R1].capability),
				(par->domain_attribute.capability));
		}
		RR=allocate_resource(
			&(par->domain_attribute.domain_data),R1,
			&(par->domain_attribute.capability));
		break;
	case RESOURCE_QUERY_SYSTEM_CALL:
		if(R1<=0){
			R1=C_P_ID;
			COPY_CAPABILITY((os->os_domain[R1].capability),
				par->domain_attribute.capability);
		}
		RR=query_resource(
			&(par->domain_attribute.domain_data),R1);
		break;
	case SEMAPHORE_ALLOCATION_SYSTEM_CALL:
		RR=allocate_semaphore(R1,R2,&(CAP_1),&(CAP_2));
		break;
	case SEMAPHORE_RELEASE_SYSTEM_CALL:
		RR=release_semaphore(R1,&(CAP_1));
		break;
	case THREAD_SET_SYSTEM_CALL:
		if(R1<0){
			R1=T_ID;
			COPY_CAPABILITY(
				(os->os_domain[C_P_ID].capability),
				par->set_thread_attribute.
					current_domain_capability);
		}
		RR=set_thread_attribute(
			R1,&(par->set_thread_attribute));
		break;
	case THREAD_QUERY_SYSTEM_CALL:
		if(R1<0){
			R1=T_ID;
			COPY_CAPABILITY((os->os_domain[RT->
					current_domain_id].capability),
				par->get_thread_attribute.
					current_domain_capability);
		}
		RR=get_thread_attribute(
			R1,&(par->get_thread_attribute));
		break;
	case SYSTEM_FILE_SYSTEM_CALL:
		if(!KERNEL_COMPARE_CAPABILITY(
			(os->system_capability),(CAP_2)))
			RR=NOT_SYSTEM_CAPABILITY;
		else{
			RR=set_system_file_capability(
				R1,R2,R3,&(CAP_1),R4,R5);
			current->file_change_flag++;
		}
		break;
	case THREAD_FILE_SYSTEM_CALL:
		if(R1>=0)
			RR=set_return_stack_file_capability(
				T_ID,R1,R2,R3,&(CAP_1),R4,R5);
		else
			RR=set_all_return_stack_file_capability(T_ID,
			&(par->set_user_file_information.
				set_flag_and_result[0]),
			&(par->set_user_file_information.file[0]));
		current->file_change_flag++;
		break;
	case SHUTDOWN_SYSTEM_CALL:
		RR=shutdown_process(&(R1),&(R2));
		break;
	}
	return return_value;
}
