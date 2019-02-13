#ifndef OS_SYSTEM_CALL
#define OS_SYSTEM_CALL

#define LFYOS_USER_MODE
#include "../driver/driver.h"
#include "../arch/include/interface.h"

#define MEMORY_SERVICE_TYPE	0 
static inline int call_memory(int body_id,int arg)
{
	return call_kernel_service(MEMORY_SERVICE_TYPE,body_id,arg);
}
#endif
