#include "driver.h"
#include "../include/os.h"
static void local_unlock_device_and_lock_memory(
	int memory_body_id,int device_id)
{
	int *pointer;
	call_kernel_register r;
	union system_call_parameter *par;
	CAPABILITY c1,c2;

	r.r1=0;
	if(device_id>=0){
		pointer=&(driver[device_id].device_spin_lock);
		if(operate_spinlock(pointer,1)<0)
			r.r1=driver[device_id].device_semaphore_id;
	}
	r.r3=0;
	if(memory_body_id>=0){
		pointer=&(driver[memory_body_id].memory_spin_lock);
		if(operate_spinlock(pointer,-1)<=0)
			r.r3=driver[memory_body_id].memory_semaphore_id;
	}
	if((r.r1==0)&&(r.r3==0))
		return;
	r.system_call=SEMAPHORE_OPERATION_SYSTEM_CALL;
	r.r2=1;
	r.r4=TRUE;
	r.r5=FALSE;
	r.r6=FALSE;

	par=get_kernel_parameter();
	COPY_CAPABILITY((par->capability.capability_1),c1);
	COPY_CAPABILITY((par->capability.capability_2),c2);
	COPY_CAPABILITY((os->system_capability),
		(par->capability.capability_1));
	COPY_CAPABILITY((os->system_capability),
		(par->capability.capability_2));
	call_kernel(&r); 
	COPY_CAPABILITY(c1,(par->capability.capability_1));
	COPY_CAPABILITY(c2,(par->capability.capability_2));
	return;
}
static void local_unlock_memory_and_lock_device(
	int memory_body_id,int device_id)
{
	int *pointer;
	call_kernel_register r;
	union system_call_parameter *par;
	CAPABILITY c1,c2;

	r.r1=0;
	if(memory_body_id>=0){
		pointer=&(driver[memory_body_id].memory_spin_lock);
		if(operate_spinlock(pointer,1)<0)
			r.r1=driver[memory_body_id].memory_semaphore_id;
	}
	r.r3=0;
	if(device_id>=0){
		pointer=&(driver[device_id].device_spin_lock);
		if(operate_spinlock(pointer,-1)<=0)
			r.r3=driver[device_id].device_semaphore_id;
	}
	if((r.r1==0)&&(r.r3==0))
		return;
	par=get_kernel_parameter();
	COPY_CAPABILITY((par->capability.capability_1),c1);
	COPY_CAPABILITY((par->capability.capability_2),c2);
	COPY_CAPABILITY((os->system_capability),
		(par->capability.capability_1));
	COPY_CAPABILITY((os->system_capability),
		(par->capability.capability_2));
	r.system_call=SEMAPHORE_OPERATION_SYSTEM_CALL;
	r.r2=1;
	r.r4=TRUE;
	r.r5=FALSE;
	r.r6=FALSE;

	call_kernel(&r);

	COPY_CAPABILITY(c1,(par->capability.capability_1));
	COPY_CAPABILITY(c2,(par->capability.capability_2));
	return;
}
void unlock_device_and_lock_memory(
	int memory_body_id,int device_id)
{
	local_unlock_device_and_lock_memory(memory_body_id,device_id);
	remote_lock_memory_body(memory_body_id);
	current->rt->current_memory_body_id=memory_body_id;
	return;
}
int unlock_memory_and_lock_device(int device_id)
{
	int memory_body_id;
	memory_body_id=current->rt->current_memory_body_id;
	current->rt->current_memory_body_id=-1;
	remote_unlock_memory_body(memory_body_id);
	local_unlock_memory_and_lock_device(memory_body_id,device_id);
	return memory_body_id;
}
