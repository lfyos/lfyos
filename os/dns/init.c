#include"dns.h"
void dns_init()
{
	int i,j;
	for(i=0;i<DNS_MAX_NAME_NUMBER;i++){
		for(j=0;j<DNS_NAME_ELEMENT_LENGTH;j++)
			dns->name_element[i].element[j]=END_CHAR;
		dns->name_element[i].next_element=i+1;
	}
	dns->name_element[DNS_MAX_NAME_NUMBER-1].next_element=(-1);
	dns->free_name_element=0;

	for(i=0;i<DNS_MAX_TREE_NODE_NUMBER;i++){
		dns->tree_node[i].name_id=(-1);
		dns->tree_node[i].parent=(-1);
		dns->tree_node[i].left_brother=(-1);
		dns->tree_node[i].right_brother=i+1;
		dns->tree_node[i].first_child=(-1);
		dns->tree_node[i].child_number=0;
	}
	dns->tree_node[DNS_MAX_TREE_NODE_NUMBER-1].right_brother=(-1);
	dns->free_tree_node=1;

	dns->tree_node[0].left_brother=0;
	dns->tree_node[0].right_brother=0;

	COPY_CAPABILITY(os->system_capability,
		dns->tree_node[0].file.file_capability); 
	return;
}
