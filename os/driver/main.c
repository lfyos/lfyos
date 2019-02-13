#include"driver.h"
#include"../include/os.h"
#define WRONG_MEMORY_BODY_ID		(-1)
#define NO_DRIVER_MAIN_FUNCTION		(-2)
#define NO_DRIVER_POINTER		(-3) 
static int memory_service_routine(
	int memory_body_id,int command_id)
{
	int ret_val;

	if((memory_body_id<0)&&(memory_body_id>=driver_number))
		return WRONG_MEMORY_BODY_ID; 
	if(((int)(driver[memory_body_id].memory_call_routine))==0)
		return NO_DRIVER_MAIN_FUNCTION;
	if(((int)(driver[memory_body_id].memory_body_pointer))==0)
		return NO_DRIVER_POINTER;
	unlock_device_and_lock_memory(memory_body_id,-1);
	ret_val=driver[memory_body_id].memory_call_routine(command_id,
		get_memory_parameter());
	unlock_memory_and_lock_device(-1);
	return ret_val;
} 
int service_routine(int type_id,int service_id,int par)
{
	int i; 
	if(driver_number<0){
		driver_number=0-driver_number;
		for(i=0;i<driver_number;i++){
			if((int)(driver[i].initialize)!=0)
				driver[i].initialize(&(driver[i]));
			driver[i].initialize=(void(*)(struct driver*))0;
		}
	}
	switch(type_id){
	case MEMORY_SERVICE_TYPE:
		return memory_service_routine(service_id,par);
	default:
		return other_service_routine(service_id,par);
	}
}
