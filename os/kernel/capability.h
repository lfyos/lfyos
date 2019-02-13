#ifndef OS_KERNEL_CAPABILITY
#define OS_KERNEL_CAPABILITY

typedef struct {
	int c1,c2,c3,c4;
}CAPABILITY;
#define COMPARE_CAPABILITY(x,y)					\
	(((x).c1==(y).c1)&&((x).c2==(y).c2)			\
	&&((x).c3==(y).c3)&&((x).c4==(y).c4))
#define KERNEL_COMPARE_CAPABILITY(x,y)				\
	(COMPARE_CAPABILITY((x),(y))				\
	||COMPARE_CAPABILITY((os->system_capability),(y)))
#define COPY_CAPABILITY(x,y) 					\
	{(y).c1=(x).c1;(y).c2=(x).c2;(y).c3=(x).c3;(y).c4=(x).c4;}
#define RESET_CAPABILITY(x)	{(x).c1=0;(x).c2=0;(x).c3=0;(x).c4=0;}

#endif
