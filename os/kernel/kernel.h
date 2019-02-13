#ifndef OS_KERNEL_KERNEL
#define OS_KERNEL_KERNEL

#include"const.h"
#include"routine_code.h"
#include"system_call_code.h"
#include"capability.h"
#include"time.h"
#include"../arch/arch.h"
#include"file_capability.h"
#include"kernel_struct.h"
#include"file.h"
#include"domain.h"
#include"driver.h"
#include"os_struct.h"
#include"call_kernel.h"
#include"server_type.h"

#ifndef LFYOS_USER_MODE

#include"function.h" 
extern struct os_data os_object; 
#define os (&(os_object)) 

#define current	((struct current_information *)(PARAMETER_POINTER+PAGE_SIZE))

#endif

#endif 
