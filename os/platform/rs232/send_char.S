.global com1_send_char,com2_send_char 
com1_send_char:
	pushfl
	cli
1:
	mov $(0x3fd),%dx
	inb %dx,%al
	andb $(0x20),%al
	je 1b
	movl 0x08(%esp),%eax
	mov $(0x3f8),%dx
	outb %al,%dx
	popfl
	ret 
com2_send_char:
	pushfl
	cli
1:
	mov $(0x2fd),%dx
	inb %dx,%al
	andb $(0x20),%al
	je 1b
	movl 0x08(%esp),%eax
	mov $(0x2f8),%dx
	outb %al,%dx
	popfl
	ret
