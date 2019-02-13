#ifndef OS_MEMORY_MEMORY_CALL
#define OS_MEMORY_MEMORY_CALL
#define MAX_FLUSH_RESULT_NUMBER 	12
union memory_call_parameter{
	struct open_or_control_file{
		int command,flag;
		int begin_address,data_length,memory_domain;
		struct kernel_file_window kernel_file_window; 
		CAPABILITY domain_capability;
	}open_or_control_file; 
	struct control_opened_file{
		int command,flag;
		int begin_address,data_length,file_window_id;
		CAPABILITY file_capability;
	}control_opened_file;
	struct install_memory_body_parameter memory_body_parameter; 
	struct close_file_window{
		int file_window_id,giveup_flag,wait_flag;
		CAPABILITY file_capability;
		int flush_number[MAX_FLUSH_RESULT_NUMBER];
	}close_file_window;
	struct file_attribute{
		int file_window_id;
		struct kernel_file_window file_window;
	}file_attribute;
	struct wakeup_file_window{
		int file_window_id,no_wait_flag,max_number;
		CAPABILITY file_capability;
	}wakeup_file_window;
	struct wakeup_memory_domain{
		int domain_id,no_wait_flag,max_number,max_file_number;
		CAPABILITY domain_capability;
	}wakeup_memory_domain; 
	struct memory_setup_map{
		int file_window_id,read_only_flag;
		int begin_logic_address,end_logic_address;
		CAPABILITY file_capability;
	}memory_setup_map;
	struct flush_memory_domain{
		int max_flush_number,domain_id;
		int giveup_flag,free_block_flag,wait_flag;
		CAPABILITY domain_capability;
		int flush_number[MAX_FLUSH_RESULT_NUMBER];
	}flush_memory_domain;
	struct flush_file_window{
		int file_window_id,update_file_window_id;
		int max_flush_number;
		int giveup_flag,free_block_flag,wait_flag;
		CAPABILITY file_capability,update_file_capability;
		int flush_number[MAX_FLUSH_RESULT_NUMBER];
	}flush_file_window;
	struct memory_resource{
		int domain_id;
		struct memory_domain_struct domain_data;
		int free_block_number,free_physical_block_number;
	}memory_resource;
}; 
#endif
