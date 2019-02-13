.global setup_task,tss_esp0
setup_task:		/*this function setup tss, and put it into GDT*/
	call get_sp_0
	movl %eax,tss_esp0
	movl $(0x00),tss_ss1
	movl $(0x00),tss_ss2
	movl $(page_dir),tss_cr3
	andl $(0x3fffffff),tss_cr3

	lea gdt_table,%edi
	lea tss,%eax
	shll $(0x10),%eax
	addl $(tss_end-tss),%eax
	lea tss,%edx
	movl %edx,%ecx
	shrl $(0x10),%ecx
	movb %cl,%dl
	andl $(0xff0000ff),%edx
	orl  $(0x00008900),%edx
	movl %eax,1*8(%edi)
	movl %edx,4+1*8(%edi)
	
	lgdt gdt_descr
	movw $(0x08),%ax
	ltr %ax

	ret

.align 16
tss:
	.long 0 				/*00:	(0,LINK)*/
tss_esp0:
	.long 0					/*04:	ESP0  	*/
tss_ss0:
	.long 0x18 				/*08:	(0,SS0)	*/
tss_esp1:
	.long 0 				/*0C:	ESP1	*/
tss_ss1:
	.long 0 				/*10:	(0,SS1)	*/
tss_esp2:
	.long 0 				/*14:	ESP2	*/
tss_ss2:
	.long 0 				/*18:	(0,SS2)	*/
tss_cr3:
	.long 0 				/*1C:	CR3	*/
tss_reg:
	.long 0 				/*20:	EIP	*/
	.long 0 				/*24:	EEFLAG	*/
	.long 0 				/*28:	EAX	*/
	.long 0 				/*2C:	ECX	*/
	.long 0 				/*30:	EDX	*/
	.long 0 				/*34:	EBX	*/
	.long 0					/*38:	ESP	*/
	.long 0 				/*3C:	EBP	*/
	.long 0 				/*40:	ESI	*/
	.long 0 				/*44:	EDI	*/
	.long 0x2b				/*48:	(0,ES)	*/
	.long 0x23 				/*4C:	(0,CS)	*/
	.long 0x2b 				/*50:	(0,SS)	*/
	.long 0x2b				/*54:	(0,DS)	*/
	.long 0x2b 				/*58:	(0,FS)	*/
	.long 0x2b 				/*5C:	(0,GS)	*/
	.long 0 				/*60:	(0,LDT)	*/
	.long 0 				/*64:	(IObitmap, 0T*/
	.org tss+0x70
tss_end:
