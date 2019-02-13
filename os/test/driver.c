#include "../kernel/kernel.h"
#include "../dns/dns.h"
#include "../include/os.h"
#include "../platform/include/service.h"
#include "../arch/sys_c/par.h"

extern void __end_rodata(),_etext();
extern int dns_driver(
	struct file_system_call_parameter *par,int sleep_semaphore);
static int test_device_driver(
	struct file_system_call_parameter *par,int sleep_semaphore)
{
	int *p,*p1,fd,length,begin_address,rw_length;
	struct file_read_parameter *read_par;

	length=((int)__end_rodata)&(0x1fffffff); 
	fd=par->kernel_file_window.file_window_id; 
	begin_address=par->begin_address;
	rw_length=par->data_length;
	p=(int *)map_memory_block(par->block.memory_id,par->block.block_id);
	switch(par->command){
	case READ_FILE:
		read_par=(struct file_read_parameter *)p;
		if((fd%3)!=0){
			read_par->block_flag=DEFAULT_MEMORY_BLOCK_FLAG;
			read_par->parent_file_window_id=fd-1;
			if(begin_address>=length){
				read_par->begin_address=begin_address;
				read_par->begin_address-=length;
			}
			RESET_CAPABILITY(read_par->
				parent_file_window_capability);
			par->reply_result=0;
			mark_memory_block_unmap(par->block.memory_id,
				par->block.block_id);
			return -1;
		}
		begin_address%=length;
		begin_address+=length+length;
		begin_address%=length;
		if(begin_address<length)
			p1=(int *)(begin_address|(0xc0000000));
		else if(begin_address<5*length)
			p1=(int *)(begin_address-length);
		else
			p1=(int *)(begin_address%length+4*length); 
		copy_data(p1,p,rw_length);
		break;
	case WRITE_FILE:
		break;
	}
	par->reply_result=0;
	mark_memory_block_unmap(par->block.memory_id,par->block.block_id);
	return par->reply_result;
} 
extern int memory_call_process(int routine_id,
	union memory_call_parameter *par);

static int put_get(volatile int *address,int value)
{
	asm volatile(
		"movl %%eax,%%ebx\n\t"
		"xchgl %%eax,(%%esi)\n\t"
		"xchgl %%eax,(%%esi)\n\t"
		"xorl %%ebx,%%eax\n\t"
		:"=a"(value)
		:"0"(value),"S"(address)
		:"%ebx"
	);
	return value;
}
static void dns_init_routine(struct driver *dr)
{
	extern void dns_init();

	dr->device_spin_lock=1;
	dns_init();
}
static char *test_map_memory_block(int block_id)
{
	return PAGE_SIZE*block_id
		+kernel_get_thread_physical_block(OS_THREAD_NUMBER);
}
static char *get_memory_body_base()
{
	return test_map_memory_block(0);
}
static void test_mark_memory_block_unmap(int block_id)
{
	return;
}
static void init_routine(struct driver *dr)
{
	static struct memory_body_struct m;
	int i,j,k,*p; 

	dr->memory_body_pointer=&m; 
	dr->device_spin_lock=100000;
	for(dr->memory_body_capacity=0,k=0;;k++){
		for(i=0;i<1024;i++,dr->memory_body_capacity++){
			p=(int *)test_map_memory_block(k*1024+i);
			j=((int)p)&0xff000000;
			if(j==0xff000000)
				return ;
			for(j=0;j<1024;j+=256,p+=256){
				if(put_get(p,0x5a5a5a5a)!=0)
					return ;
				if(put_get(p,0xa5a5a5a5)!=0)
					return ;
			}
		}
	}
	return ;
} 
struct driver driver[]=
{
	{	0,0,
		get_memory_body_base,
		test_map_memory_block,test_mark_memory_block_unmap,
		dns_init_routine,
		0,
		dns_driver,
		0,
	},
	{	0,0,
		get_memory_body_base,
		test_map_memory_block,test_mark_memory_block_unmap,
		(MEMORY_BODY_ID!=1)?0:init_routine,
		(MEMORY_BODY_ID!=1)?0:memory_call_process,
		(MEMORY_BODY_ID!=1)?0:test_device_driver,
		0,
	},
	{	0,0,
		get_memory_body_base,
		test_map_memory_block,test_mark_memory_block_unmap,
		(MEMORY_BODY_ID!=2)?0:init_routine,
		(MEMORY_BODY_ID!=2)?0:memory_call_process,
		(MEMORY_BODY_ID!=2)?0:test_device_driver,
		0,
	},
};
int driver_number=((sizeof(driver)/sizeof(driver[0])));
