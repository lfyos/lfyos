#include"../kernel/kernel.h" 
#define base	((char *)(0xc00b8000))
int thread_id=-1; 
void kernel_print_string(char *p)
{
	static int i=100,j=200;
	int ii,jj;
	char *s,*d;

	if((thread_id>=0)&&(thread_id!=(current->thread_id)))
		return; 
	for(;;p++){
		if((i<0)||(j<0))
			{i=24;j=0;}
		if(j>=80)
			{i++;j=0;};
		if(i>=25){
			for(i=24,j=0,ii=0;ii<23;ii++)
				for(jj=0;jj<80;jj++){
					d=2*(80*ii+jj)+base;
					s=d+160;*d=(*s);
				}; 
			for(jj=0;jj<80;jj++){
				d=2*(80*23+jj)+base;s=d+160;
				*d=(*s);*s=' ';
			}
		}
		switch(*p){
		case '\0':
			return;
		case '\n':
			for(ii=i,jj=j,i++,j=0;jj<80;jj++)
				*(2*(80*ii+jj)+base)=' ';
			break;
		default:
			ii=i;jj=j;j++;*(2*(80*ii+jj)+base)=(*p);
			break;
		}
	}
	return;
}
void kernel_print_number(int number)
{
	int i,x;
	char p[9]; 
	for(i=7;i>=0;i--){
		x=number&0xf;
		number=((number>>4)&(0x0fffffff)); 
		if((x>=0)&&(x<=9))
			p[i]=(char)(x+(int)'0');
		else
			p[i]=(char)(x-10+(int)'A');
	}
	p[8]='\0';
	kernel_print_string(p);
	return;
}
