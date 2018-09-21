#include "packer.h"


#define OFF_JUMP 0x10178

#define OFF_BEGIN_DECRYPT 0x151
#define OFF_LENGTH_TO_DECRYPT 0x156

#define OFF_KEY 0xac


void update_injector(char *b, uint64_t bn, char *s, uint64_t n, uint64_t l, uint64_t r, uint32_t *k){
	Elf64_Ehdr *h = (void*)s;

	(void)n;
	(void)l;
	(void)r;
	(void)k;
	(void)h;
	(void)b;
	if (OFF_JUMP >= bn || OFF_BEGIN_DECRYPT >= bn || OFF_LENGTH_TO_DECRYPT >= bn || OFF_KEY >= bn)
		fail("injector is corrupted");
	*(uint32_t *)(b + OFF_JUMP) = h->e_entry;
	/*
	*(uint32_t *)(b + OFF_BEGIN_DECRYPT) = elf_offset_to_addr(s, n, l);
	*(uint32_t *)(b + OFF_LENGTH_TO_DECRYPT) = r-l+1;
	for (int i = 0; i < 4; i += 1){
		((uint32_t *)(b + OFF_KEY))[i] = k[i];
	}
	*/
}

void range_to_encrypt(char *s, uint64_t n, uint64_t *l, uint64_t *r){
	*l = elf_off_text_section(s,n);
	*r = elf_off_text_section(s,n) + elf_size_text_section(s,n)-1;
}

void insert_spec(char **s, uint64_t *n, uint64_t pos, uint64_t add){
        char *ns = (char *)mmap(NULL,(*n) + add, PROT_WRITE | PROT_EXEC,
                MAP_PRIVATE | MAP_ANON, -1, 0);
        uint64_t i,j,l;
        for (i = 0; i <= pos; i += 1){ ns[i] = (*s)[i]; }
        for (j = 0; j < add; j += 1){ns[i+j] = 0; }
        for (l = 0; i+l < *n; l += 1){ ns[i+j+l] = (*s)[i+l]; }
	*s = ns;
	*n = (*n) + add;
}

uint64_t rearrange(char **s, uint64_t *n, uint64_t *offset){
        int x;
        Elf64_Ehdr *h = (void*)*s;
        Elf64_Phdr *ph = (*(void**)s) + h->e_phoff;

        (void)n;
        x = elf_last_load_segment(*s, *n);
	uint64_t add = ph[x].p_memsz - ph[x].p_filesz;
	insert_spec((char**)s,n,*offset,add);
	(*offset) += add;
	return add;
}

int inject_binary(char **s, uint64_t *n, uint64_t *l, uint64_t *r, uint32_t *k){
	char *b;uint64_t bn; uint64_t offset;

	if (fget("prog", &b, &bn) == FALSE) return fail("can\'t open res/injector");

	elf_check_valid(*s, *n);
	elf_check_valid(b, bn);
	range_to_encrypt(*s, *n, l, r);
	update_injector(b, bn, *s, *n, *l, *r, k);
	offset = elf_offset_after_last_load_segment(*s, *n);
	//ADD
	uint64_t add = rearrange(s, n, &offset);
	insert(s, n, offset, b, bn);
	//END ADD
	elf_change_size_last_load_segment(*s, *n, bn+add);
	elf_set_off_entry(*s, *n, offset + 1 + elf_offset_entry(b, bn));
	elf_shift_offset(*s, *n, offset, bn+add);
	return offset;
}
