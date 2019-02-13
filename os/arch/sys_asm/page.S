.global page_fault_interruption
page_fault_interruption:
	pushl %eax
	pushl %ebx
	pushl %ecx
	pushl %edx
	pushl %esi
	pushl %edi
	pushl %ebp 
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

	movl %ss,%eax
	movl %eax,%ds
	movl %eax,%es
	movl %eax,%fs
	movl %eax,%gs 
	movl 0x2c(%esp),%eax
	pushl %eax
	movl %cr2,%eax
	pushl %eax
	pushl $(0x00)
	call page_fault_process
	cmpl $(0x00),%eax
	je lfyos_exit

	popl %eax
	popl %eax
	popl %eax

	popl %gs
	popl %fs
	popl %es
	popl %ds 
	popl %ebp
	popl %edi
	popl %esi
	popl %edx
	popl %ecx
	popl %ebx
	popl %eax
	addl $(0x04),%esp 
	iret
