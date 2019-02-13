#include"../../driver/driver.h" 
#include "par.h" 
int page_fault_process(int flag,int cr2,int err_code)
{
	int i,x,y,*p; 
	int file=((cr2>>29)&(0x07)),offset=(cr2&(0x1fffffff));
	int dir=((cr2>>22)&(0x3ff)),system_dir=(cr2>>22)&(0x7f);
	struct user_file_information *f;
	CAPABILITY *c1,*c2;

	if((dir>=(3*256))||(file<0)||(file>=OS_USER_FILE_NUMBER))
		return 0;
	if((int)(f=current->file_pointer[file])==0)
		return 0;
	if(((err_code&0x02)==2)&&(!IS_READ_WRITE_RIGHT(f->right)))
		return 0;
	if(flag<=0)
		for(i=0,x=cr2;i<CR2_BUF_NUMBER;i++,x=y){
			y=current->env->point.cr2[i];
			current->env->point.cr2[i]=x;
			if(((0xffc00000)&cr2)==((0xffc00000)&y))
				break;
		}
	for(;;){
		if(((x=page_dir[dir])&(0x01))!=0)
			p=(int *)((0xfffff000&x)|0xc0000000);
		else{ 
			x=memory_body_page_table_number;
			if(f->memory_body_id>=x)
				return 0; 
			x=f->memory_body_id;
			x=memory_body_page_table[x].number; 
			if(f->file_window_id>=x)
				return 0;
			x=memory_body_page_table[f->memory_body_id].
				fgt[f->file_window_id].length;
			if(system_dir>=x)
				return 0;
			c1=&(memory_body_page_table[f->memory_body_id].
				fgt[f->file_window_id].file_capability);
			c2=&(f->file_capability);
			if(!KERNEL_COMPARE_CAPABILITY(*c1,*c2))
				return 0;
			p=add_tlb_buf(dir,f->memory_body_id,
				f->file_window_id,system_dir,f->right);
		}
		if(flag>0)
			return 1;
		if(((int)p)!=0){
			p=p+((offset>>12)&(0x3ff));
			if(((*p)&0x01)!=0){
				if((err_code&0x01)==0){
					if((err_code&0x02)!=2)
						return 1;
					if(((*p)&0x02)==2)
						return 1;
				}else if(((*p)&0x02)==2)
					return 1;
			}
		}
		if(memory_main_map_process(f->memory_body_id,
			f->file_window_id,offset,offset+(f->map_length),
			((err_code&0x02)!=2)?TRUE:FALSE,
			&(f->file_capability))<0)
				return 0;
	}
} 
