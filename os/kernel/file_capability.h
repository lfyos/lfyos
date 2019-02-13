#ifndef OS_FILE_CAPABILITY
#define OS_FILE_CAPABILITY

struct user_file_information{
	int memory_body_id,file_window_id,map_length,right;
	CAPABILITY file_capability;
};
#define COMPARE_USER_FILE_INFORMATION(s,d)			\
(								\
	((s).memory_body_id==(d).memory_body_id)		\
	&&((s).file_window_id==(d).file_window_id)		\
	&&((s).right==(d).right)				\
	&&COMPARE_CAPABILITY((s).file_capability,(d).file_capability)\
) 
#define COPY_USER_FILE_INFORMATION(s,d)			\
{							\
	(d).memory_body_id=(s).memory_body_id;		\
	(d).file_window_id=(s).file_window_id;		\
	(d).map_length=(s).map_length;			\
	(d).right=(s).right;				\
	COPY_CAPABILITY((s).file_capability,(d).file_capability);\
} 
#define RESET_USER_FILE_INFORMATION(file)		\
{							\
	(file).memory_body_id=(-1);			\
	(file).file_window_id=(-1);			\
	(file).map_length=(-1);				\
	(file).right=NO_ACCESS_RIGHT;			\
	RESET_CAPABILITY((file).file_capability);	\
} 
#endif 
