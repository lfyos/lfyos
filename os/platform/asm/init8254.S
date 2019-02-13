.global setup_8254
setup_8254:
	movb $(0x34),%al
	outb %al,$(0x43)
	movl 0x04(%esp),%eax
	outb %al,$(0x40)
	movb %ah,%al
	outb %al,$(0x40)
	ret
