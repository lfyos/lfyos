#ifndef OS_DNS
#define OS_DNS

#include "../kernel/kernel.h"

#define	DNS_NAME_ELEMENT_LENGTH		2
#define DNS_MAX_NAME_NUMBER		1024*100
#define DNS_MAX_TREE_NODE_NUMBER	256*100

#define END_CHAR			'\0'
#define SEPARATOR_CHAR			'/'

struct dns_name_element{
	char element[DNS_NAME_ELEMENT_LENGTH];
	int next_element;
};
struct dns_tree_node{
	int name_id;
	int parent,left_brother,right_brother,first_child,child_number;
	struct kernel_file_information file;
	CAPABILITY modify_capability;
};
struct dns_struct{
	int free_name_element,free_tree_node;
	struct dns_name_element name_element[DNS_MAX_NAME_NUMBER];
	struct dns_tree_node tree_node[DNS_MAX_TREE_NODE_NUMBER];
};

#ifndef LFYOS_USER_MODE

extern struct dns_struct my_dns;
#define dns (&my_dns)

extern void dns_init(); 
extern int dns_create_tree_node(char *name,
	int *result_flag,struct kernel_file_information *f,
	CAPABILITY *parent_capability,CAPABILITY *modify_capability);
extern int dns_release_tree_node(char *name,int with_children_flag,
	CAPABILITY *modify_capability);
extern int dns_get_first_name(char *name,int name_length,
	CAPABILITY *file_capability); 
extern int dns_search_part_tree_node(int node_id,char **name_pointer);
extern int dns_search_tree_node(int node_id,char *name); 
extern int dns_driver(
	struct file_system_call_parameter *par,int sleep_semaphore_id);
#endif 
#endif
