#include "../driver/driver.h"
extern void kernel_print_number(int );
extern void kernel_print_string(char *);
extern char kernel_get_char();
extern int kernel_get_number(); 
int other_service_routine(int service_id,int par)
{
	asm volatile("sti\n\t"); 
	switch(service_id){
	case 1:
		kernel_print_number(par);
		break;
	case 2:
		kernel_print_string((char *)(par));
		break;
	case 3:
		service_id=(int)kernel_get_char();
		break;
	case 4:
		service_id=kernel_get_number();
		break;
	case 5:
		{
			extern int thread_id;
			thread_id=par;
		}
		break;
	case 6:
		{
			extern void setup_8259();
			extern void setup_8254(int timer_length);
			setup_8259();
			setup_8254(par);
		}
		break;
	}
	asm volatile("cli\n\t");
	return service_id;
}
