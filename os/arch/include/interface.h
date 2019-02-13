#ifndef OS_ARCHITECTURE_INTERFACE
#define OS_ARCHITECTURE_INTERFACE

#define get_kernel_parameter()		\
	(((union system_call_parameter *)PARAMETER_POINTER)-1)
#define get_memory_parameter()		\
	(((union memory_call_parameter *)PARAMETER_POINTER)-1)
#define get_file_system_parameter() 				\
	(((struct file_system_call_parameter *)PARAMETER_POINTER)-1)
#define get_thread_physical_block() ((char*)PARAMETER_POINTER)
static inline int call_kernel_service(int service,int cmd,int arg)
{
	volatile int eax=service,ebx=cmd,ecx=arg;
	asm volatile(
		"pushl %%ebp\n\t"
		"int $(0x71)\n\t"
		"popl %%ebp\n\t"
		:"+a"(eax):"b"(ebx),"c"(ecx):"%edx","%esi","%edi"
	);
	return eax;
}
typedef volatile struct general_register call_kernel_register;
static inline void call_kernel(call_kernel_register *r)
{
	switch(r->system_call){
	case CREATE_THREAD_SYSTEM_CALL:
		asm volatile(
			"pushl %%ebp\n\t"
			"int $(0x70)\n\t"
			"cmpl $(0x00),%%eax\n\t"
			"jl 2f\n\t"
			"cmpl $(0x00),%%ebx\n\t"
			"je 2f\n\t"
			"pushl %%ecx\n\t"
			"movl %%edi,%%esp\n\t"
			"call *%%esi\n\t"
			"xorl %%ebx,%%ebx\n\t"
		"1:\n\t"
			"popl %%eax\n\t"
			"pushl %%eax\n\t"
			"int $(0x70)\n\t"
			"movl $(-1),%%ebx\n\t"
			"jmp 1b\n\t"
		"2:\n\t"
			"popl %%ebp\n\t"
			:"+a"(r->system_call)
			:"b"(r->r1),"c"(THREAD_RETURN_SYSTEM_CALL),
			 "S"(r->r4),"D"(r->r5)
			:"%edx"
		);
		return ;
	case THREAD_CALL_SYSTEM_CALL:
		if(r->r1!=0)
			break;
		asm volatile(
			"pushl %%ebp\n\t"
			"int $(0x70)\n\t"
			"cmpl $(0x00),%%eax\n\t"
			"jne 2f\n\t" 
			"pushl %%esi\n\t"
			"pushl %%edx\n\t"
			"call *%%ecx\n\t"
			"popl %%edx\n\t" 
			"xorl %%ebx,%%ebx\n\t"
		"1:\n\t"
			"popl  %%eax\n\t"
			"pushl %%eax\n\t"
			"int $(0x70)\n\t"
			"movl $(-1),%%ebx\n\t"
			"jmp 1b\n\t"
		"2:\n\t"
			"popl %%ebp\n\t"
			:"+a"(r->system_call)
			:"b"(r->r1),"c"(r->r2),"d"(r->r3),
			 "S"(THREAD_RETURN_SYSTEM_CALL),"D"(0)
		);
		return ;
	default:
		break;
	}
	asm volatile(
		"pushl %%ebp\n\t"
		"movl 0x18(%%eax),%%ebp\n\t"
		"movl 0x00(%%eax),%%eax\n\t"
		"int $(0x70)\n\t"
		"popl %%ebp\n\t" 
		:"=a"(r->system_call),"+b"(r->r1),"+c"(r->r2),
		 "+d"(r->r3),"+S"(r->r4),"+D"(r->r5):"a"(r)
	);
	return;
} 
#endif
