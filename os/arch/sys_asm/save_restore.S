.global save_parameter,restore_parameter 
thread_register_context:
	.long 0 
idle_restore:
	sti
	hlt
	cli
	pushl $(0x00)
	pushl $(0x00)
	pushl $(idle_restore)
restore_parameter:
	popl %ebx
	popl %ebx
	popl %eax
	lss kernel_stack_descr,%esp
	pushl %eax
	pushl %ebx
	call system_call
	movl %eax,thread_register_context
	cmpl $(0x00),%eax
	je idle_restore

	movl  0x44(%eax),%esp

	movl  0x38(%eax),%ebx
	movl  0x3c(%eax),%ecx
	movl  0x40(%eax),%edx
	movl  %ebx,%es			/*restore es*/
	movl  %ecx,%fs			/*restore fs*/
	movl  %edx,%gs			/*restore gs*/

	movl  0x04(%eax),%ebx		/*restore ebx */
	movl  0x08(%eax),%ecx		/*restore ecx */
	movl  0x0c(%eax),%edx		/*restore edx */
	movl  0x10(%eax),%esi		/*restore esi */
	movl  0x14(%eax),%edi		/*restore edi */
	movl  0x18(%eax),%ebp		/*restore ebp */
					/*NO restore R7*/ 
	pushl 0x30(%eax)		/*restore ss */
	pushl 0x2c(%eax)		/*restore esp */
	pushl 0x28(%eax)		/*restore eflag*/
	pushl 0x24(%eax)		/*restore  cs */
	pushl 0x20(%eax)		/*restore eip */ 

	pushl 0x00(%eax)
	pushl 0x34(%eax)
	popl  %ds			/*restore ds*/
	popl  %eax			/*restore eax*/ 
	iret				/* return to application */

save_parameter:	/*save thread parameter	when enter os kernel*/
	pushl %eax 			/*push eax,ds into stack */
	pushl %ds
	
	movl %ss,%eax			/*set segment register */
	movl %eax,%ds

	movl thread_register_context,%eax
					/*compute where to save */ 
	cmpl $(0x00),%eax
	je no_active_thread

	popl  0x34(%eax)		/* save ds from stack*/
	pushl %es
	popl  0x38(%eax)		/* save es */
	pushl %fs
	popl  0x3c(%eax)		/* save fs */
	pushl %gs
	popl  0x40(%eax)		/* save gs */

	popl 0x00(%eax)			/*save eax from stack*/
	movl %ebx,0x04(%eax)		/*save ebx */
	movl %ecx,0x08(%eax)		/*save ecx */
	movl %edx,0x0c(%eax)		/*save edx */
	movl %esi,0x10(%eax)		/*save esi */
	movl %edi,0x14(%eax)		/*save edi */
	movl %ebp,0x18(%eax)		/*save ebp */

	popl %ebx			/*pop return address to ebx*/

	popl 0x20(%eax)			/*save eip */
	popl 0x24(%eax)			/*save  cs */
	popl 0x28(%eax)			/*save eflag*/
	popl 0x2c(%eax)			/*save esp */
	popl 0x30(%eax)			/*save ess */

	movl %esp,0x44(%eax)		/*save sp_0 */
exit_save:
	lss kernel_stack_descr,%esp
	pushl %ebx		/*pushl return address(in ebx) */
	movl %ds,%eax
	movl %eax,%es
	movl %eax,%fs
	movl %eax,%gs
	ret			/*return to caller */
no_active_thread:
	popl %eax
	popl %eax
	popl %ebx		/*popl return address to ebx */
	jmp exit_save 
