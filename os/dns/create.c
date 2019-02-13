#include"dns.h"
#define NO_NAME_ELEMENT			(-1)
#define NO_TREE_NODE			(-2) 
#define WRONG_CAPABILITY		(-3)
#define WRONG_PARENT_ID			(-4)
static int dns_create_name(char *name)
{
	int first_id,last_id,cur_id,i;
	for(first_id=-1,last_id=-1;;){
		if((cur_id=dns->free_name_element)<0){
			dns->free_name_element=first_id;
			return NO_NAME_ELEMENT;
		}
		dns->free_name_element
			=dns->name_element[cur_id].next_element;
		dns->name_element[cur_id].next_element=(-1);
		if(first_id<0)
			first_id=cur_id;
		else
			dns->name_element[last_id].next_element=cur_id; 
		last_id=cur_id;
		for(i=0;i<DNS_NAME_ELEMENT_LENGTH;i++,name++){
			dns->name_element[cur_id].element[i]=(*name);
			if((*name)==END_CHAR){
				for(;i<DNS_NAME_ELEMENT_LENGTH;i++)
					dns->name_element[cur_id].
						element[i]=END_CHAR;
				return first_id;
			}
		} 
	}
}
static int dns_create_tree_node_with_path_and_name(char *dir_name,
	char *name,int *result_flag,CAPABILITY *parent_capability)
{
	int parent_id,node_id,l_brother,r_brother,name_id;
	CAPABILITY *c_f;

	if(dir_name==name)
		parent_id=0;
	else if((parent_id=dns_search_tree_node(0,dir_name))<0)
		return parent_id;
	if((parent_id<0)||(parent_id>=DNS_MAX_TREE_NODE_NUMBER))
		return WRONG_PARENT_ID;
	c_f=&(dns->tree_node[parent_id].modify_capability);
	if(!KERNEL_COMPARE_CAPABILITY((*c_f),*parent_capability))
		return WRONG_CAPABILITY;
	if((node_id=dns_search_tree_node(parent_id,name))>=0){
		*result_flag=1;
		return node_id;
	}
	if((node_id=dns->free_tree_node)<0)
		return NO_TREE_NODE;
	if((name_id=dns_create_name(name))<0)
		return name_id;
	dns->free_tree_node=dns->tree_node[node_id].right_brother;

	dns->tree_node[node_id].name_id=name_id;
	dns->tree_node[node_id].parent=parent_id;
	dns->tree_node[node_id].first_child=(-1);
	dns->tree_node[node_id].child_number=0;
	dns->tree_node[parent_id].child_number++;
	if((l_brother=dns->tree_node[parent_id].first_child)<0){
		dns->tree_node[parent_id].first_child  =node_id;
		dns->tree_node[node_id  ].left_brother =node_id;
		dns->tree_node[node_id  ].right_brother=node_id;
	}else{
		r_brother=dns->tree_node[l_brother].right_brother;
		dns->tree_node[l_brother ].right_brother=node_id;
		dns->tree_node[r_brother].left_brother =node_id;
		
		dns->tree_node[node_id].right_brother=r_brother;
		dns->tree_node[node_id].left_brother =l_brother;
	}
	*result_flag=(-1);
	return node_id;
}
int dns_create_tree_node(char *name,
	int *result_flag,struct kernel_file_information *f,
	CAPABILITY *parent_capability,CAPABILITY *modify_capability)
{
	int ret_val;
	char *dir_name;
	
	if((*name)==END_CHAR){
		if(!KERNEL_COMPARE_CAPABILITY(
			(os->system_capability),(*parent_capability)))
				return WRONG_CAPABILITY;
		*result_flag=1;
		COPY_FILE_INFORMATION((*f),(dns->tree_node[0].file));
		COPY_CAPABILITY((*modify_capability),
			(dns->tree_node[0].modify_capability));
		return 0;
	}
	for(dir_name=name;;name++)
		if((*name)==END_CHAR)
			break;
	for(;;name--){
		if(dir_name==name){
			ret_val=dns_create_tree_node_with_path_and_name(
				dir_name,name,result_flag,
				parent_capability);
			break;
		}else if((*name)==SEPARATOR_CHAR){
			*name=END_CHAR;
			ret_val=dns_create_tree_node_with_path_and_name(
				dir_name,name+1,result_flag,
				parent_capability);
			*name=SEPARATOR_CHAR;
			break;
		}
	}
	if((ret_val>=0)&&(ret_val<DNS_MAX_TREE_NODE_NUMBER)){
		if(*result_flag<0)
			COPY_FILE_INFORMATION(
				(*f),(dns->tree_node[ret_val].file));
		COPY_CAPABILITY((*modify_capability),
			(dns->tree_node[ret_val].modify_capability));
	}
	return ret_val;
}
