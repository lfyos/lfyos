#include"driver.h"
#include"../include/os.h"
int memory_main_map_process(int memory_body_id,int file_window_id,
	int begin_logic_address,int end_logic_address,
	int read_only_flag,CAPABILITY *file_capability)
{
	int return_value;
	union memory_call_parameter *par=get_memory_parameter();
	typeof (par->memory_setup_map) par_bak;
	
	copy_data(&(par->memory_setup_map),&par_bak,sizeof(par_bak));
	par->memory_setup_map.file_window_id=file_window_id;
	par->memory_setup_map.begin_logic_address=begin_logic_address;
	par->memory_setup_map.end_logic_address=end_logic_address;
	par->memory_setup_map.read_only_flag=read_only_flag;
	COPY_CAPABILITY((*file_capability),
		(par->memory_setup_map.file_capability));

	return_value=call_memory(memory_body_id,
		SETUP_MAP_MEMORY_CALL);

	copy_data(&par_bak,&(par->memory_setup_map),sizeof(par_bak));
	return return_value;
}
char *map_memory_block(int body_id,int block_id)
{
	if((body_id<0)||(body_id>=driver_number))
		return (char *)0;
	if((int)(driver[body_id].driver_map_memory_block)==0)
		return (char *)0;
	return driver[body_id].driver_map_memory_block(block_id);
}
void mark_memory_block_unmap(int body_id,int block_id)
{
	return;
}
