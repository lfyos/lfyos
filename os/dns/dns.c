#include"dns.h" 
#include"../include/os.h" 
#define WRONG_TREE_NODE_ID	-1
#define WRONG_CAPABILITY	-1 
static int name_to_id_routine(struct file_system_call_parameter *par,
	struct file_system_return_name_to_id *arg)
{
	int node_id,last_name;
	char *p,*q;
	CAPABILITY *c_s,*c_d;
	struct kernel_file_information *f_i;

	p=&(arg->name[0]);
	last_name=sizeof(arg->name)/sizeof(arg->name[0])-1;
	arg->name[last_name]=END_CHAR;
	if((node_id=dns_search_part_tree_node(0,&p))<0)
		return node_id;
	if(node_id>=DNS_MAX_TREE_NODE_NUMBER)
		return WRONG_TREE_NODE_ID;
	f_i=&(dns->tree_node[node_id].file);
	c_s=&(f_i->file_capability);
	c_d=&(arg->file.file_capability);
	if(!KERNEL_COMPARE_CAPABILITY((*c_s),(*c_d)))
		return WRONG_CAPABILITY;

	COPY_FILE_INFORMATION((*f_i),(arg->file));
	for(q=&(arg->name[0]),par->reply_result=0;;p++,q++){
		if((*q=(*p))==END_CHAR)
			break;
		par->reply_result++;
		if(p==&(arg->name[last_name]))
			break;
	}
	arg->name[last_name]=END_CHAR;
	return par->reply_result;
}
static int add_name_and_id_routine(
	struct file_system_return_add_name_and_id *arg)
{
	arg->name[-1+sizeof(arg->name)/sizeof(arg->name[0])]=END_CHAR;
	return dns_create_tree_node(
		arg->name,&(arg->result_flag),&(arg->file),
		&(arg->parent_capability),&(arg->modify_capability));
}
static int delete_name_and_id_routine(
	struct file_system_return_delete_name_and_id *arg)
{
	arg->name[-1+sizeof(arg->name)/sizeof(arg->name[0])]=END_CHAR;
	return dns_release_tree_node(arg->name,
		arg->with_children_flag,&(arg->delete_capability));
}
static int list_name_and_id_routine(
	struct file_system_return_list_name *arg)
{
	arg->name[-1+sizeof(arg->name)/sizeof(arg->name[0])]=END_CHAR;
	return dns_get_first_name(&(arg->name[0]),
		sizeof(arg->name),&(arg->file_capability));
}
int dns_driver(
	struct file_system_call_parameter *par,int sleep_semaphore_id)
{
	int return_value=0; 

	switch(par->command){
	default:
	case OPEN_FILE:
	case CLOSE_FILE:
	case READ_FILE: 
	case WRITE_FILE:
	case OPERATE_FILE:
		break;
	case NAME_TO_ID:
		return_value=name_to_id_routine(par,
			(struct file_system_return_name_to_id *)
				(get_thread_physical_block()));
		break; 
	case ADD_NAME_AND_ID:
		return_value=add_name_and_id_routine(
			(struct file_system_return_add_name_and_id *)
				(get_thread_physical_block()));
		break;
	case DELETE_NAME_AND_ID:
		return_value=delete_name_and_id_routine(
			(struct file_system_return_delete_name_and_id *)
				(get_thread_physical_block()));
		break;
	case LIST_NAME_AND_ID:
		return_value=list_name_and_id_routine(
			(struct file_system_return_list_name *)
				(get_thread_physical_block()));
		break;
	}
	par->reply_result=return_value;
	return return_value;
} 
