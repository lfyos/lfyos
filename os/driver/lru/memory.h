#ifndef OS_LFY_MEMORY
#define OS_LFY_MEMORY 

#include"memory_call_code.h"
#include"memory_domain.h"
#include"memory_struct.h"
#include"call_memory.h" 

#ifndef LFYOS_USER_MODE

#define MOUNT_BLOCK(BN,B,FRONT,BACK,POINT)			\
{								\
	if((POINT)<0)						\
		{(POINT)=(BN);(B)->FRONT=(BN);(B)->BACK=(BN);}	\
	else{							\
		front=(POINT);b_f=front+(memory_body->memory_block);\
		back=b_f->BACK;b_b=back+(memory_body->memory_block);\
		(B)->FRONT=front; 	(B)->BACK=back;		\
		b_f->BACK=(BN);		b_b->FRONT=(BN);	\
	}							\
} 
#define UN_MOUNT_BLOCK(BN,B,FRONT,BACK,POINT)		\
{							\
	if((front=(B)->FRONT)==(BN))			\
		{(POINT)=(-1);}				\
	else{						\
		back=(B)->BACK;				\
		if((POINT)==(BN))			\
			(POINT)=front;			\
		b_f=front+(memory_body->memory_block);	\
		b_b=back +(memory_body->memory_block);	\
		b_f->BACK=back;	b_b->FRONT=front;	\
	}						\
}
#include"memory_function.h"
#define memory_body	((struct memory_body_struct *)	\
	driver[current->rt->current_memory_body_id].memory_body_pointer)
#endif

#endif
