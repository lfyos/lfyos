.global startup_32,stext
.text
stext:
startup_32:
	cli
	cld
	movl %cs,%eax
	andl $(0xfffffff0),%eax
	addl $(0x08),%eax
	movl %eax,%ds
	movl %eax,%es
	movl %eax,%ss
	movl %eax,%fs
	movl %eax,%gs

	call kernel_get_page_table_base
	andl $(0x3fffffff),%eax
	movl %eax,%edi
	movl $(256*1024),%ecx
	movl $007,%eax		/*"007" doesn't mean with right to kill,
				 but PRESENT+RW+USER */
1:	stosl
	add $(0x1000),%eax
	decl %ecx
	jne 1b

	movl $(page_dir-0xc0000000),%edi 
	xorl %eax,%eax
2:	stosl
	cmp $(page_dir-0xc0000000+0x1000),%edi
	jne 2b
	
	call kernel_get_page_table_base
	andl $(0x3ffff000),%eax
	orl $(0x07),%eax
	
	movl $(page_dir-0xc0000000),%edi 
	movl $(0x100),%ecx
1:
	movl %eax,0x000(%edi)
	movl %eax,0xc00(%edi)
	addl $(0x04),%edi
	addl $(0x1000),%eax
	decl %ecx
	jne 1b

	movl $page_dir-0xc0000000,%eax
	movl %eax,%cr3		/* set the page table pointer.. */
	movl %cr0,%eax
	orl $0x80000000,%eax
	movl %eax,%cr0		/* ..and set paging (PG) bit */
	jmp 1f			/* flush the prefetch-queue */
1:
	movl $1f,%eax
	jmp *%eax		/* make sure eip is relocated */
1: 
	movl %cr0,%eax		# 486 or better
	andl $0x80000011,%eax	# Save PG,PE,ET
	orl $0x50022,%eax	# set AM, WP, NE and MP
	movl %eax,%cr0 
check_x87:
	clts
	fninit
	fstsw %ax

	lgdt gdt_descr

	ljmp $(0x10),$(1f)
1:
	movl $(0x18),%eax	#reload all the segment registers
	movl %eax,%ds		# after changing gdt.
	movl %eax,%es
	movl %eax,%fs
	movl %eax,%gs

	lss kernel_stack_descr,%esp	# Load processor stack
	xorl %eax,%eax
	lldt %ax
	
	cli

	call init_tlb
	call setup_idt
	call setup_task
	pushl $(0x00)
	call set_gdt_item
	popl %eax

	movl %cr3,%eax
	movl %eax,%cr3

	pushl $(0x01)
	call os_init

/*	never return here	*/

.global kernel_stack_descr,page_dir

.align 16
kernel_stack_descr:
	.long 0xc00a0000
	.long 0x18
kernel_stack_descr_end:

.org 0x1000
.align 0x1000 
page_dir:

.org page_dir+0x1000
page_dir_end:
