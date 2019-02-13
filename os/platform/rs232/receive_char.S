.global com1_receive_char,com2_receive_char 
com1_receive_char:
	mov $(0x3fd),%dx
	inb %dx,%al
	movb %al,%ah
	andb $(0x01),%ah
	je com1_receive_char
	andb $(0x1e),%al
	jne com1_receive_char
	mov $(0x3f8),%dx
	movl $(0x00),%eax
	in %dx,%al 
	ret 
com2_receive_char:
	mov $(0x2fd),%dx
	inb %dx,%al
	movb %al,%ah
	andb $(0x01),%ah
	je com2_receive_char
	andb $(0x1e),%al
	jne com2_receive_char
	mov $(0x2f8),%dx
	movl $(0x00),%eax
	in %dx,%al 
	ret
