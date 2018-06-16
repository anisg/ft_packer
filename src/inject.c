#include "packer.h"

#include <string.h>

void elf64_update_header(void *b, size_t len, size_t pos, size_t add){
	Elf64_Ehdr *h = b;
	if (h->e_shoff > pos) h->e_shoff += add;
}

void elf64_update_segments(void *b, size_t len, size_t pos, size_t add){
	Elf64_Ehdr *h = b;
	Elf64_Phdr *ph = b + h->e_phoff;
	int n = h->e_phnum;
	for (int i = 0; i < n; i += 1){
		if (ph->p_offset > pos) {
			ph->p_offset += add;
			ph->p_vaddr += add;
			ph->p_paddr += add;
		}
		else if (ph->p_offset <= pos && ph->p_offset + ph->p_filesz > pos){
			ph->p_filesz += add;
			ph->p_memsz += add;
		}
		ph++;
	}
}

void elf64_update_sections_header(void *b, size_t len, size_t pos, size_t add){
	Elf64_Ehdr *h = b;
	int n = h->e_shnum;
	Elf64_Shdr *sh = b + h->e_shoff;
	for (int i = 0; i < n; i += 1){
		if (sh->sh_offset > pos){
			sh->sh_offset += add;
			sh->sh_addr += add;
		}
		sh++;
	}
	sh = b + h->e_shoff;
	char *strs = b + (sh[h->e_shstrndx]).sh_offset;
	printf("strs: %zu\n", (sh[h->e_shstrndx]).sh_offset);
	for (int i = 0; i < n; i += 1){
		printf("type: %d, name: %s, pos:%zu\n", sh->sh_type, strs + sh->sh_name, sh->sh_offset);
		sh++;
	}
}

void elf_replace_s(char **b, size_t *len, char *bin, int n){
	int pos_length = 0x52b;
	int pos_data = 0x5d4;

	//insert(b, len, pos_data, "0123456789abcdefg", 17);
	//elf64_update_header(*b, *len, pos_data, 16);
	//elf64_update_sections_header(*b, *len, pos_data, 16);
	//elf64_update_segments(*b, *len, pos_data, 16);
	int *x = ((*b) + pos_length);
	x[0] = 16;
}

void inject_binary(char *s, int n){
	char *b;size_t blen;
	if (!fget("bin_template", &b, &blen)) return ;
	elf_replace_s(&b, &blen, s, n);
	fput("woody", b, blen);
	//return TRUE;
}
