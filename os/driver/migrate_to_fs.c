#include "../kernel/kernel.h"
#include "../include/os.h" 
#define FILE_SYSTEM_ERROR	(-1) 
int migrate_to_file_system(int processor_process_id,
	struct file_system_call_parameter *file_system_par,
	struct thread_physical_block *pb)
{
	call_kernel_register r;
	struct thread_physical_block bak_pb;
	union system_call_parameter *par;

	remote_unlock_memory_body(current->rt->current_memory_body_id);
	par=get_kernel_parameter();
	COPY_THREAD_PHYSICAL_BLOCK((par->thread_physical_block),bak_pb);
	COPY_THREAD_PHYSICAL_BLOCK((*pb),(par->thread_physical_block));
	r.system_call=WITH_BLOCK_THREAD_CALL_SYSTEM_CALL;
	r.r1=processor_process_id;
	
	call_kernel(&r);

	COPY_THREAD_PHYSICAL_BLOCK(bak_pb,(par->thread_physical_block));
	remote_lock_memory_body(current->rt->current_memory_body_id);

	return ((r.system_call==1)?(r.r1):FILE_SYSTEM_ERROR);
}
