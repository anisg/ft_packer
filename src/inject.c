#include "packer.h"

#define OFF_KEY 0x00

#define OFF_BEGIN_DECRYPT 0x95
#define OFF_LENGTH_TO_DECRYPT 0x9A

#define OFF_JUMP 0xd9

void update_injector(char *b, uint32_t bn, char *s, uint32_t n, uint32_t l, uint32_t r){
	Elf64_Ehdr *h = (void*)s;

	*(uint32_t *)(b + OFF_JUMP) = 0x4003f0;
	*(uint32_t *)(b + OFF_BEGIN_DECRYPT) = elf_offset_to_addr(s, n, l);
	*(uint32_t *)(b + OFF_LENGTH_TO_DECRYPT) = r-l+1;
}

void range_to_encrypt(char *s, uint32_t n, uint32_t *l, uint32_t *r){
	int x;
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = s + h->e_phoff;

	x = elf_first_load_segment(s, n);
	uint32_t off = 0;
	if (ph[x].p_offset <= h->e_phoff + sizeof(*ph) * h->e_phnum)
		off = 0x3f0;//h->e_phoff + sizeof(*ph) * h->e_phnum) - ph[x].p_offset;
	*l = off + ph[x].p_offset;
	*r = off +ph[x].p_offset + ph[x].p_filesz;
}

int inject_binary(char **s, uint32_t *n, uint32_t *l, uint32_t *r){
	char *b;uint32_t bn; uint32_t offset;

	if (fget("res/injector", &b, &bn) == FALSE) return fail("can\'t open res/injector");

	range_to_encrypt(*s, *n, l, r);
	update_injector(b, bn, *s, *n, *l, *r);
	offset = elf_offset_after_last_load_segment(*s, *n);
	insert(s, n, offset, b, bn);
	elf_change_size_last_load_segment(*s, *n, bn);
	elf_set_off_entry(*s, *n, offset + 1 + elf_offset_entry(b, bn));
	elf_shift_offset(*s, *n, offset, bn);
	return offset;
}
