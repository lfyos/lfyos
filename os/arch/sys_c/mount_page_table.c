#include"../../driver/driver.h" 
#include"par.h" 

#define file_page_table	(memory_body_page_table[memory_body_id].fgt)

void mount_page_table(int memory_body_id,int file_id,
	int map_length,CAPABILITY *file_capability)
{
	int block_number;

	if(memory_body_id<0)
		return ;
	if(memory_body_id>=memory_body_page_table_number)
		return ; 
	block_number=(map_length+PAGE_SIZE)/(PAGE_SIZE*1024);
	file_page_table[file_id].length=block_number;
	file_page_table[file_id].memory_body_id=(-1);
	file_page_table[file_id].physical_block_id=(-1);
	file_page_table[file_id].reference_number=0; 
	COPY_CAPABILITY(*file_capability,
		file_page_table[file_id].file_capability);
}
void unmount_page_table(int memory_body_id,int file_id)
{
	if(memory_body_id<0)
		return ;
	if(memory_body_id>=memory_body_page_table_number)
		return ; 
	file_page_table[file_id].length=0;
	file_page_table[file_id].memory_body_id=-1;
	file_page_table[file_id].physical_block_id=-1;
	file_page_table[file_id].reference_number=0;
}
