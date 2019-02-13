.global setup_8259
setup_8259:
	movb $(0x11),%al
	outb %al,$(0x20)
	movb $(0x20),%al
	outb %al,$(0x21)
	movb $(0x04),%al
	outb %al,$(0x21)
	movb $(0x03),%al
	outb %al,$(0x21)

	movb $(0xfe),%al
	outb %al,$(0x21)
	
	movb $(0x11),%al
	outb %al,$(0xa0)
	movb $(0x70),%al
	outb %al,$(0xa1)
	movb $(0x02),%al
	outb %al,$(0xa1)
	movb $(0x01),%al
	outb %al,$(0xa1)
	
	movb $(0x04),%al
	outb %al,$(0xa1)

	ret
