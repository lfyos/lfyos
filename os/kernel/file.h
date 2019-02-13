#ifndef OS_NETWORK_STRUCT
#define OS_NETWORK_STRUCT 
struct network_address_struct{
	int address;
};
#define SET_LOCAL_NETWORK_ADDRESS(d)	{(d).address=0;}
#define COPY_NETWORK_ADDRESS(s,d)				\
{								\
	(d).address=(s).address;				\
} 
#define IS_SAME_NETWORD_ADDRESS(s,d)	((s).address==(d).address)

typedef struct {
	int offset_1,offset_2;
}FILE_OFFSET;
#define RESET_FILE_OFFSET(d)	{(d).offset_1=0;(d).offset_2=0;}
#define COPY_FILE_OFFSET(s,d)			\
	{(d).offset_1=(s).offset_1;(d).offset_2=(s).offset_2;}
#define IS_SAME_FILE_OFFSET(s,d)		\
	(((s).offset_1==(d).offset_1)&&((s).offset_2==(d).offset_2))

struct kernel_file_information{
	struct {
		struct network_address_struct network_node;
		int server_id,file_id,domain_id,priority;
		CAPABILITY domain_capability;
	}remote;
	int server_id,file_id;
	CAPABILITY file_capability;
};
#define SET_INITIAL_FILE_INFORMATION(d)				\
{								\
	SET_LOCAL_NETWORK_ADDRESS((d).remote.network_node);	\
	(d).remote.server_id=0;		(d).remote.file_id=0;	\
	(d).remote.domain_id=0;	(d).remote.priority=0;	\
	RESET_CAPABILITY((d).remote.domain_capability);	\
	(d).server_id=0;		(d).file_id=0;		\
	RESET_CAPABILITY((d).file_capability);			\
}
#define COPY_FILE_INFORMATION(s,d)		\
	{copy_data(&(s),&(d),sizeof(struct kernel_file_information));}
#define IS_SAME_KERNEL_FILE_INFORMATION(s,d)			\
(								\
	  ((s).server_id==(d).server_id)			\
	&&((s).file_id==(d).file_id)				\
	&&COMPARE_CAPABILITY((s).file_capability,(d).file_capability)\
	&&IS_SAME_NETWORD_ADDRESS(				\
		(s).remote.network_node,(d).remote.network_node)\
	&&((s).remote.server_id==(d).remote.server_id)		\
	&&((s).remote.file_id==(d).remote.file_id)		\
	&&((s).remote.domain_id==(d).remote.domain_id)		\
	&&((s).remote.priority==(d).remote.priority)		\
	&&COMPARE_CAPABILITY((s).remote.domain_capability,	\
		(d).remote.domain_capability)			\
)
struct kernel_file_window{
	struct kernel_file_information file;
	FILE_OFFSET offset;
	int file_window_id,window_length,right;
};
#define SET_INITIAL_FILE_WINDOW(d)				\
{								\
	SET_INITIAL_FILE_INFORMATION((d).file)			\
	RESET_FILE_OFFSET((d).offset);				\
	(d).file_window_id=-1;					\
	(d).window_length=MAX_FILE_WINDOW_LENGTH;		\
	(d).right=READ_WRITE_RIGHT; 				\
}
#define IS_SAME_KERNEL_FILE_WINDOW(s,d)				\
(								\
	  IS_SAME_KERNEL_FILE_INFORMATION((s).file,(d).file)	\
	&&IS_SAME_FILE_OFFSET((s).offset,(d).offset)		\
	&&((s).window_length==(d).window_length)		\
	&&((s).right==(d).right)				\
)
#define COPY_FILE_WINDOW(s,d)		\
	{copy_data(&(s),&(d),sizeof(struct kernel_file_window));}

struct file_read_parameter{
	int parent_file_window_id,begin_address,block_flag;
	CAPABILITY parent_file_window_capability;
};
#define DEFAULT_MEMORY_BLOCK_FLAG		1
#define PRIVATE_SHARE_MEMORY_BLOCK_FLAG		1

struct file_system_call_parameter{
	int command,flag,begin_address,data_length,reply_result;
	int reply_id,reply_code;
	struct thread_physical_block block;
	struct kernel_file_window kernel_file_window;
};
#define FILE_SYSTEM_CALL_PARAMETER_WAIT_FOR_REPLAY_FLAG		1
#define FILE_SYSTEM_CALL_PARAMETER_DEFAULT_FLAG			1

#define OPEN_FILE		0
#define CLOSE_FILE		1
#define READ_FILE		2
#define WRITE_FILE		3
#define OPERATE_FILE		4
#define NAME_TO_ID		5
#define ADD_NAME_AND_ID		6
#define DELETE_NAME_AND_ID	7
#define LIST_NAME_AND_ID	8

struct file_system_return_name_to_id{
	struct kernel_file_information file;
	char name[(PAGE_SIZE-
		sizeof(struct kernel_file_information))/sizeof(char)];
};
struct file_system_return_add_name_and_id{
	int result_flag;
	CAPABILITY parent_capability,modify_capability;
	struct kernel_file_information file;
	char name[(PAGE_SIZE-sizeof(int)-
		sizeof(CAPABILITY)-sizeof(CAPABILITY)-
		sizeof(struct kernel_file_information))/sizeof(char)];
};
struct file_system_return_delete_name_and_id{
	int with_children_flag;
	CAPABILITY  delete_capability;
	char name[(PAGE_SIZE-sizeof(int)-
		sizeof(CAPABILITY))/sizeof(char)];
};
struct file_system_return_list_name{
	CAPABILITY  file_capability;
	char name[(PAGE_SIZE-sizeof(CAPABILITY))/sizeof(char)];
};
#endif
