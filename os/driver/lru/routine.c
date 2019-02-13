#include"../driver.h" 
int memory_call_process(int routine_id,union memory_call_parameter *par)
{
	int return_value=0; 
	switch(routine_id){
	default:
		return_value=(-1);
		break;
	case CONTROL_UNOPENED_FILE_MEMORY_CALL_FROM_ROOT:
		SET_INITIAL_FILE_WINDOW(
			par->open_or_control_file.kernel_file_window);
	case CONTROL_UNOPENED_FILE_MEMORY_CALL:
		return_value=control_unopened_file(
			par->open_or_control_file.command,
			par->open_or_control_file.flag,
			par->open_or_control_file.begin_address,
			par->open_or_control_file.data_length,
			current->rt->physical_block.memory_id,
			current->rt->physical_block.block_id,
			&(par->open_or_control_file.kernel_file_window),
			par->open_or_control_file.memory_domain,
			current->t->semaphore_id,
			&(par->open_or_control_file.
				domain_capability));
		break;
	case CONTROL_OPENED_FILE_MEMORY_CALL:
		return_value=control_opened_file(
			current->t->semaphore_id,
			current->rt->physical_block.memory_id,
			current->rt->physical_block.block_id,
			par->control_opened_file.command,
			par->control_opened_file.file_window_id,
			par->control_opened_file.begin_address,
			par->control_opened_file.data_length,
			par->control_opened_file.flag,
			&(par->control_opened_file.file_capability));
		break;
	case OPEN_FILE_WINDOW_MEMORY_CALL: 
		return_value=open_file_window(
			&(par->open_or_control_file.kernel_file_window),
			par->open_or_control_file.flag,
			par->open_or_control_file.memory_domain, 
			&(par->open_or_control_file.domain_capability),
			current->t->semaphore_id); 
		break; 
	case CLOSE_FILE_WINDOW_MEMORY_CALL:
		return_value=close_file_window(
			par->close_file_window.file_window_id,
			par->close_file_window.giveup_flag,
			par->close_file_window.wait_flag,
			&(par->close_file_window.file_capability),
			current->t->semaphore_id,
			&(par->close_file_window.flush_number[0]));
		break;
	case SETUP_MAP_MEMORY_CALL:
		return_value=memory_setup_map(current->t->semaphore_id,
			par->memory_setup_map.file_window_id,
			par->memory_setup_map.begin_logic_address,
			par->memory_setup_map.end_logic_address,
			par->memory_setup_map.read_only_flag,
			&(par->memory_setup_map.file_capability));
		break;
	case FLUSH_PROCESS_MEMORY_CALL:
		return_value=flush_memory_domain(
			par->flush_memory_domain.max_flush_number,
			par->flush_memory_domain.giveup_flag,
			par->flush_memory_domain.free_block_flag,
			par->flush_memory_domain.wait_flag,
			par->flush_memory_domain.domain_id,
			current->t->semaphore_id,
			&(par->flush_memory_domain.domain_capability),
			&(par->flush_memory_domain.flush_number[0]));
		break;	
	case FLUSH_FILE_WINDOW_MEMORY_CALL:
		return_value=flush_file_window(
			par->flush_file_window.file_window_id,
			par->flush_file_window.update_file_window_id, 
			par->flush_file_window.max_flush_number,
			par->flush_file_window.giveup_flag,
			par->flush_file_window.free_block_flag,
			par->flush_file_window.wait_flag,
			current->t->semaphore_id,
			&(par->flush_file_window.file_capability),
			&(par->flush_file_window.
				update_file_capability),
			&(par->flush_file_window.flush_number[0]));
		break;
	case QUERY_PROCESS_MEMORY_CALL:
		return_value=query_memory_resource(
			par->memory_resource.domain_id,
			&(par->memory_resource.domain_data));
		par->memory_resource.free_block_number=
			memory_body->head->free_block_number;
		par->memory_resource.free_physical_block_number=
			memory_body->head->free_physical_block;
		break;
	case SET_PROCESS_MEMORY_CALL:
		return_value=allocate_memory_resource(
			par->memory_resource.domain_id,
			&(par->memory_resource.domain_data));
		if(return_value>=0)
			return_value=query_memory_resource(
				par->memory_resource.domain_id,
				&(par->memory_resource.domain_data));
		par->memory_resource.free_block_number=
			memory_body->head->free_block_number;
		par->memory_resource.free_physical_block_number=
			memory_body->head->free_physical_block;
		break;
	case QUERY_FILE_WINDOW_MEMORY_CALL:
		return_value=set_query_file_window_attribute(FALSE, 
			par->file_attribute.file_window_id,
			&(par->file_attribute.file_window));
		break;
	case SET_FILE_WINDOW_MEMORY_CALL:
		return_value=set_query_file_window_attribute(TRUE,
			par->file_attribute.file_window_id,
			&(par->file_attribute.file_window));
		break;
	case WAKEUP_FILE_WINDOW_MEMORY_CALL:
		return_value=wakeup_file_window(
			par->wakeup_file_window.file_window_id,
			par->wakeup_file_window.no_wait_flag,
			par->wakeup_file_window.max_number,
			&(par->wakeup_file_window.file_capability));
		break;
	case WAKEUP_MEMORY_DOMAIN_MEMORY_CALL:
		return_value=wakeup_memory_domain(
			par->wakeup_memory_domain.domain_id,
			par->wakeup_memory_domain.no_wait_flag,
			par->wakeup_memory_domain.max_number,
			par->wakeup_memory_domain.max_file_number,
			&(par->wakeup_memory_domain.domain_capability));
		break;
	case INIT_MEMORY_BODY:
		if(!KERNEL_COMPARE_CAPABILITY((os->system_capability),
			(par->memory_body_parameter.capability)))
				break;
		return_value=init_memory_body_routine(
			memory_body,1,&(par->memory_body_parameter),
			driver[current->rt->current_memory_body_id].
				get_memory_body_base(),
			driver[current->rt->current_memory_body_id].
				memory_body_capacity);
		break;
	case LINK_MEMORY_BODY:
		if(!KERNEL_COMPARE_CAPABILITY((os->system_capability),
			(par->memory_body_parameter.capability)))
				break;
		return_value=init_memory_body_routine(
			memory_body,2,&(par->memory_body_parameter),
			driver[current->rt->current_memory_body_id].
				get_memory_body_base(),
			driver[current->rt->current_memory_body_id].
				memory_body_capacity);
		break;
	} 
	return return_value;
}
