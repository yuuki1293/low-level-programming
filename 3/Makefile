AS = nasm
LD = ld
ASFLAGS = -f elf64 -g -F dwarf

forth: forth.o lib.o
	$(LD) -o $@ $^

%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	rm *.o forth

.PHONY: clean