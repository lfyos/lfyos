#ifndef OS_KERNEL_ARCH
#define OS_KERNEL_ARCH

#define PARAMETER_POINTER		(0xc0001000)

#define CR2_BUF_NUMBER			6
typedef struct{
	void (*ip)();
	int cs;
	int flag;
	char *sp;
	int ss;
	int ds,es,fs,gs;
	char *sp_0; 
	int cr2[CR2_BUF_NUMBER];
}run_point;
extern char *get_sp_0();
extern char *get_sp();
#define IS_USER_MODE(r_p)	((((r_p).cs)&(0x03))==(0x03))
#define DEFAULT_CS		(0x23)
#define DEFAULT_DS		(0x2b)
#define DEFAULT_SP		get_sp()
#define DEFAULT_SP_0		get_sp_0()

#define MODIFY_TO_USER_RUN_POINT(r_p)			\
{							\
	(r_p).flag=(0x000200);				\
	(r_p).sp=DEFAULT_SP;(r_p).sp_0=DEFAULT_SP_0;	\
	(r_p).cs=DEFAULT_CS;(r_p).ds=DEFAULT_DS;(r_p).ss=DEFAULT_DS;\
	(r_p).es=DEFAULT_DS;(r_p).fs=DEFAULT_DS;(r_p).gs=DEFAULT_DS;\
}
#define RESET_USER_RUN_POINT(r_p,start_routine)		\
{							\
	extern void start_routine();			\
	int cr2_i;					\
	MODIFY_TO_USER_RUN_POINT(r_p);			\
	(r_p).ip=start_routine;				\
	for(cr2_i=0;cr2_i<CR2_BUF_NUMBER;cr2_i++)	\
		(r_p).cr2[cr2_i]=(-1);			\
}
#define RESET_RUN_POINT(r_p)	   RESET_USER_RUN_POINT(r_p,lfyos_exit)
#define RESET_START_RUN_POINT(r_p) RESET_USER_RUN_POINT(r_p,lfyos_main)

#define KERNEL_CS		(0x10)
#define KERNEL_DS		(0x18)
#define KERNEL_SP		get_sp_0()
#define KERNEL_SP_0		get_sp_0()

#define RESET_KERNEL_RUN_POINT(r_p,start_routine)	\
{							\
	extern void start_routine();			\
	int cr2_i;					\
	(r_p).cs=KERNEL_CS;(r_p).ds=KERNEL_DS;(r_p).ss=KERNEL_DS;\
	(r_p).es=KERNEL_DS;(r_p).fs=KERNEL_DS;(r_p).gs=KERNEL_DS;\
	(r_p).sp=KERNEL_SP;(r_p).sp_0=KERNEL_SP_0;	\
	(r_p).ip=start_routine;(r_p).flag=(0x000200);	\
	for(cr2_i=0;cr2_i<CR2_BUF_NUMBER;cr2_i++)	\
		(r_p).cr2[cr2_i]=(-1);			\
}
#define INIT_DEVICE_RUN_POINT(r_p)			\
	RESET_KERNEL_RUN_POINT(r_p,start_device_routine)
#define INIT_SERVER_RUN_POINT(r_p)			\
	RESET_KERNEL_RUN_POINT(r_p,start_server)

#define COPY_RUN_POINT(s,d)	copy_data(&(s),&(d),sizeof(run_point))
#define COPY_RUN_POINT_INSTRUCTION_POINTER(s,d)		\
		{(d).ip=(s).ip;(d).cs=(s).cs;} 
extern void prepare_for_save();
extern int prepare_for_restore();
extern char *kernel_get_page_table_base();
extern char *kernel_get_thread_physical_block(int thread_id);
extern void init_tlb();
extern void mark_file_capability_modify(int user_file_id);
extern void mark_physical_block_modify();
extern int operate_spinlock(int *pointer,int value);
extern void restore_parameter(int (*fun)(),int system_call_flag); 

inline static void get_current_time(struct kernel_time *t)
{
	int h,l;
	asm volatile("rdtsc\n\t":"=a"(l),"=d"(h));
	t->v_l=l&0x3fffffff;
	t->v_m=(((l>>30)&0x03)|(h<<2))&0x3fffffff;
	t->v_h=(h>>28)&0x0f;
} 
inline static void clear_data(void *d,int length)
{
	int ecx=length/sizeof(int),*edi=(int *)d;
	asm volatile(
		"cld\n\t"
		"rep stosl\n\t"
		:"+c"(ecx),"+D"(edi) :"a"(0)
	);
	if((ecx=length%sizeof(int))>0)
		asm volatile(
			"cld\n\t"
			"rep stosb\n\t"
			:"+c"(ecx),"+D"(edi) :"a"(0)
		);
}
inline static void copy_data(void *s,void *d,int length)
{
	int ecx=length/sizeof(int),*esi=(int *)s,*edi=(int *)d;
	asm volatile(
		"cld\n\t"
		"rep movsl\n\t"
		:"+c"(ecx),"+S"(esi),"+D"(edi)
	);
	if((ecx=length%sizeof(int))>0)
		asm volatile(
			"cld\n\t"
			"rep movsb\n\t"
			:"+c"(ecx),"+S"(esi),"+D"(edi)
		);
}
inline static void start_all_processors() {return;}
inline static void remind_physical_cpu_do_switch(int physical_cpu_id)
	{return;}
inline static int get_physical_cpu() {return 0;}

#endif
