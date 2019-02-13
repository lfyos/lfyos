#include "../../kernel/system_call_code.h" 
.global setup_idt
fatal_interruption_routine:
idle_interruption_routine:
	iret 
setup_idt:			/*this function setup idt table */
	lea idt_table,%edi
	xorl %ecx,%ecx
1:
	andl $3,(%edi)
	shll $13,(%edi)
	movl 4(%edi),%edx
	movl $((0x10)<< 16),%eax
	movw %dx,%ax		/* selector = 0x0010 = cs */
	andl $0xffff0000,%edx
	orl  $0x00008E00,%edx	/* interrupt gate - dpl=0, present */
	orl  (%edi),%edx

	movl %eax,(%edi)
	movl %edx,4(%edi)
	addl $8,%edi
	incl %ecx

	lea idt_table_end,%eax
	cmpl %edi,%eax
	jne 1b

	lea idle_interruption_routine,%edx
	movl $((0x10)<< 16),%eax
	movw %dx,%ax		/* selector = 0x0010 = cs */
	andl $0xffff0000,%edx
	orl  $0x00008E00,%edx	/* interrupt gate - dpl=0, present */ 
1:
	cmpl $256,%ecx
	je 2f
	movl %eax,(%edi)
	movl %edx,4(%edi)
	addl $8,%edi
	incl %ecx
	jmp 1b
2: 
	lidt idt_descr

	ret
.align 16
	.word 0
idt_descr:
	.word 256*8-1
	.long idt_table
