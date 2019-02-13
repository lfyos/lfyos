#include"dns.h"
#define WRONG_TREE_NODE_ID		(-1)
#define WRONG_PARENT_TREE_NODE_ID	(-2)
#define EXIST_CHILDREN_WHEN_RELEASE	(-3)
#define WRONG_CAPABILITY		(-4)
static int dns_release_one_tree_node(int node_id)
{	
	int i,first_name_id,next_name_id;
	int parent,left_brother,right_brother;

	if((node_id<=0)||(node_id>=DNS_MAX_TREE_NODE_NUMBER))
		return WRONG_TREE_NODE_ID;
	if((parent=dns->tree_node[node_id].parent)>=0){
		if(parent>=DNS_MAX_TREE_NODE_NUMBER)
			return WRONG_PARENT_TREE_NODE_ID;
		left_brother=dns->tree_node[node_id].left_brother;
		if(node_id==left_brother){
			dns->tree_node[parent].first_child=-1;
			dns->tree_node[parent].child_number=0;
		}else{
			dns->tree_node[parent].child_number--;
			right_brother=dns->tree_node[node_id].
				right_brother;
			dns->tree_node[left_brother].right_brother
				=right_brother;
			dns->tree_node[right_brother].left_brother
				=left_brother;
			if(dns->tree_node[parent].first_child==node_id)
				dns->tree_node[parent].first_child
					=right_brother;
		}
	}
	if((first_name_id=dns->tree_node[node_id].name_id)>=0){
		for(i=first_name_id;;i=next_name_id){
			next_name_id=dns->name_element[i].next_element;
			if(next_name_id<0){
				dns->name_element[i].next_element
					=dns->free_name_element;
				dns->free_name_element=first_name_id;
				break;
			}
		}
	}
	dns->tree_node[node_id].name_id=(-1);
	dns->tree_node[node_id].parent=(-1);
	dns->tree_node[node_id].left_brother=(-1);
	dns->tree_node[node_id].right_brother=dns->free_tree_node;
	dns->tree_node[node_id].first_child=(-1);
	dns->tree_node[node_id].child_number=0;
	dns->free_tree_node=node_id;

	return node_id;
}
int dns_release_tree_node(char *name,
	int with_children_flag,CAPABILITY *modify_capability)
{
	int i,first_id,node_id;

	if((node_id=dns_search_tree_node(0,name))<0)
		return node_id;
	if(node_id>=DNS_MAX_TREE_NODE_NUMBER)
		return WRONG_TREE_NODE_ID;
	if(!KERNEL_COMPARE_CAPABILITY(
		(dns->tree_node[node_id].modify_capability),
		(*modify_capability)))
			return WRONG_CAPABILITY;
	if(with_children_flag)
		for(;;){
			first_id=dns->tree_node[node_id].first_child;
			if(first_id<0)
				break;
			for(;;){
				i=dns->tree_node[first_id].first_child;
				if(i<0)
					break;
				first_id=i;
			}
		 	if((i=dns_release_one_tree_node(first_id))<0)
				return i;
		}
	else
		if(dns->tree_node[node_id].first_child>=0)
			return EXIST_CHILDREN_WHEN_RELEASE;
	return dns_release_one_tree_node(node_id);
}
