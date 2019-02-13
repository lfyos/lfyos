.global system_call_routine,timer_interruption_routine
system_call_routine:
	call save_parameter
	pushl $(0x00)
	pushl $(system_call_process) 
	call restore_parameter		/*never return */ 
timer_interruption_routine: 
	call save_parameter
	pushl $(0x01)
	pushl $(time_call_process)
	call restore_parameter		/*never return */
