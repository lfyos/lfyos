.global gdt_table,gdt_descr,set_gdt_item
set_gdt_item:
	cmpl $(0x00),0x04(%esp)
	jne 2f
	movl $(__end_rodata-0xc0000000),%eax
	shrl $(0x0c),%eax
	andl $(0xffff),%eax
	movl %eax,%ebx
	movl $(__end_rodata-0xc0000000),%eax
	andl $(0xfff),%eax
	je 1f
	incl %ebx
1:
	movl %ebx,(0x20+gdt_table)
	movl $(0x00ccfa00),(0x24+gdt_table)
	movl $(0x00000001),(0x28+gdt_table)
	movl $(0x00ccf200),(0x2c+gdt_table)
	ret
2:
	movl $(0x0000ffff),(0x20+gdt_table)
	movl $(0x00cffa00),(0x24+gdt_table)
	movl $(0x0000ffff),(0x28+gdt_table)
	movl $(0x00cff200),(0x2c+gdt_table)
	ret 
.align 16
	.word 0
gdt_descr:
	.word gdt_table_end-gdt_table
	.long gdt_table
gdt_descr_end:
.align 16
gdt_table:
	.quad 0x0000000000000000/* NULL descriptor */
	.quad 0x0000000000000000/* task tss */ 
	.quad 0x00cf9a000000ffff/* 0x010 kernel 4GB code at 0x00000000*/
	.quad 0x00cf92000000ffff/* 0x018 kernel 4GB data at 0x00000000*/ 
	.quad 0x00cffa000000ffff/* 0x023 kernel 4GB code at 0x00000000*/
	.quad 0x00cff2000000ffff/* 0x02b kernel 4GB data at 0x00000000*/ 
	.quad 0x00c1fa000000ffff/* 0x033 user 512MB code at 0x00000000*/
	.quad 0x00c1f2000000ffff/* 0x03b user 512MB data at 0x00000000*/ 
	.quad 0x20c1fa000000ffff/* 0x043 user 512MB code at 0x20000000*/
	.quad 0x20c1f2000000ffff/* 0x04b user 512MB data at 0x20000000*/ 
	.quad 0x40c1fa000000ffff/* 0x053 user 512MB code at 0x40000000*/
	.quad 0x40c1f2000000ffff/* 0x05b user 512MB data at 0x40000000*/ 
	.quad 0x60c1fa000000ffff/* 0x063 user 512MB code at 0x60000000*/
	.quad 0x60c1f2000000ffff/* 0x06b user 512MB data at 0x60000000*/ 
	.quad 0x80c1fa000000ffff/* 0x073 user 512MB code at 0x80000000*/
	.quad 0x80c1f2000000ffff/* 0x07b user 512MB data at 0x80000000*/ 
	.quad 0xa0c1fa000000ffff/* 0x083 user 512MB code at 0xa0000000*/
	.quad 0xa0c1f2000000ffff/* 0x08b user 512MB data at 0xa0000000*/ 
	.quad 0xc0c0fa0000000001/* 0x093 parameter code at 0xc0000000*/
	.quad 0xc0c0f20000000001/* 0x09b parameter data at 0xc0000000*/ 
gdt_table_end:
