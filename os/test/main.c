#include"../include/os.h"
#include"../arch/sys_c/par.h"
#include"../platform/include/service.h"

int jiffies_64=0;
extern void test(void),my_test();
void test_main(void )
{
	int i,j,sem_id[3];
	call_kernel_register r;
	struct kernel_time t2,t3;
	extern void init_file_system(int sem_id[]);
	extern int open_file(int fd),init_memory();
	extern void register_file(int fd);
	union system_call_parameter *par=get_kernel_parameter();
	print_string("\n\n                              Congratulations\n\n");
	print_string("\n\n     Your SASOS operating System has been installed and bootted successfully!");

	print_string("\n\n     Next, I am going to initialize processor set, push any key to continue ...\n");
	get_char();
	print_string("\n\n     Next, I am going to initialize the file system processor process, push any key to continue ...\n");
	get_char();
	init_file_system(sem_id);
	print_string("\n\n     File system processor process has been initialized.Next,I am going to initialize the memory management. push any key to continue ...\n");
	get_char();

	init_memory();

	print_string("\n\n     Memory management and file system processor process have all been initialized.Next I am going to open a file,after that I will create some threads to run on the opened file,thus implement Programms Running on Files. Push any key to begin registering file......\n");
	get_char();
	
	for(i=0;i<FILE_NUMBER;i++)
		register_file(i);
	print_string("\n\nPush any key to begin openning file......\n");
	get_char();
	for(i=0;i<FILE_NUMBER;i++)
		open_file(i);
	i=open_file(0);
	
	RESET_CAPABILITY(
		get_kernel_parameter()->capability.capability_1);
	RESET_CAPABILITY(
		get_kernel_parameter()->capability.capability_2);
	r.system_call=THREAD_FILE_SYSTEM_CALL;
	r.r1=0;
	r.r2=512;
	r.r3=READ_ONLY_RIGHT;
	r.r3=READ_WRITE_RIGHT;
	r.r4=MEMORY_BODY_ID;
	r.r5=i;
	call_kernel(&r);

print_string("\n\n\n\n     Both file system processor process and memory management have been initialized,also files have been opened. Next I am going to create some threads to run on these opened files. When these threads are running, the programm that it is running,the code that it is executing, and the data that it is accessing, all these information are in opened files,Push any key to continue .....");
	get_char();
	for(i=0;i<OS_CPU_NUMBER;i++){
		r.system_call=CPU_SET_SYSTEM_CALL;
		r.r1=i;
		r.r2=i%OS_CPU_SET_NUMBER;
		RESET_CAPABILITY(get_kernel_parameter()
			->capability.capability_1);
		call_kernel(&r);
		r.system_call=(-1);
		call_kernel(&r);
	}

	RESET_CAPABILITY(par->capability.capability_1);
	r.system_call=CREATE_THREAD_SYSTEM_CALL; 
	r.r1=1;
	r.r4=(int)my_test;
	r.r5=(int)DEFAULT_SP;
	call_kernel(&r); 
	for(i=0;i<3;i++){
		r.system_call=SEMAPHORE_TIME_SYSTEM_CALL;
		r.r1=(sem_id[i]<0)?sem_id[i]:(0-sem_id[i]);
		r.r2=FALSE;
		r.r3=FALSE;
		r.r4=SEMAPHORE_PROCESS_TIMER_ACTION
			|SEMAPHORE_V_OPERATION_TIMER_ACTION;
		r.r5=(-1);
		get_current_time(&(get_kernel_parameter()
			->set_semaphore_time.first_time));

		SET_MINIMAL_STEP(get_kernel_parameter()
			->set_semaphore_time.step_time);
		for(j=0;j<50;j++)
			TIME_INC(1830*1000,get_kernel_parameter()
				->set_semaphore_time.step_time);

		RESET_CAPABILITY(get_kernel_parameter()
			->set_semaphore_time.old_semaphore_capability);
		RESET_CAPABILITY(get_kernel_parameter()
			->set_semaphore_time.new_semaphore_capability);

		call_kernel(&r);
	} 
	for(;;){
		union memory_call_parameter *m_par;
		m_par=get_memory_parameter();
		get_current_time(&t2);
		for(;;){
			get_current_time(&t3);
			if((t3.v_m-t2.v_m)>=(30))
				break; 
			r.system_call=(-1);
			call_kernel(&r);
		}
		for(i=15;i>=0;i--){
			r.system_call=(-1);
			call_kernel(&r);
			m_par->flush_memory_domain.
				max_flush_number=64;
			m_par->flush_memory_domain.giveup_flag=FALSE;
			m_par->flush_memory_domain.
				free_block_flag=FALSE;
			m_par->flush_memory_domain.wait_flag=FALSE;
			m_par->flush_memory_domain.domain_id=i;
			RESET_CAPABILITY(m_par->flush_memory_domain.
				domain_capability);
			r.system_call=call_memory(MEMORY_BODY_ID,
				FLUSH_PROCESS_MEMORY_CALL);
		}

		for(i=2*OS_CPU_SET_NUMBER;i<OS_CPU_NUMBER;i++){
			r.system_call=CPU_SET_SYSTEM_CALL;
			r.r1=i;r.r2=i%3;
			RESET_CAPABILITY(get_kernel_parameter()->
				capability.capability_1);
			call_kernel(&r);
			r.system_call=(-1);
			call_kernel(&r);
		}

		for(i=0;i<OS_CPU_NUMBER;i++){
			r.system_call=CPU_SET_SYSTEM_CALL;
			r.r1=i;r.r2=i%OS_CPU_SET_NUMBER;
			RESET_CAPABILITY(get_kernel_parameter()->
				capability.capability_1);
			call_kernel(&r);
			r.system_call=(-1);
			call_kernel(&r);
		}
	} 
	r.system_call=THREAD_RETURN_SYSTEM_CALL;
	r.r1=TRUE;
	call_kernel(&r);

	return ;
}
