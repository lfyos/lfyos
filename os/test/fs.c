#include"../include/os.h"
#include"../platform/include/service.h"
extern void test(),__end_rodata(),lfyos_exit(),_etext();
int do_exit()
{
	call_kernel_register r;
	struct kernel_time time;
	int id,length=((int)(__end_rodata))&(0x1fffffff);

	get_current_time(&time);

	r.system_call=(-1);
	call_kernel(&r);
	id=r.system_call+r.r1+r.r2+r.r3+r.r4+r.r5+time.v_l;
	r.r1=10000; 
	id=(id%(r.r1)+r.r1+r.r1)%(r.r1);

	RESET_CAPABILITY(
		get_kernel_parameter()->capability.capability_1);
	r.system_call=CREATE_THREAD_SYSTEM_CALL;
	r.r1=2;
	asm volatile( "movl %%cs,%%eax\n\t" :"=a" (r.r5)); 
	r.r5=(r.r5)&0x03;
	if(r.r5==3)
		r.r5=(int)DEFAULT_SP;
	else
		r.r5=(int)KERNEL_SP;
	if(id==0){
		r.r4 =(0x1fffffff)&((int)(test));
		for(id=0;id<55;id++)
			r.r4+=length; 
		r.r4 =(int)test;
		call_kernel(&r);
		print("\ncreate thread with id is ",r.system_call);
	}else{
		r.r4=(int)lfyos_exit;
		call_kernel(&r);
	} 
	for(;;){
		r.system_call=THREAD_RETURN_SYSTEM_CALL;
		call_kernel(&r);
	}
	return 0;
}
void delay_routine(call_kernel_register *r)
{
	union system_call_parameter *par;

	par=get_kernel_parameter();

	r->system_call=(-1);
	call_kernel(r);

	r->system_call=(-1);
	call_kernel(r);

	r->system_call=SEMAPHORE_TIME_SYSTEM_CALL; 
	r->r2=TRUE;
	r->r3=TRUE;
	r->r4=SEMAPHORE_V_OPERATION_TIMER_ACTION;
	r->r5=(-1);

	get_current_time((struct kernel_time *)
		&(par->set_semaphore_time.first_time));
	TIME_INC(1830*4,(par->set_semaphore_time.first_time));
	SET_MOST_TIME(par->set_semaphore_time.step_time);
	RESET_CAPABILITY(par->set_semaphore_time.
		old_semaphore_capability);
	RESET_CAPABILITY(par->set_semaphore_time.
		new_semaphore_capability);

	call_kernel(r);

	return;
}
void file_system_call_deal(call_kernel_register * env)
{
	int *p,*p1,fd,begin_address,rw_length;
	int length=((int)(__end_rodata))&(0x1fffffff);
	struct file_system_call_parameter *par;
	struct file_read_parameter *read_par;
	struct kernel_time time;

	par=get_file_system_parameter(); 
	switch(env->system_call){
	case PROCESS_TIME_OUT_REQUEST:
		RESET_CAPABILITY(get_kernel_parameter()
			->capability.capability_1);
		env->system_call=CREATE_THREAD_SYSTEM_CALL;
		env->r1=2;
		env->r4=(int)(do_exit);
		asm volatile( "movl %%cs,%%eax\n\t" :"=a" (env->r5)); 
		env->r5=(env->r5)&0x03;
		if(env->r5==3)
			env->r5=(int)DEFAULT_SP;
		else
			env->r5=(int)KERNEL_SP;
		call_kernel(env);
		env->r1=1;
		break;
	case SIGNAL_PROCESS_REQUEST:
		env->system_call=THREAD_CALL_SYSTEM_CALL;
		env->r1=0;
		env->r2=(int)delay_routine;
		env->r3=(int)env;
		call_kernel(env);
		env->r1=1;
		break;
	case FILE_OPERATION_REQUEST:
		fd=par->kernel_file_window.file_window_id;
		begin_address=par->begin_address;
		rw_length=par->data_length;
		env->r1=1;
		switch(par->command){
		case READ_FILE:
			env->system_call=THREAD_CALL_SYSTEM_CALL;
			env->r1=0;
			env->r2=(int)delay_routine;
			env->r3=(int)env;
			call_kernel(env);
			get_current_time(&time);
			get_kernel_parameter()->set_thread_attribute.
				priority=0;
			get_kernel_parameter()->set_thread_attribute.
				cpu_set_id=(time.v_l)%OS_CPU_SET_NUMBER;
			get_kernel_parameter()->set_thread_attribute.
				signal_bitmap=0x87654321;
			get_kernel_parameter()->set_thread_attribute.
				mandatory_exit_number=0;
			env->system_call=THREAD_SET_SYSTEM_CALL;
			env->r1=(-1);
			call_kernel(env);

			RESET_CAPABILITY(get_kernel_parameter()
				->capability.capability_1);
			env->system_call=SEND_SIGNAL_SYSTEM_CALL;
			env->r1=(-1);
			env->r2=0x12345678;
			call_kernel(env);
			read_par=(struct file_read_parameter *)
				get_thread_physical_block();
			read_par->block_flag=DEFAULT_MEMORY_BLOCK_FLAG;
			read_par->parent_file_window_id=fd-1;
			read_par->begin_address=(begin_address>=length)
				?(begin_address-length):begin_address;
			RESET_CAPABILITY(read_par->
				parent_file_window_capability);
			begin_address%=length;
			begin_address+=length+length;
			begin_address%=length;
			if(begin_address<length)
				p=(int *)(begin_address|(0xc0000000));
			else if(begin_address<5*length)
				p=(int *)(begin_address-length);
			else{
				begin_address%=length;
				begin_address+=4*length; 
				p=(int *)begin_address; 
			}
			if((fd%3)!=0)
				env->r1=(-1);
			else{
				env->r1=1;
				p1=(int *)get_thread_physical_block();
				rw_length=rw_length/sizeof(int);
				for(fd=0;fd<rw_length;fd++,p++,p1++){
					asm volatile(
						"pushl %%cs:(%%esi)\n\t"
						"popl (%%edi)\n\t"
						::"S"(p),"D"(p1)
					);
				}
			}
			break;
		case WRITE_FILE:
		default:
			env->r1=1;
			break;
		}
		break;
	default:
		env->r1=1;
		break;
	}
	par->reply_result=PAGE_SIZE;
	par->data_length=PAGE_SIZE;
	env->system_call=THREAD_RETURN_SYSTEM_CALL;
	call_kernel(env);
	return ;
}
