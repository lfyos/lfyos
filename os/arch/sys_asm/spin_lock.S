.global operate_spinlock
operate_spinlock:
	pushl %ebx
	pushl %edi
	movl 0x0c(%esp),%edi
	movl 0x10(%esp),%ebx

	pushfl
	cli
	movl $(1024*1024*1024),%eax
1:
	xchgl (%edi),%eax
	cmpl $(1024*1024*1024),%eax
	je 1b
	addl %eax,%ebx
	xchgl %ebx,(%edi)

	popfl
	popl %edi
	popl %ebx
	ret
