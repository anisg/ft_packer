#include "packer.h"

void insert(char **s1, size_t *n1, int pos, char *s2, size_t n2){
	char *ns = malloc( (*n1) + n2 - 1);
	size_t i,j,l;
	for (i = 0; i < pos; i += 1){ ns[i] = (*s1)[i]; }
	for (j = 0; j < n2; j += 1){ns[i+j] = s2[j]; }
	for (l = 0; i+l+1 < *n1; l += 1){ ns[i+j+l] = (*s1)[i+l+1]; }
	//TODO: clean here *s
	*s1 = ns;
	*n1 = (*n1) + n2 - 1;
}

#include <string.h>

void elf64_update_header(void *b, size_t len, size_t pos, size_t add){
	Elf64_Ehdr *h = b;
	printf("inpoint: %zx\n", pos);
	printf("entry: %zx\n", h->e_entry);
	printf("phoff: %zx\n", h->e_phoff);
	printf("shoff: %zx\n", h->e_shoff);
	printf("shnum: %zx, phnum: %zx\n", h->e_shnum, h->e_phnum);
	if (h->e_shoff > pos) h->e_shoff += add;
}

void elf64_update_segments(void *b, size_t len, size_t pos, size_t add){
	Elf64_Ehdr *h = b;
	Elf64_Phdr *ph = b + h->e_phoff;
	int n = h->e_phnum;
	for (int i = 0; i < n; i += 1){
		//printf("%04d --> %zu (+%zu) => %zu\n", ph->p_type, ((void*)ph->p_offset), ph->p_filesz, ((void*)ph->p_offset) + ph->p_filesz);
		if (ph->p_offset > pos) {
			//printf("YES\n");
			ph->p_offset += add;
			ph->p_vaddr += add;
			ph->p_paddr += add;
			//printf("====> %zu (+%zu) => %zu\n", ((void*)ph->p_offset), ph->p_filesz, ((void*)ph->p_offset) + ph->p_filesz);
		}
		else if (ph->p_offset <= pos && ph->p_offset + ph->p_filesz > pos){
			ph->p_filesz += add;
			ph->p_memsz += add;
			//printf("LOAD\n");
			//printf("FOUND matched segment: %zu\n", ((void*)ph->p_offset) + ph->p_filesz);
		}
		ph++;
	}
}

void elf64_update_sections_header(void *b, size_t len, size_t pos, size_t add){
	Elf64_Ehdr *h = b;
	int n = h->e_shnum;
	printf("nsize:%d\n", n);
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
	for (int i = 0; i < n; i += 1){
		printf("type: %d, name: %s\n", sh->sh_type, strs + sh->sh_name);
		sh++;
	}
}

void elf_replace_s(char **b, size_t *len, char *bin, int n){
	int pos_length = 0x52b;
	int pos_data = 0x5d4;

	insert(b, len, pos_data, "0123456789abcdefg", 17);
	elf64_update_header(*b, *len, pos_data, 16);
	elf64_update_sections_header(*b, *len, pos_data, 16);
	elf64_update_segments(*b, *len, pos_data, 16);
	int *x = ((*b) + pos_length);
	x[0] = 6;
}

void inject_binary(char *s, int n){
	char *b;size_t blen;
	if (!fget("bin_template", &b, &blen)) return ;
	//if (is_elf_format(b,blen) == FALSE)
	//	return FALSE;
	elf_replace_s(&b, &blen, s, n);
	fput("woody", b, blen);
	//return TRUE;
}
