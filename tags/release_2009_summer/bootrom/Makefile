# Makefile for bootrom, see ../common/Makefile.common for common settings

# DO NOT use thumb mode in the phase 1 bootloader since that generates a section with glue code
ARMSRC = fromflash.c 
THUMBSRC = usb.c bootrom.c
ASMSRC = ram-reset.s flash-reset.s

## There is a strange bug with the linker: Sometimes it will not emit the glue to call
## BootROM from ARM mode. The symbol is emitted, but the section will be filled with
## zeroes. As a temporary workaround, do not use thumb for the phase 2 bootloader
## -- Henryk Plötz <henryk@ploetzli.ch> 2009-09-01
ARMSRC := $(ARMSRC) $(THUMBSRC)
THUMBSRC := 

# Do not move this inclusion before the definition of {THUMB,ASM,ARM}SRC
include ../common/Makefile.common

all: $(OBJDIR)/bootrom.s19

$(OBJDIR)/bootrom.elf: $(VERSIONOBJ) $(ASMOBJ) $(ARMOBJ) $(THUMBOBJ)
	$(LD) -g -Tldscript-flash --oformat elf32-littlearm -Map=$(patsubst %.elf,%.map,$@) -o $@ $^

clean:
	$(DELETE) $(OBJDIR)$(PATHSEP)*.o
	$(DELETE) $(OBJDIR)$(PATHSEP)*.elf
	$(DELETE) $(OBJDIR)$(PATHSEP)*.s19
	$(DELETE) $(OBJDIR)$(PATHSEP)*.map
	$(DELETE) $(OBJDIR)$(PATHSEP)*.d
	$(DELETE) version.c

.PHONY: all clean help 
help:
	@echo Multi-OS Makefile, you are running on $(DETECTED_OS)
	@echo Possible targets:
	@echo +	all   - Make $(OBJDIR)/bootrom.s19, the main bootrom
	@echo +	clean - Clean $(OBJDIR)
