#ifndef OS_SERVICE_LIBRARY
#define OS_SERVICE_LIBRARY 
static inline void set_timer_length(int timer_length)
{
	call_kernel_service(-1,6,timer_length);
}
static inline void set_print_thread(int id)	
{
	call_kernel_service(-1,5,id);
} 
static inline int get_number() 			
{
	return call_kernel_service(-1,4,0);
}
static inline char get_char() 			
{
	return (char)call_kernel_service(-1,3,0);
}
static inline void print_string(char *str)
{
	call_kernel_service(-1,2,(int)str);
}
static inline void print_number(int num) 	
{	
	call_kernel_service(-1,1,num);
} 
static inline void print(char *str,int number) 	
{
	print_string(str);print_number(number);
}
#endif
