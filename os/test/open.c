#include"../kernel/capability.h"
#include"../arch/sys_c/par.h"
#include"../dns/dns.h"
#include"../include/os.h"
#include"../platform/include/service.h"
static void create_name(int fd,char *name)
{
	int i;
	for(i=31;i>=0;i--,fd=fd>>1)
		name[i]=(char)((fd&0x01)+(int)'0'); 
	name[32]=END_CHAR;
}
void register_file(int fd)
{
	union memory_call_parameter *par;
	struct file_system_return_add_name_and_id *p;

	par=get_memory_parameter(); 
	p=(struct file_system_return_add_name_and_id *)
		get_thread_physical_block();
	par->open_or_control_file.command=ADD_NAME_AND_ID;
	par->open_or_control_file.flag=FILE_WINDOW_DEFAULT_FLAG;
	par->open_or_control_file.data_length=PAGE_SIZE;
	par->open_or_control_file.memory_domain=fd%16; 
	RESET_CAPABILITY(par->open_or_control_file.domain_capability);

	create_name(fd,&(p->name[0]));

	if((fd%(0x03))==0){
		p->file.server_id=(-MEMORY_BODY_ID);
	}else if((fd%(0x03))==1)
		p->file.server_id=1;
	else 
		p->file.server_id=2;

	p->file.file_id=fd;
	RESET_CAPABILITY(p->file.file_capability); 
	RESET_CAPABILITY(p->file.remote.domain_capability);
	p->file.remote.server_id=0;
	p->file.remote.file_id=0;
	p->file.remote.domain_id=0;
	p->file.remote.priority=0;
	p->file.remote.network_node.address=0;

	RESET_CAPABILITY(p->parent_capability); 
	RESET_CAPABILITY(p->modify_capability); 
	print("\nregister file ",fd);
	print("result is ",call_memory(MEMORY_BODY_ID,
		CONTROL_UNOPENED_FILE_MEMORY_CALL_FROM_ROOT));
	return;
}
int open_file(int fd)
{
	int i,return_value;
	union memory_call_parameter *par;
	struct file_system_return_name_to_id *p;

	par=get_memory_parameter(); 
	p=(struct file_system_return_name_to_id *)
		get_thread_physical_block();
	for(i=0;;i++){
		par->open_or_control_file.command=NAME_TO_ID;
		par->open_or_control_file.flag=FILE_WINDOW_DEFAULT_FLAG;
		par->open_or_control_file.data_length=PAGE_SIZE;
		par->open_or_control_file.memory_domain=fd%16;
		RESET_CAPABILITY(par->open_or_control_file.
			domain_capability);

		create_name(fd,&(p->name[0]));
		RESET_CAPABILITY(p->file.file_capability); 

		return_value=call_memory(MEMORY_BODY_ID,
			(i!=0)?CONTROL_UNOPENED_FILE_MEMORY_CALL
			:CONTROL_UNOPENED_FILE_MEMORY_CALL_FROM_ROOT);
		print("\nfile name to id result ",return_value);
		print("id is ",p->file.file_id);
		
		COPY_FILE_INFORMATION((p->file),
		   (par->open_or_control_file.kernel_file_window.file));
		if(return_value==0)
			break;
	} 
	par->open_or_control_file.command=OPEN_FILE_WINDOW_MEMORY_CALL; 
	par->open_or_control_file.flag=FILE_WINDOW_DEFAULT_FLAG;
	par->open_or_control_file.data_length=PAGE_SIZE;
	par->open_or_control_file.memory_domain=fd%16;
	RESET_CAPABILITY(par->open_or_control_file.domain_capability); 
	RESET_FILE_OFFSET(par->open_or_control_file.
		kernel_file_window.offset);
	par->open_or_control_file.kernel_file_window.window_length
		=MAX_FILE_WINDOW_LENGTH;
	par->open_or_control_file.kernel_file_window.right
		=READ_WRITE_RIGHT;
		 
	fd=call_memory(MEMORY_BODY_ID,OPEN_FILE_WINDOW_MEMORY_CALL);
	print("open file result",fd);
	return fd;
}
