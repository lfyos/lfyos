#include"dns.h"
#define NOT_FIND_TREE_NODE	(-1)
static int dns_is_equal_tree_node(int node_id,char *name)
{
	char s;
	int i,name_id=dns->tree_node[node_id].name_id;
	for(;;name_id=dns->name_element[name_id].next_element){
		if((name_id<0)||(name_id>=DNS_MAX_NAME_NUMBER))
			return ((*name)==END_CHAR)?0:(-1);
		for(i=0;i<DNS_NAME_ELEMENT_LENGTH;i++,name++){
			s=dns->name_element[name_id].element[i];
			if(s!=(*name))
				return (-1);
			if(s==END_CHAR)
				return 0;
		}
	}
}
int dns_search_part_tree_node(int node_id,char **name_pointer)
{
	int i,first_child;
	char *p,bak,*name;

	for(name=(*name_pointer);;){
		if((first_child=dns->tree_node[node_id].first_child)<0){
			*name_pointer=name;
			return node_id;
		}
		for(p=name;;p++){
			if(((*p)==END_CHAR)||((*p)==SEPARATOR_CHAR))
				break;
		}
		bak=(*p);
		for(i=first_child,*p=END_CHAR;;){
			if(dns_is_equal_tree_node(i,name)>=0){
				if((*p=bak)==END_CHAR){
					*name_pointer=p;
					return i;
				}
				name=p+1;
				node_id=i;
				break;
			}
			i=dns->tree_node[i].right_brother;
			if(i==first_child){
				*p=bak;
				*name_pointer=name;
				return node_id;
			}
		}
	}
}
int dns_search_tree_node(int node_id,char *name)
{
	node_id=dns_search_part_tree_node(node_id,&name);
	return ((*name)==END_CHAR)?node_id:NOT_FIND_TREE_NODE;
}
