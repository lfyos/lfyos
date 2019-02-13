#include"../include/os.h"
#include"../platform/include/service.h" 
extern void test_file_system(void ); 
static int init_file_system_routine(int file_system_id)
{
	call_kernel_register r;
	union system_call_parameter *par;

	print("\n                ",file_system_id);

	par=get_kernel_parameter(); 

	RESET_CAPABILITY(par->domain_attribute.capability);
	RESET_CAPABILITY(par->domain_attribute.domain_data.capability);
	RESET_CAPABILITY(par->domain_attribute.domain_data.
		thread_create_capability);
	r.system_call=RESOURCE_QUERY_SYSTEM_CALL;
	r.r1=file_system_id;
	call_kernel(&r);
	print("query file system processor resource,result is ",
		r.system_call);

	par->domain_attribute.domain_data.file[0].memory_body_id
		=MEMORY_BODY_ID;
	par->domain_attribute.domain_data.file[0].file_window_id=0;
	par->domain_attribute.domain_data.file[0].map_length=512;
	par->domain_attribute.domain_data.file[0].right=READ_ONLY_RIGHT;
	RESET_CAPABILITY(par->domain_attribute.
		domain_data.file[0].file_capability);
	par->domain_attribute.domain_data.file[1].memory_body_id
		=MEMORY_BODY_ID;
	par->domain_attribute.domain_data.file[1].file_window_id=0;
	par->domain_attribute.domain_data.file[1].map_length=512;
	par->domain_attribute.domain_data.file[1].right=READ_WRITE_RIGHT;
	RESET_CAPABILITY(par->domain_attribute.
		domain_data.file[1].file_capability);

	par->domain_attribute.domain_data.max_thread_number=OS_THREAD_NUMBER;
	par->domain_attribute.domain_data.max_semaphore_number
		=OS_SEMAPHORE_NUMBER;
	par->domain_attribute.domain_data.max_return_stack_number
		=OS_RETURN_STACK_NUMBER;
	par->domain_attribute.domain_data.cpu_set_id
		=file_system_id%OS_CPU_SET_NUMBER;

	RESET_CAPABILITY(par->domain_attribute.capability);
	RESET_CAPABILITY(par->domain_attribute.domain_data.capability);
	RESET_CAPABILITY(par->domain_attribute.
		domain_data.thread_create_capability);

	r.r1=file_system_id;

	par->domain_attribute.domain_data.start_point.ip=test_file_system;
	par->domain_attribute.domain_data.start_point.cs
		=(file_system_id==1)?KERNEL_CS:DEFAULT_CS; 
	par->domain_attribute.domain_data.start_point.ds
		=(file_system_id==1)?KERNEL_DS:DEFAULT_DS; 
	par->domain_attribute.domain_data.start_point.ss 
		=(file_system_id==1)?KERNEL_DS:DEFAULT_DS; 
	par->domain_attribute.domain_data.start_point.es 
		=(file_system_id==1)?KERNEL_DS:DEFAULT_DS; 
	par->domain_attribute.domain_data.start_point.sp
	=(file_system_id==1)?(char *)KERNEL_SP:(char *)DEFAULT_SP; 
	par->domain_attribute.domain_data.start_point.sp_0
	=(file_system_id==1)?(char *)KERNEL_SP_0:(char *)DEFAULT_SP_0;

	r.system_call=RESOURCE_ALLOCATION_SYSTEM_CALL;
	call_kernel(&r);
	print("\n                ",file_system_id);
	print("allocate processor resource for file system,result is ",
		r.system_call);
	print_string("\n\n");

	return (par->domain_attribute.domain_data.semaphore);
} 
void init_file_system(int sem_id[])
{
	sem_id[0]=init_file_system_routine(1);
	sem_id[1]=init_file_system_routine(2);
	sem_id[2]=init_file_system_routine(3);
}
