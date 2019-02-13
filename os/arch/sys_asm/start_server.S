#include "../../kernel/system_call_code.h"
.global start_server
start_server:
	pushl %gs
	pushl %fs
	pushl %es
	pushl %ds
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax

	movl %ss,%eax
	movl %eax,%ds
	movl %eax,%es
	movl %eax,%fs
	movl %eax,%gs

	movl %esp,%eax
	pushl %eax
	call start_server_routine
1:
	movl $(THREAD_RETURN_SYSTEM_CALL),%eax
	movl $(0x00),%ebx
	int $(0x70)
	jmp 1b
