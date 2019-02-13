#include "../kernel/system_call_code.h" 
.global test_file_system,copy_cs_data_routine 
test_file_system: 
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax
	movl %esp,%eax
	pushl %eax
	call file_system_call_deal
	popl %eax
	popl %eax
	popl %ebx
	popl %ecx
	popl %edx
	popl %esi
	popl %edi
	popl %ebp 
1:
	movl $(THREAD_RETURN_SYSTEM_CALL),%eax
	movl $(-1),%ebx
	int $(0x70)
	jmp 1b 
