#include "../../driver/driver.h"
#include "par.h"

static struct file_page_table fgt[FILE_NUMBER]; 
struct memory_body_page_table memory_body_page_table[]=
{
	{ fgt,sizeof(fgt)/sizeof(fgt[0]) },
	{ fgt,sizeof(fgt)/sizeof(fgt[0]) },
	{ fgt,sizeof(fgt)/sizeof(fgt[0]) },
	{ fgt,sizeof(fgt)/sizeof(fgt[0]) },
	{ fgt,sizeof(fgt)/sizeof(fgt[0]) },
};
int memory_body_page_table_number
      =sizeof(memory_body_page_table)/sizeof(memory_body_page_table[0]); 
