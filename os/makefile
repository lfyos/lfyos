LFYOS_DIR=$(shell pwd)
LFYOS_OBJ=./arch/boot/head.o $(shell ./lfyos_files|sort)
LINUX_DIR=/usr/src/linux
CC=gcc -O3 -D MEMORY_BODY_ID=2 -D FILE_NUMBER=23 -Wall -fno-common -pedantic -c -o
LD=ld -m elf_i386 -T $(LINUX_DIR)/arch/x86/kernel/vmlinux.lds -e stext

$(LINUX_DIR)/vmlinux :	$(LFYOS_OBJ)
	@$(LD) -o $(LINUX_DIR)/vmlinux $(LFYOS_OBJ)
	@nm $(LINUX_DIR)/vmlinux			\
  |grep -v '\(compiled\)\|\(\.o$$\)\|\( [aUw] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)'\
  |sort>$(LINUX_DIR)/System.map
	@cat $(LINUX_DIR)/Makefile|sed "s/^[ \t]*vmlinux[ \t]*:/delete_vmlinux_for_lfyos\t:/">$(LINUX_DIR)/lfyos_makefile
	@make -C $(LINUX_DIR) -f $(LINUX_DIR)/lfyos_makefile bzImage>/dev/null>/dev/null
	@cp -f $(LINUX_DIR)/arch/i386/boot/bzImage /boot/lfyos 
	@rm -f $(LINUX_DIR)/arch/i386/boot/bzImage $(LINUX_DIR)/vmlinux
.c.o	:
	$(CC) $@ $<
.S.o	:
	$(CC) $@ $<
clean	:
	@rm -f $(LFYOS_OBJ) /boot/lfyos $(LINUX_DIR)/lfyos_makefile
	@rm -f $(LINUX_DIR)/arch/i386/boot/bzImage $(LINUX_DIR)/vmlinux
