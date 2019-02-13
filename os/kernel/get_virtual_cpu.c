#include "kernel.h"
int get_virtual_cpu(int switch_virtual_cpu_flag)
{
	static int current_cpu=0;
	if(switch_virtual_cpu_flag)
		current_cpu++;
	return (current_cpu=(current_cpu<0)?0:(current_cpu%OS_CPU_NUMBER));
}
