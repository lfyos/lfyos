#include"../../include/os.h"
char *kernel_get_thread_physical_block(int thread_id)
{
	return	(((1024*1024)+(2*thread_id*PAGE_SIZE))
			+kernel_get_page_table_base());
}
char *get_sp_0()
{
	unsigned int x=(unsigned int)get_kernel_parameter();
	unsigned int y=(unsigned int)get_memory_parameter();
	unsigned int z=(unsigned int)get_file_system_parameter();

	x=(x>y)?((y>z)?z:y):((x>z)?z:x);
	return (char *)(x-32);
}
char *get_sp()
{
	return get_sp_0()-2048-512;
}
int get_cs()
{
	return DEFAULT_CS;
}
int get_ds()
{
	return DEFAULT_DS;
}
