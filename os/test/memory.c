#include"../driver/driver.h"
#include"../include/os.h"
#include"../arch/sys_c/par.h"
#include"../platform/include/service.h"

static int block_hash_function(int file_window,int logic_block)
{
	int i;

	i=memory_body->head->file_number;
	i=i*file_window;
	i=i+logic_block;
	return i;
} 
static int file_hash_function(struct kernel_file_window *f)
{
	int i;

	i=memory_body->head->file_number;
	i=i*(f->file.server_id);
	i=i+(f->file.file_id);
	return i;
}

void  init_memory()
{
	int i,bn,n=256*1024,ret_val;
	union memory_call_parameter *par;
	struct memory_domain_struct *p;

	par=get_memory_parameter();

	par->memory_body_parameter.block_hash_function
		=block_hash_function;
	par->memory_body_parameter.file_hash_function
		=file_hash_function;
	par->memory_body_parameter.my_processor=0;
	par->memory_body_parameter.my_memory_body=MEMORY_BODY_ID;

	par->memory_body_parameter.block_number=n;
	par->memory_body_parameter.file_number=FILE_NUMBER;
	par->memory_body_parameter.memory_domain_number=16;
	par->memory_body_parameter.hash_number=100000;

	RESET_CAPABILITY(par->memory_body_parameter.capability);

	bn=call_memory(MEMORY_BODY_ID,INIT_MEMORY_BODY);

print("\n              init memory finish, free block number is:",bn);

	bn=bn/16;
	n=n/16;

for(i=0;i<16;i++){
	par=get_memory_parameter();
	par->memory_resource.domain_id=i;
	p=&(par->memory_resource.domain_data);
	RESET_CAPABILITY(p->capability);
	ret_val=call_memory(MEMORY_BODY_ID,QUERY_PROCESS_MEMORY_CALL);

print("\n              query memory process' memory resource:",ret_val);

	par=get_memory_parameter();
	par->memory_resource.domain_id=i;
	p=&(par->memory_resource.domain_data);
	RESET_CAPABILITY(p->capability);

	p->flag=0;

	p->max_file_number=FILE_NUMBER;
	p->trigger_close_file_number=FILE_NUMBER;
	p->max_block_number=n; 
	p->physical_max_block_number=bn;

	p->trigger.write_block_number=n-500;
	p->trigger.free_block_number=n-400;
	p->trigger.system_write_block_number=1000;
	p->trigger.system_free_block_number=800;

	p->physical_trigger.write_block_number=bn-500;
	p->physical_trigger.free_block_number=bn-400;
	p->physical_trigger.system_write_block_number=1000;
	p->physical_trigger.system_free_block_number=800;

	RESET_CAPABILITY(p->capability);
	ret_val=call_memory(MEMORY_BODY_ID,SET_PROCESS_MEMORY_CALL);

print("\n              allocate memory process' memory resource:",ret_val);
}
	return; 
}
