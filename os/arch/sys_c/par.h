extern int page_dir[1024];
struct file_page_table_item{
	int page_table_block[128];
	int item_number[128];
	int reference_number[128];
};
struct file_page_table{
	int length,reference_number;
	int memory_body_id,physical_block_id;
	CAPABILITY file_capability;
};
struct memory_body_page_table{
	struct file_page_table *fgt;
	int number;
};
extern struct memory_body_page_table memory_body_page_table[];
extern int memory_body_page_table_number; 
extern int *add_tlb_buf(int dir,int memory_body_id,
	int system_file,int file_dir,int right); 
