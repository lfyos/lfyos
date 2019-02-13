#ifndef OS_KERNEL_TIME
#define OS_KERNEL_TIME

struct kernel_time {
	int v_l,v_m,v_h;
}; 
#define MOST_TIME_LOW			(1024*1024*1024)
#define MOST_TIME_MIDDLE		(1024*1024*1024)
#define MOST_TIME_HIGH			(1024*1024*1024) 
#define MINIMAL_TIME_STEP_LOW			100000
#define MINIMAL_TIME_STEP_MIDDLE		0
#define MINIMAL_TIME_STEP_HIGH			0

#define COPY_TIME(x,y)		\
{				\
	(y).v_l=(x).v_l;	\
	(y).v_m=(x).v_m;	\
	(y).v_h=(x).v_h;	\
}
#define SET_MOST_TIME(x)			\
{						\
	(x).v_l=(MOST_TIME_LOW-1);		\
	(x).v_m=(MOST_TIME_MIDDLE-1);		\
	(x).v_h=(MOST_TIME_HIGH-1);		\
}
#define SET_MINIMAL_STEP(x)			\
{						\
	(x).v_l=(MINIMAL_TIME_STEP_LOW);	\
	(x).v_m=(MINIMAL_TIME_STEP_MIDDLE);	\
	(x).v_h=(MINIMAL_TIME_STEP_HIGH);	\
}
#define TIME_ADD(x,y)						\
{								\
	(y).v_l=(y).v_l+(x).v_l;				\
	(y).v_m=(y).v_m+(((y).v_l)/MOST_TIME_LOW);		\
	(y).v_l=(((y).v_l)%MOST_TIME_LOW);			\
	(y).v_m=(y).v_m+(x).v_m;				\
	(y).v_h=(y).v_h+(((y).v_m)/MOST_TIME_MIDDLE);		\
	(y).v_m=(((y).v_m)%MOST_TIME_MIDDLE);			\
	(y).v_h=(y).v_h+(x).v_h;				\
	(y).v_h=(((y).v_h>=MOST_TIME_HIGH)||((y).v_h<0))	\
		?MOST_TIME_HIGH:(((y).v_h)%MOST_TIME_HIGH);	\
}
#define TIME_INC(x,y)						\
{								\
	(y).v_l=(y).v_l+x;					\
	(y).v_m=(y).v_m+(((y).v_l)/MOST_TIME_LOW);		\
	(y).v_l=(((y).v_l)%MOST_TIME_LOW);			\
	(y).v_h=(y).v_h+(((y).v_m)/MOST_TIME_MIDDLE);		\
	(y).v_h=(((y).v_h>=MOST_TIME_HIGH)||((y).v_h<0))	\
		?MOST_TIME_HIGH:(((y).v_h)%MOST_TIME_HIGH);	\
}
#define TIME_SUB(x,y)						\
{								\
	(y).v_l=(y).v_l-(x).v_l;				\
	for(;;) 						\
		if((y).v_l>=0) 					\
			break;					\
		else{						\
			(y).v_m--;				\
			(y).v_l=(y).v_l+MOST_TIME_LOW;		\
		}						\
	(y).v_m=(y).v_m-(x).v_m;				\
	for(;;) 						\
		if((y).v_m>=0) 					\
			break;					\
		else{						\
			(y).v_h--;				\
			(y).v_m=(y).v_m+MOST_TIME_MIDDLE;	\
		}						\
	(y).v_h=(y).v_h-(x).v_h;				\
}
#define TIME_EQUAL(x,y) 				\
(							\
	  ((x).v_h==(y).v_h)				\
	&&((x).v_m==(y).v_m)				\
	&&((x).v_l==(y).v_l)				\
)
#define TIME_GREAT(x,y) 				\
(							\
	((x).v_h>(y).v_h)?TRUE:				\
	((x).v_h<(y).v_h)?FALSE:			\
	((x).v_m>(y).v_m)?TRUE:				\
	((x).v_m<(y).v_m)?FALSE:			\
	((x).v_l>(y).v_l)?TRUE:				\
	FALSE						\
)
#define TIME_GREAT_EQUAL(x,y) 	(TIME_GREAT(x,y)|TIME_EQUAL(x,y))
#define TIME_NOT_EQUAL(x,y)	(!(TIME_EQUAL(x,y)))
#define TIME_LESS(x,y)		(!(TIME_GREAT_EQUAL(x,y)))
#define TIME_LESS_EQUAL(x,y)	(!(TIME_GREAT(x,y)))

#endif
