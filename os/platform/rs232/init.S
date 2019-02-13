.global rs232_init 
rs232_init:
init_com1:
	mov $(0x3FB),%dx
	mov $(0x80),%al
	out %al,%dx
	mov $(0x3F8),%dx
	mov $(0x0c),%al
	out %al,%dx
	mov $(0x3F9),%dx
	mov $(0x00),%al
	out %al,%dx
	mov $(0x3Fb),%dx
	mov $(0x0b),%al
	out %al,%dx
	mov $(0x3Fc),%dx
	mov $(0x0b),%al
	out %al,%dx
	mov $(0x3F9),%dx
	mov $(0x07),%al
	out %al,%dx
init_com2:
	mov $(0x2FB),%dx
	mov $(0x80),%al
	out %al,%dx
	mov $(0x2F8),%dx
	mov $(0x0c),%al
	out %al,%dx
	mov $(0x2F9),%dx
	mov $(0x00),%al
	out %al,%dx
	mov $(0x2Fb),%dx
	mov $(0x0b),%al
	out %al,%dx
	mov $(0x2Fc),%dx
	mov $(0x0b),%al
	out %al,%dx
	mov $(0x2F9),%dx
	mov $(0x07),%al
	out %al,%dx 
	ret
