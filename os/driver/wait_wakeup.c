#include "../kernel/kernel.h"
#include "../include/os.h" 
void memory_wakeup(int processor,int semaphore)
{
	call_kernel_register r;
	union system_call_parameter *par;
	CAPABILITY c;

	if((processor<0)||(semaphore<0))
		return;
	par=get_kernel_parameter();
	COPY_CAPABILITY((par->capability.capability_1),c);
	COPY_CAPABILITY((os->system_capability),
		(par->capability.capability_1));
	r.system_call=SEMAPHORE_OPERATION_SYSTEM_CALL;
	r.r1=semaphore;
	r.r2=1;
	r.r3=0;
	r.r6=FALSE;
	call_kernel(&r);
	COPY_CAPABILITY(c,(par->capability.capability_1));
	return;
}
void memory_wait(int sleep_semaphore,
	int next_processor,int next_semaphore)
{
	call_kernel_register r;
	union system_call_parameter *par;
	CAPABILITY c1,c2;

	par=get_kernel_parameter();
	COPY_CAPABILITY((par->capability.capability_1),c1);
	COPY_CAPABILITY((par->capability.capability_2),c2);

	COPY_CAPABILITY((os->system_capability),
		(par->capability.capability_1));
	COPY_CAPABILITY((os->system_capability),
		(par->capability.capability_2));
	r.system_call=SEMAPHORE_OPERATION_SYSTEM_CALL;
	r.r1=0;

	r.r3=sleep_semaphore;
	r.r4=TRUE;
	r.r5=FALSE;
	r.r6=TRUE;

	call_kernel(&r);

	if((next_processor>=0)&&(next_semaphore>0)){
		r.system_call=SEMAPHORE_OPERATION_SYSTEM_CALL;
		COPY_CAPABILITY((os->system_capability),
			(par->capability.capability_1));
		COPY_CAPABILITY((os->system_capability),
			(par->capability.capability_2));
		r.r1=next_semaphore;
		r.r2=(-1);
		r.r3=0;
		r.r6=FALSE;

		call_kernel(&r);
	}
	COPY_CAPABILITY(c1,(par->capability.capability_1));
	COPY_CAPABILITY(c2,(par->capability.capability_2));
	return;
}
