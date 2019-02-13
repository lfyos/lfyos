#include"../include/os.h"
#include"../arch/sys_c/par.h"
#include"../platform/include/service.h" 
static void display_memory_domain()
{
	struct memory_domain_struct *p;
	print_string("\ninput memory process ID:"); 
	p=&(get_memory_parameter()->memory_resource.domain_data);
	get_memory_parameter()->memory_resource.domain_id
		=get_number();
	RESET_CAPABILITY(p->capability);
	print("\nresult is ",
		call_memory(MEMORY_BODY_ID,QUERY_PROCESS_MEMORY_CALL));
	print_string("\nsystem information");
	print("\n          system free block number ",
		get_memory_parameter()->
		memory_resource.free_block_number);
	print("\n          system free physical block number ",
		get_memory_parameter()->
		memory_resource.free_physical_block_number);

	print_string("\nprocess information");
	print("\n          read block number ",p->read_block_number);
	print("\n          write block number ",p->write_block_number);
	print("\n          max block number ",p->max_block_number);
	print("\n          current used block number ",p->block_number);
	print("\n          max physical number",
		p->physical_max_block_number);
	print("\n          current userd physical number",
		p->physical_block_number);
	
	print_string("\ntrigger");
	print("\n         write block number",
		p->trigger.write_block_number);
	print("\n         free block number",
		p->trigger.free_block_number);
	print("\n         system write block number",
		p->trigger.system_write_block_number);
	print("\n         system free block number",
		p->trigger.system_free_block_number);

	print_string("\nphysical trigger");
	print("\n         write block number",
		p->physical_trigger.write_block_number);
	print("\n         free block number",
		p->physical_trigger.free_block_number);
	print("\n         system write block number",
		p->physical_trigger.system_write_block_number);
	print("\n         system free block number",
		p->physical_trigger.system_free_block_number);
	return;
}
static void display_thread()
{
	call_kernel_register r;
	print_string("\ninput thread id");
	r.system_call=THREAD_QUERY_SYSTEM_CALL;
	r.r1=get_number();
	RESET_CAPABILITY(get_kernel_parameter()->
		get_thread_attribute.current_domain_capability);
	call_kernel(&r);
	print("\nresult is",r.system_call);
	r.system_call=get_kernel_parameter()->
		get_thread_attribute.thread_data.state;
	if(r.system_call==THREAD_SLEEP_STATE)
		print("\nsleep state, semaphore id is",
			get_kernel_parameter()->
			get_thread_attribute.thread_data.
				sleep_semaphore_id);
	else if(r.system_call==THREAD_RUN_STATE)
		print_string("\nrun state");
	else if(r.system_call==THREAD_READY_STATE)
		print_string("\nready state");
	else
		print("\nstate is ",get_kernel_parameter()->
			get_thread_attribute.thread_data.state);
	print("\npriority is ",get_kernel_parameter()->
		get_thread_attribute.thread_data.priority);
	print("\nthread semaphore is ",get_kernel_parameter()->
		get_thread_attribute.thread_data.semaphore_id);
	print("\ncurrent process id is ",get_kernel_parameter()->
		get_thread_attribute.return_stack_data.
			current_domain_id);
	print("\ncurrent semaphore id is ",get_kernel_parameter()->
		get_thread_attribute.return_stack_data.
			user_semaphore_id);
	print("\ncurrent pointer is ",(int)(get_kernel_parameter()->
		get_thread_attribute.return_stack_data.
			environment.point.ip));
	print("\ncurrent memory body is ",(int)(get_kernel_parameter()->
		get_thread_attribute.return_stack_data.
			current_memory_body_id));
	return;
}
static void display_semaphore()
{
	int id;
	call_kernel_register r;
	print_string("\ninput semaphore id");
	r.system_call=SEMAPHORE_INFORMATION_SYSTEM_CALL;
	r.r1=get_number();
	RESET_CAPABILITY(get_kernel_parameter()->
		capability.capability_1)
	call_kernel(&r);
	print("\nresult is",r.system_call);
	print("\nsemaphore value is",get_kernel_parameter()->
		semaphore_attribute.value);
	print("\nsemaphore entity type id is",get_kernel_parameter()->
		semaphore_attribute.entity_type_id);
	print("\nsemaphore entity id is",get_kernel_parameter()->
		semaphore_attribute.entity_id);
 	id=get_kernel_parameter()->semaphore_attribute.entity_type_id;

	if(id==SEMAPHORE_PROCESS_ENTITY_TYPE)
		print("\nSEMAPHORE_PROCESS_ENTITY_TYPE",id);
	else if(id==SEMAPHORE_THREAD_ENTITY_TYPE)
		print("\nSEMAPHORE_THREAD_ENTITY_TYPE",id);
	else if(id==SEMAPHORE_RETURN_STACK_ENTITY_TYPE)
		print("\nSEMAPHORE_RETURN_STACK_ENTITY_TYPE",id);
	else if(id==SEMAPHORE_MEMORY_ENTITY_TYPE)
		print("\nSEMAPHORE_MEMORY_ENTITY_TYPE",id);
	else if(id==SEMAPHORE_DEVICE_ENTITY_TYPE)
		print("\nSEMAPHORE_DEVICE_ENTITY_TYPE",id);
	else
		print("\nUNKNOWN_ENTITY_TYPE",id);
	return;
}
static void wakeup_file_window()
{
	typeof(get_memory_parameter()->wakeup_file_window) *par;
	par=&(get_memory_parameter()->wakeup_file_window);
	print_string("\ninput file window id");
	par->file_window_id=get_number();
	print_string("\ninput no wait flag[>=0?TRUE:FALSE]");
	par->no_wait_flag=get_number()>=0?TRUE:FALSE;
	par->max_number=-1;
	RESET_CAPABILITY(par->file_capability);
	print("\nresult is ",call_memory(MEMORY_BODY_ID,
				WAKEUP_FILE_WINDOW_MEMORY_CALL));
	return;
}
static void wakeup_memory_domain()
{
	typeof(get_memory_parameter()->wakeup_memory_domain) *par;
	par=&(get_memory_parameter()->wakeup_memory_domain);
	print_string("\ninput memory domain id");
	par->domain_id=get_number();
	print_string("\ninput no wait flag[>=0?TRUE:FALSE]");
	par->no_wait_flag=((get_number()>=0)?TRUE:FALSE);
	par->max_number=-1;
	par->max_file_number=-1;
	RESET_CAPABILITY(par->domain_capability);
	print("\nresult is ",call_memory(MEMORY_BODY_ID,
		WAKEUP_MEMORY_DOMAIN_MEMORY_CALL));
	return;
}
void my_test()
{
	int thread_id,x;
	for(thread_id=-1;;){
		x=get_number();
		print_string("\n\n");
		set_print_thread(1);
		if(x==0){
			print_string("\ninput new time length :");
			x=get_number();
			set_timer_length(x);
		}else if(x==1){
			print_string("\ninput new thread id :");
			thread_id=get_number();
		}else if(x==2){
			display_memory_domain();
			thread_id=1;
		}else if(x==3){
			display_thread();
			thread_id=1;
		}else if(x==4){
			display_semaphore();
			thread_id=1;
		}else if(x==5){
			wakeup_file_window();
			thread_id=1;
		}else if(x==6){
			wakeup_memory_domain();
			thread_id=1;
		}else
			print_string("\nNo such command");
		print_string("\n");
		set_print_thread(thread_id);
	}
}
