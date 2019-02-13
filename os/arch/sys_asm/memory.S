.global service_call_routine,call_device_driver
service_call_routine:
	pushl %gs
	pushl %fs
	pushl %es
	pushl %ds
	pushl %ecx
	pushl %ebx
	pushl %eax 
	movl %ss,%eax
	movl %eax,%ds
	movl %eax,%es
	movl %eax,%fs
	movl %eax,%gs 
	call service_routine 
	popl %ebx
	popl %ebx 
	popl %ecx 
	popl %ds
	popl %es
	popl %fs
	popl %gs 
	iret
call_device_driver:
	pushl %ebp
	movl %esp,%ebp
	movl 0x0c(%ebp),%eax
	andl $(0xfffffff0),%eax
	subl $(0x10),%eax
	movl %eax,%esp 
	pushfl
	sti
	pushl %ebp 
	pushl %edi 
	pushl %esi 
	pushl %edx 
	pushl %ecx 
	pushl %ebx

	pushl 0x10(%ebp)
	pushl 0x0c(%ebp)
	call *0x08(%ebp)
	popl %ebx
	popl %ebx

	popl %ebx
	popl %ecx
	popl %edx
	popl %esi
	popl %edi
	popl %ebp
	popfl
	movl %ebp,%esp
	popl %ebp
	ret