.align 16 
idt_table:
/* 	first value is interruption gate dpl(only 0-3)
	second value is interruption  process function  
	setup_idt function turn all items in this table 
	into interruption gates 
*/
	.long 0,fatal_interruption_routine	/*00:div*/
	.long 0,fatal_interruption_routine	/*01:debug*/
	.long 0,fatal_interruption_routine	/*02:reserved */
	.long 0,fatal_interruption_routine	/*03:break point*/
	.long 0,fatal_interruption_routine	/*04:overflow */
	.long 0,fatal_interruption_routine	/*05:bound*/
	.long 0,fatal_interruption_routine	/*06:invalid operand*/
	.long 0,fatal_interruption_routine	/*07:can not use coprocessor */
	.long 0,fatal_interruption_routine	/*08:exception occur again*/
	.long 0,fatal_interruption_routine	/*09:coprocessor segment error*/
	.long 0,fatal_interruption_routine	/*0a:invalid tss*/
	.long 0,fatal_interruption_routine	/*0b:segment do not exist*/
	.long 0,fatal_interruption_routine	/*0c:stack fault*/
	.long 0,fatal_interruption_routine	/*0d:general protection*/
	.long 0,page_fault_interruption		/*0e:page fault protection*/
	.long 0,fatal_interruption_routine	/*0f:reserved*/

	.long 0,fatal_interruption_routine	/*10:coprocessor exception*/
	.long 0,fatal_interruption_routine	/*11:align check */
	.long 0,fatal_interruption_routine	/*12:*/
	.long 0,fatal_interruption_routine	/*13:*/
	.long 0,fatal_interruption_routine	/*14:*/
	.long 0,fatal_interruption_routine	/*15:*/
	.long 0,fatal_interruption_routine	/*16:*/
	.long 0,fatal_interruption_routine	/*17:*/
	.long 0,fatal_interruption_routine	/*18:*/
	.long 0,fatal_interruption_routine	/*19:*/
	.long 0,fatal_interruption_routine	/*1a:*/
	.long 0,fatal_interruption_routine	/*1b:*/
	.long 0,fatal_interruption_routine	/*1c:*/
	.long 0,fatal_interruption_routine	/*1d:*/
	.long 0,fatal_interruption_routine	/*1e:*/
	.long 0,fatal_interruption_routine	/*1f:*/

	.long 0,timer_interruption_routine	/*20:*/
	.long 0,idle_interruption_routine	/*21:*/
	.long 0,idle_interruption_routine	/*22:*/
	.long 0,idle_interruption_routine	/*23:*/
	.long 0,idle_interruption_routine	/*24:*/
	.long 0,idle_interruption_routine	/*25:*/
	.long 0,idle_interruption_routine	/*26:*/
	.long 0,idle_interruption_routine	
			/*27: frequently generate interruption,why?*/ 
	.long 0,idle_interruption_routine	/*28:*/
	.long 0,idle_interruption_routine	/*29:*/
	.long 0,idle_interruption_routine	/*2a:*/
	.long 0,idle_interruption_routine	/*2b:*/
	.long 0,idle_interruption_routine	/*2c:*/
	.long 0,idle_interruption_routine	/*2d:*/
	.long 0,idle_interruption_routine	/*2e:*/
	.long 0,idle_interruption_routine	/*2f:*/

	.long 0,idle_interruption_routine	/*30:*/
	.long 0,idle_interruption_routine	/*31:*/
	.long 0,idle_interruption_routine	/*32:*/
	.long 0,idle_interruption_routine	/*33:*/
	.long 0,idle_interruption_routine	/*34:*/
	.long 0,idle_interruption_routine	/*35:*/
	.long 0,idle_interruption_routine	/*36:*/
	.long 0,idle_interruption_routine	/*37:*/
	.long 0,idle_interruption_routine	/*38:*/
	.long 0,idle_interruption_routine	/*39:*/
	.long 0,idle_interruption_routine	/*3a:*/
	.long 0,idle_interruption_routine	/*3b:*/
	.long 0,idle_interruption_routine	/*3c:*/
	.long 0,idle_interruption_routine	/*3d:*/
	.long 0,idle_interruption_routine	/*3e:*/
	.long 0,idle_interruption_routine	/*3f:*/

	.long 0,idle_interruption_routine	/*40:*/
	.long 0,idle_interruption_routine	/*41:*/
	.long 0,idle_interruption_routine	/*42:*/
	.long 0,idle_interruption_routine	/*43:*/
	.long 0,idle_interruption_routine	/*44:*/
	.long 0,idle_interruption_routine	/*45:*/
	.long 0,idle_interruption_routine	/*46:*/
	.long 0,idle_interruption_routine	/*47:*/
	.long 0,idle_interruption_routine	/*48:*/
	.long 0,idle_interruption_routine	/*49:*/
	.long 0,idle_interruption_routine	/*4a:*/
	.long 0,idle_interruption_routine	/*4b:*/
	.long 0,idle_interruption_routine	/*4c:*/
	.long 0,idle_interruption_routine	/*4d:*/
	.long 0,idle_interruption_routine	/*4e:*/
	.long 0,idle_interruption_routine	/*4f:*/

	.long 0,idle_interruption_routine	/*50:*/
	.long 0,idle_interruption_routine	/*51:*/
	.long 0,idle_interruption_routine	/*52:*/
	.long 0,idle_interruption_routine	/*53:*/
	.long 0,idle_interruption_routine	/*54:*/
	.long 0,idle_interruption_routine	/*55:*/
	.long 0,idle_interruption_routine	/*56:*/
	.long 0,idle_interruption_routine	/*57:*/
	.long 0,idle_interruption_routine	/*58:*/
	.long 0,idle_interruption_routine	/*59:*/
	.long 0,idle_interruption_routine	/*5a:*/
	.long 0,idle_interruption_routine	/*5b:*/
	.long 0,idle_interruption_routine	/*5c:*/
	.long 0,idle_interruption_routine	/*5d:*/
	.long 0,idle_interruption_routine	/*5e:*/
	.long 0,idle_interruption_routine	/*5f:*/

	.long 0,idle_interruption_routine	/*60:*/
	.long 0,idle_interruption_routine	/*61:*/
	.long 0,idle_interruption_routine	/*62:*/
	.long 0,idle_interruption_routine	/*63:*/
	.long 0,idle_interruption_routine	/*64:*/
	.long 0,idle_interruption_routine	/*65:*/
	.long 0,idle_interruption_routine	/*66:*/
	.long 0,idle_interruption_routine	/*67:*/
	.long 0,idle_interruption_routine	/*68:*/
	.long 0,idle_interruption_routine	/*69:*/
	.long 0,idle_interruption_routine	/*6a:*/
	.long 0,idle_interruption_routine	/*6b:*/
	.long 0,idle_interruption_routine	/*6c:*/
	.long 0,idle_interruption_routine	/*6d:*/
	.long 0,idle_interruption_routine	/*6e:*/
	.long 0,idle_interruption_routine	/*6f:*/

	.long 3,system_call_routine		/*70:*/
	.long 3,service_call_routine		/*71:*/
	.long 0,idle_interruption_routine	/*72:*/
	.long 0,idle_interruption_routine	/*73:*/
	.long 0,idle_interruption_routine	/*74:*/
	.long 0,idle_interruption_routine	/*75:*/
	.long 0,idle_interruption_routine	/*76:*/
	.long 0,idle_interruption_routine	/*77:*/
	.long 0,idle_interruption_routine	/*78:*/
	.long 0,idle_interruption_routine	/*79:*/
	.long 0,idle_interruption_routine	/*7a:*/
	.long 0,idle_interruption_routine	/*7b:*/
	.long 0,idle_interruption_routine	/*7c:*/
	.long 0,idle_interruption_routine	/*7d:*/
	.long 0,idle_interruption_routine	/*7e:*/
	.long 0,idle_interruption_routine	/*7f:*/
idt_table_end:
	.org idt_table+256*8-1
	.long 0
