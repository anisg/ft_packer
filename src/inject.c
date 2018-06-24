#include "packer.h"


#define OFF_JUMP 0x1d2

#define OFF_BEGIN_DECRYPT 0x151
#define OFF_LENGTH_TO_DECRYPT 0x156

#define OFF_KEY 0xac


void update_injector(char *b, uint64_t bn, char *s, uint64_t n, uint64_t l, uint64_t r, uint32_t *k){
	Elf64_Ehdr *h = (void*)s;

	if (OFF_JUMP >= bn || OFF_BEGIN_DECRYPT >= bn || OFF_LENGTH_TO_DECRYPT >= bn || OFF_KEY >= bn)
		fail("injector is corrupted");
	*(uint32_t *)(b + OFF_JUMP) = h->e_entry;
	*(uint32_t *)(b + OFF_BEGIN_DECRYPT) = elf_offset_to_addr(s, n, l);
	*(uint32_t *)(b + OFF_LENGTH_TO_DECRYPT) = r-l+1;
	for (int i = 0; i < 4; i += 1){
		((uint32_t *)(b + OFF_KEY))[i] = k[i];
	}
}

void range_to_encrypt(char *s, uint64_t n, uint64_t *l, uint64_t *r){
	*l = elf_off_text_section(s,n);
	*r = elf_off_text_section(s,n) + elf_size_text_section(s,n)-1;
}

int inject_binary(char **s, uint64_t *n, uint64_t *l, uint64_t *r, uint32_t *k){
	char *b;uint64_t bn; uint64_t offset;

	if (fget("res/injector", &b, &bn) == FALSE) return fail("can\'t open res/injector");

	elf_check_valid(*s, *n);
	elf_check_valid(b, bn);
	range_to_encrypt(*s, *n, l, r);
	update_injector(b, bn, *s, *n, *l, *r, k);
	offset = elf_offset_after_last_load_segment(*s, *n);
	insert(s, n, offset, b, bn);
	elf_change_size_last_load_segment(*s, *n, bn);
	elf_set_off_entry(*s, *n, offset + 1 + elf_offset_entry(b, bn));
	elf_shift_offset(*s, *n, offset, bn);
	return offset;
}
