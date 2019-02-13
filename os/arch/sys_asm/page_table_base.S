.global kernel_get_page_table_base
kernel_get_page_table_base:
	movl $(_end+0x1000),%eax
	andl $(0xfffff000),%eax
	ret
