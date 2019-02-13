#include"../kernel/capability.h"
#include"../arch/sys_c/par.h"
#include"../include/os.h"
#include"../platform/include/service.h"

#define STEP 64

extern void update_test(int file_id,int number);
extern int open_file(int fd); 
void test()
{
	call_kernel_register r;
	struct kernel_time time;
	int ii,i,j,k,l,thread_id,s=0,file_id;
	volatile int *volatile p;
for(ii=0;;ii++){
	r.system_call=-1;
	call_kernel(&r);
	thread_id=r.system_call;
	print("\nthread begin :",thread_id);
	for(i=1;i<FILE_NUMBER-1;i++){
		for(j=0;j<1;j++)
			file_id=open_file(i);
		r.system_call=THREAD_FILE_SYSTEM_CALL;
		r.r1=1;
		r.r2=512;
		r.r3=READ_WRITE_RIGHT;
		r.r4=MEMORY_BODY_ID;
		r.r5=file_id;
		RESET_CAPABILITY(get_kernel_parameter()
			->capability.capability_1);
		call_kernel(&r);

		print("\nThread ",thread_id);
		print("set result is ",r.system_call);

		for(j=0,p=(int *)(2*512*1024*1024)-1;j<128*STEP;j=j+1){
			if(((thread_id+i+j)%2)!=0){
				print_string("\nR:");
				for(k=0;k<1024;k=k+STEP){
					for(l=0,s=0;l<1024;l=l+1,p=p-1)
						s=s+(*p);
				}
			}else{
				print_string("\nW:");
				for(k=0;k<1024;k=k+STEP)
				for(l=0,s=0;l<1024;l=l+1,p=p-1){
					asm volatile(
					"xchgl (%%esi),%%eax\n\t"
					:"=a"(r.system_call)
					:"a"(*p),"S"(p));
					s=s+r.system_call;
				}
			}
			get_current_time(&time);
			print("Time",time.v_m);
			print("thread",thread_id);
			print(":",ii);
			print("file",i);
			print(":",j); 
			print(":",s);

			get_kernel_parameter()->set_thread_attribute.
				priority=0;
			get_kernel_parameter()->set_thread_attribute.
				cpu_set_id=(time.v_l)%OS_CPU_SET_NUMBER;
			get_kernel_parameter()->set_thread_attribute.
				signal_bitmap=0x12345678;
			get_kernel_parameter()->set_thread_attribute.
				mandatory_exit_number=0; 
			r.system_call=THREAD_SET_SYSTEM_CALL;
			r.r1=(-1);
			call_kernel(&r);
			r.system_call=-1;
			call_kernel(&r);
			if((thread_id%2)==0)
				update_test(file_id,64);
		}
	}
}
	r.system_call=THREAD_RETURN_SYSTEM_CALL;
	r.r1=1;
	call_kernel(&r);
	return ;
} 
void update_test(int file_id,int number)
{
	int i=0;
	union memory_call_parameter *par=get_memory_parameter();

	par->flush_file_window.file_window_id=file_id;
	par->flush_file_window.update_file_window_id
		=(file_id>=2)?file_id-1:-1;
	par->flush_file_window.max_flush_number=number;
	par->flush_file_window.giveup_flag=FALSE;
	par->flush_file_window.free_block_flag=FALSE;
	par->flush_file_window.wait_flag=FALSE;
	RESET_CAPABILITY(par->flush_file_window.file_capability);
	RESET_CAPABILITY(par->flush_file_window.update_file_capability);

	i=call_memory(MEMORY_BODY_ID,FLUSH_FILE_WINDOW_MEMORY_CALL); 

	print("upd",i);
	for(i=0;i<MAX_FLUSH_RESULT_NUMBER;i++)
		print(":",par->flush_file_window.flush_number[i]);
	return;
} 
