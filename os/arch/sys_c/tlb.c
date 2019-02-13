#include "../../driver/driver.h"
#include "par.h"
/*
	NOT implement  
		mark_memory_block_unmap();
	for 
		map_memory_block();
*/
static int pointer,tlb_flag=1;
static int buf[CR2_BUF_NUMBER],*reference[CR2_BUF_NUMBER];
int page_dir_modify_flag=1,physical_block_modify=1;

int *add_tlb_buf(int dir,int memory_body_id,
	int system_file,int file_dir,int right)
{ 
	int i,j,*p;
	struct file_page_table *file_page_table;
	struct file_page_table_item *pgt;

	if((dir<0)||(dir>=3*256))
		return (int *)0;
	i=buf[pointer];
	if((i>=0)&&(i<3*256)){
		buf[pointer]=(-1);
		page_dir[i]=0;
		tlb_flag++;
	}
	file_page_table=memory_body_page_table[memory_body_id].fgt;
	i=file_page_table[system_file].memory_body_id;
	j=file_page_table[system_file].physical_block_id;
	if((i<0)||(j<0))
		return (int)0;
	pgt=(struct file_page_table_item *)
		map_memory_block(memory_body_id,j);
	if((j=pgt->page_table_block[file_dir])<0)
		return (int *)0;
	p=(int *)(map_memory_block(memory_body_id,j));
	j=(0x3ffff000)&((int)p);
	buf[pointer]=dir;
	page_dir[dir]=(j|right); 
	reference[pointer]=(&(pgt->reference_number[file_dir]));
	(*(reference[pointer]))++;
	pointer=((1+pointer)%CR2_BUF_NUMBER);

	return p;
}
void init_tlb()
{
	int i,j;
	struct file_page_table *file_page_table;
	struct system_page_table {
		int add[1024];
	}*system_page_table=(struct system_page_table *)
		kernel_get_page_table_base();
	for(i=0;i<1024;i++){
		if(i<(3*256))
			page_dir[i]=0;
		else{
			j=(int)&(system_page_table[i-(3*256)]);
			j=j&(0x3ffff000);
			page_dir[i]=(j|(0x07));
		}
	};
	for(j=0;j<memory_body_page_table_number;j++){
		file_page_table=memory_body_page_table[j].fgt;
		for(i=0;i<(memory_body_page_table[j].number);i++){
			file_page_table[i].length=0;
			file_page_table[i].memory_body_id=(-1);
			file_page_table[i].physical_block_id=(-1);
			file_page_table[i].reference_number=0;
			RESET_CAPABILITY(
				file_page_table[i].file_capability);
		}
	}
	for(i=0;i<CR2_BUF_NUMBER;i++)
		buf[i]=-1;
	pointer=0;
	page_dir_modify_flag=1;
	physical_block_modify=1;
	return;
}
static void notify_thread_switch()
{
	int i,j;
	for(i=0;i<CR2_BUF_NUMBER;i++){
		j=buf[i];
		if((j>=0)&&(j<3*256)){
			page_dir[j]=0;
			(*(reference[i]))--;
			tlb_flag++;
		}
		buf[i]=-1;
	};
	for(i=0;i<CR2_BUF_NUMBER;i++)
		page_fault_process(1,current->env->point.cr2[i],0);
	pointer=0; 
	return ;
}
static void notify_physical_block_switch()
{
	int body_id,p,*a_p=(int *)kernel_get_page_table_base();

	body_id=current->rt->physical_block.stack_memory_id;
	if((body_id>=0)&&(body_id<driver_number))
		p=(int)(map_memory_block(body_id,
			current->rt->physical_block.stack_block_id));
	else
		p=(int)(kernel_get_thread_physical_block(
			current->thread_id));
	p=(p&(0x3ffff000))|(0x07);
	*(0+a_p)=p;

	body_id=current->rt->physical_block.memory_id;
	if((body_id>=0)&&(body_id<driver_number))
		p=(int)(map_memory_block(body_id,
			current->rt->physical_block.block_id));
	else
		p=PAGE_SIZE+(int)(kernel_get_thread_physical_block(
			current->thread_id));
	p=(p&(0x3ffff000))|(0x07);
	*(1+a_p)=p;

	if(tlb_flag==0)
		asm volatile(
			"invlpg 0xc0000000\n\t"
			"invlpg 0xc0001000\n\t"
		);
	return ;
}
void prepare_for_save()
{
	return;
}
int prepare_for_restore()
{
	if(physical_block_modify!=0){
		physical_block_modify=0;
		notify_physical_block_switch();
	}
	if(page_dir_modify_flag!=0){
		page_dir_modify_flag=0;
		notify_thread_switch();
	}
	if(tlb_flag!=0){
		tlb_flag=0;
		asm volatile(
			"movl %%cr3,%%eax\n\t" 
			"movl %%eax,%%cr3\n\t"
			:::"%eax"
		);
	}
	return TRUE;
}
void mark_file_capability_modify(int user_file_id)
{
	page_dir_modify_flag=1;
}
void mark_physical_block_modify()
{
	physical_block_modify=1;
}
