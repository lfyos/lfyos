#include "dns.h"
#define WRONG_TREE_NODE_ID	-1
#define WRONG_CAPABILITY	-2 
int dns_get_first_name(char *name,int name_length,
	CAPABILITY *file_capability)
{
	int node_id,child_id,name_id,i,child_number;
	if((node_id=dns_search_tree_node(0,name))<0)
		return node_id;
	if(node_id>=DNS_MAX_TREE_NODE_NUMBER)
		return WRONG_TREE_NODE_ID;
	if(!KERNEL_COMPARE_CAPABILITY(
		(dns->tree_node[node_id].file.file_capability),
		(*file_capability)))
			return WRONG_CAPABILITY;
	child_number=dns->tree_node[node_id].child_number;
	if((child_id=dns->tree_node[node_id].first_child)>=0){
		dns->tree_node[node_id].first_child=
			dns->tree_node[child_id].right_brother; 
		for(name_id=dns->tree_node[child_id].name_id;;){
			if((name_id<0)||(name_length<=0))
				break;
			for(i=0;i<DNS_NAME_ELEMENT_LENGTH;i++){
				if(name_length<=0)
					break;
				if((*name=dns->name_element[name_id].
					element[i])==END_CHAR)
				{
					name_length=0;
					break;
				}
				name++;
				name_length--;
			}
			name_id=dns->name_element[name_id].next_element;
		}
	}
	*name=END_CHAR;
	return child_number;
}
