#include"../include/os.h"
static int input_port(int address)
{
	int val=0,edx=address&(0xffff);
	call_kernel_register r;
	asm volatile( "inb (%%dx),%%al\n\t" :"+a" (val),"+d"(edx) );
	r.system_call=(-1);
	call_kernel(&r);
	return val;
}
static int kernel_get_key(int *shift)
{
	int x;
	static int my_shift=0; 
	for(;;){
		if((my_shift!=0)&&(my_shift!=1))
			my_shift=0;
		if((input_port(0x64)&0x01)!=0)
			switch(x=input_port(0x60)){
			case 0x2a:
			case 0x36:
				my_shift=1;
				break;
			case 0xaa:
			case 0xb6:
				my_shift=0;
				break;
			default:
				if((x&0x80)==0){
					*shift=my_shift;
					return x;
				}
				break;
			}
	}
}
char kernel_get_char()
{
	int shift; 
	for(;;){
		switch(kernel_get_key(&shift)){
		case 0x29:return (shift)?'~':'`';
		case 0x02:return (shift)?'!':'1';
		case 0x03:return (shift)?'@':'2';
		case 0x04:return (shift)?'#':'3';
		case 0x05:return (shift)?'$':'4';
		case 0x06:return (shift)?'%':'5';
		case 0x07:return (shift)?'^':'6';
		case 0x08:return (shift)?'&':'7';
		case 0x09:return (shift)?'*':'8';
		case 0x0a:return (shift)?'(':'9';
		case 0x0b:return (shift)?')':'0';
		case 0x0c:return (shift)?'_':'-';
		case 0x0d:return (shift)?'+':'=';

		case 0x0f:return (shift)?'\t':'\t';
		case 0x10:return (shift)?'Q':'q';
		case 0x11:return (shift)?'W':'w';
		case 0x12:return (shift)?'E':'e';
		case 0x13:return (shift)?'R':'r';
		case 0x14:return (shift)?'T':'t';
		case 0x15:return (shift)?'Y':'y';
		case 0x16:return (shift)?'U':'u';
		case 0x17:return (shift)?'I':'i';
		case 0x18:return (shift)?'O':'o';
		case 0x19:return (shift)?'P':'p';
		case 0x1a:return (shift)?'{':'[';
		case 0x1b:return (shift)?'}':']';
		case 0x1c:return (shift)?'\n':'\n';

		case 0x1d:return (shift)?'\t':'\t';
		case 0x1e:return (shift)?'A':'a';
		case 0x1f:return (shift)?'S':'s';
		case 0x20:return (shift)?'D':'d';
		case 0x21:return (shift)?'F':'f';
		case 0x22:return (shift)?'G':'g';
		case 0x23:return (shift)?'H':'h';
		case 0x24:return (shift)?'J':'j';
		case 0x25:return (shift)?'K':'k';
		case 0x26:return (shift)?'L':'l';
		case 0x27:return (shift)?':':';';
		case 0x28:return (shift)?'\"':'\'';
		
		case 0x2c:return (shift)?'Z':'z';
		case 0x2d:return (shift)?'X':'x';
		case 0x2e:return (shift)?'C':'c';
		case 0x2f:return (shift)?'V':'v';
		case 0x30:return (shift)?'B':'b';
		case 0x31:return (shift)?'N':'n';
		case 0x32:return (shift)?'M':'m';
		case 0x33:return (shift)?'<':',';
		case 0x34:return (shift)?'>':'.';
		case 0x35:return (shift)?'?':'/';

		case 0x39:return (shift)?' ':' ';

		}
	}
}
int kernel_get_number()
{
	int i,x,s;
	extern void kernel_print_string(char *);
	char c[2];

	for(i=0,x=0,s=1;;i++){
		x=x*10;
		c[0]=kernel_get_char();
		c[1]=(char)0;
		kernel_print_string(c);
		switch(c[0]){
		case '9':x++;
		case '8':x++;
		case '7':x++;
		case '6':x++;
		case '5':x++;
		case '4':x++;
		case '3':x++;
		case '2':x++;
		case '1':x++;
		case '0':
			break;
		case '\n':
			if(i>0)
				return s*(x/10);
			x=x/10;
			i--;
			break;
		case '-':
			if(i==0)
				s=(-s);
			x=x/10;
			i--;
			break;
		default:
			x=x/10;
			i--;
			break;
		}
	}
	return 0;
}
