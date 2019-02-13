#include"../driver.h"
#define WRONG_FILE_NUMBER	-1
#define WRONG_FILE_STATE	-2
#define WRONG_CAPABILITY	-3
int set_query_file_window_attribute(int set_or_query_flag,
	int file_window_id,struct kernel_file_window *file_window)
{
	struct file_window *f;

	if(file_window_id<0)
		return WRONG_FILE_NUMBER;
	if(file_window_id>=(memory_body->head->file_number))
		return WRONG_FILE_NUMBER;
	f=(memory_body->file_window)+file_window_id;
	if(f->state!=FILE_OPEN)
		return WRONG_FILE_STATE;
	if(!KERNEL_COMPARE_CAPABILITY(
		(f->kernel_file_window.file.file_capability),
		(file_window->file.file_capability)))
			return WRONG_CAPABILITY;
	mark_file_accessed(file_window_id);
	if(set_or_query_flag){
		COPY_FILE_WINDOW(*file_window,f->kernel_file_window); 
	}else{
		COPY_FILE_WINDOW(f->kernel_file_window,*file_window); 
	}
	return file_window_id;
} 
