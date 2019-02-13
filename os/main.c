#include"include/os.h"
#include"platform/include/service.h" 
#include"dns/dns.h"

struct os_data os_object; 
struct dns_struct my_dns;

void lfyos_main()
{
	extern void test_main(); 
	set_timer_length(17);
	test_main();
}
void lfyos_exit()
{
	call_kernel_register r; 
	for(;;){
		r.system_call=THREAD_RETURN_SYSTEM_CALL;
		r.r1=TRUE;
		call_kernel(&r);
	}
}
