#include "../../driver/driver.h"
#include "par.h" 
/*
	NOT implement  
		mark_memory_block_unmap();
	for 
		map_memory_block();
*/

#define file_page_table	(memory_body_page_table[memory_body_id].fgt) 
extern int page_dir_modify_flag;

int memory_need_clear_tlb_item(int memory_body_id)
{
	return (-1);
} 
static void clear_tlb(int memory_body_id,int file_window_id,int l_a)
{
	int i;
	struct user_file_information *f_p;
	for(i=0;i<OS_USER_FILE_NUMBER;i++,l_a+=0x20000000)
		if((int)(f_p=current->file_pointer[i])!=0)
		if(f_p->memory_body_id==memory_body_id)
		if(f_p->file_window_id==file_window_id)
			asm volatile( "invlpg (%%eax)\n\t" ::"a"(l_a));
	return;
}
static int memory_set_tlb_item_routine(
	int memory_body_id,int file_window,
	int logic_block_number,int physical_block_id,int right,
	int block_id,int *free_block_id)
{
	int i,*p,*p1,return_value,ii,jj;
	int dir,physical_number,logic_address;
	char *physical_base;
	struct file_page_table_item *pgt;

	if(memory_body_id<0)
		return (-1);
	if(memory_body_id>=memory_body_page_table_number)
		return (-1); 
	if(file_window<0)
		return (-1);
	if(file_window>=(memory_body_page_table[memory_body_id].number))
		return (-1); 
	if(logic_block_number>=(128*1024))
		return (-1);
	dir=logic_block_number/1024;
	if(dir>=file_page_table[file_window].length)
		return (-1); 
	if(physical_block_id<0)
		return (-1);
	physical_base=map_memory_block(
		memory_body_id,physical_block_id);

	logic_address=logic_block_number;
	logic_address=((logic_address<<12)&(0x1ffff000));
	logic_block_number=logic_block_number%1024;

	physical_number=(((int)(physical_base))&(0x3ffff000));
	physical_number=physical_number+(right&(0x7));
	physical_number=(physical_number|(0x01));

	ii=file_page_table[file_window].memory_body_id;
	jj=file_page_table[file_window].physical_block_id;
	if((ii<0)||(jj<0)){ 
		file_page_table[file_window].reference_number=0;
		file_page_table[file_window].memory_body_id
			=memory_body_id;
		file_page_table[file_window].physical_block_id
			=(*free_block_id); 
		pgt=(struct file_page_table_item *)
			map_memory_block(memory_body_id,*free_block_id);
		ii=sizeof(pgt->page_table_block);
		ii=ii/sizeof(pgt->page_table_block[0]);
		for(i=0;i<ii;i++){
			pgt->page_table_block[i]=(-1);
			pgt->item_number[i]=0;
			pgt->reference_number[i]=0;
		}
		(*free_block_id)=(-1); 
		return (-1);
	}
	pgt=(struct file_page_table_item *)(
		map_memory_block(memory_body_id,jj));

	if((i=pgt->page_table_block[dir])>=0)
		p=(int *)(map_memory_block(memory_body_id,i));
	else{
		pgt->item_number[dir]=0;
		pgt->reference_number[dir]=0;
		if((i=(*free_block_id))<0)
			return (-1);
		*free_block_id=(-1);
		pgt->page_table_block[dir]=i; 
		p=(int *)(map_memory_block(memory_body_id,i));
		for(i=0,p1=p;i<1024;i++,p1++)
			*p1=0;
		file_page_table[file_window].reference_number++;
	}
	p=p+logic_block_number;
	return_value=(*p);
	*p=physical_number;
	if((return_value&(0x01))==0){
		pgt->item_number[dir]++;
		return 1;
	}
	clear_tlb(memory_body_id,file_window,logic_address);
	return (-1);
}
static int memory_clear_tlb_item_routine(
	int memory_body_id,int file_window,
	int logic_block_number,int block_id,int *free_block_id)
{
	int ii,jj,*p,v,dir,block_number,logic_address;
	struct file_page_table_item *pgt;
	
	if(memory_body_id<0)
		return (-1);
	if(memory_body_id>=memory_body_page_table_number)
		return (-1); 
	if(file_window<0)
		return (-1);
	if(file_window>=(memory_body_page_table[memory_body_id].number))
		return (-1); 
	if(logic_block_number>=(128*1024))
		return (-1);
	dir=logic_block_number/1024;
	if(dir>=file_page_table[file_window].length)
		return (-1);
	logic_address=logic_block_number;
	logic_address=((logic_address<<12)&(0x1ffff000)); 

	ii=file_page_table[file_window].memory_body_id;
	jj=file_page_table[file_window].physical_block_id;
	if((ii<0)||(jj<0))
		return (-1);
	pgt=(struct file_page_table_item *)(
		map_memory_block(memory_body_id,jj));

	logic_block_number=logic_block_number%1024;
	if((block_number=pgt->page_table_block[dir])<0)
		return (-1);
	p=(int *)(map_memory_block(memory_body_id,block_number));
	p=p+logic_block_number;

	v=(*p);
	*p=0;
	if((v&(0x01))==0)
		return (-1);
	pgt->item_number[dir]--;

	clear_tlb(memory_body_id,file_window,logic_address);

	if((pgt->item_number[dir]>0)||(pgt->reference_number[dir]>0))
		return (v&(0x40));
	page_dir_modify_flag++;
	if((pgt->page_table_block[dir])>=0){
		file_page_table[file_window].reference_number--;
		*free_block_id=pgt->page_table_block[dir];
		free_block_id++;
	}
	pgt->page_table_block[dir]=(-1);
	pgt->item_number[dir]=0;
	pgt->reference_number[dir]=0;

	if(file_page_table[file_window].reference_number>0)
		return (v&(0x40));

	*free_block_id=file_page_table[file_window].physical_block_id;
	free_block_id++;

	file_page_table[file_window].memory_body_id=(-1);
	file_page_table[file_window].physical_block_id=(-1);
	file_page_table[file_window].reference_number=0;

	return (v&(0x40));
}
int memory_set_tlb_item(int memory_body_id,int file_window,
	int logic_block_number,int physical_block_id,int right,
	int block_id,int *free_block_id)
{
	int return_value,eflag; 
	asm volatile(
		"pushfl\n\t"
		"popl %%eax\n\t"
		"cli\n\t"
		:"=a"(eflag)
	); 
	return_value=memory_set_tlb_item_routine(memory_body_id,
		file_window,logic_block_number,physical_block_id,
		right,block_id,free_block_id); 
	asm volatile(
		"pushl %%eax\n\t"
		"popfl\n\t"
		::"a"(eflag)
	); 
	return return_value;
}
int memory_clear_tlb_item(int memory_body_id,int file_window,
	int logic_block_number,int block_id,int *free_block_id)
{
	int return_value,eflag; 
	asm volatile(
		"pushfl\n\t"
		"popl %%eax\n\t"
		"cli\n\t"
		:"=a"(eflag)
	); 
	return_value=memory_clear_tlb_item_routine(memory_body_id,
		file_window,logic_block_number,block_id,free_block_id); 
	asm volatile(
		"pushl %%eax\n\t"
		"popfl\n\t"
		::"a"(eflag)
	); 
	return return_value;
}
