#include "packer.h"

#include <string.h>

void elf64_update_header(void *b, size_t len, size_t pos, size_t add){
	Elf64_Ehdr *h = b;
	h->e_shoff += add;
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
	/*sh = b + h->e_shoff;
	char *strs = b + (sh[h->e_shstrndx]).sh_offset;
	printf("strs: %zu\n", (sh[h->e_shstrndx]).sh_offset);
	for (int i = 0; i < n; i += 1){
		printf("type: %d, name: %s, pos:%zu\n", sh->sh_type, strs + sh->sh_name, sh->sh_offset);
		sh++;
	}*/
}

int find_last_loaded_segment(void *b, size_t len, int *pos){
	Elf64_Ehdr *h = b;
	int n = h->e_phnum;
	Elf64_Phdr *ph = b + h->e_phoff;
	int x = -1;
	for (int i = 0; i < n; i += 1){
		if (ph[i].p_type == PT_LOAD){
			x = i;
		}
	}
	if (x == -1)
		return FALSE;
	*pos = x;
	return TRUE;	 
}

int find_entry_in_file(void *b, size_t len, size_t *e){
	Elf64_Ehdr *h = b;
	size_t entry = h->e_entry;
	int n = h->e_phnum;
	size_t pos = 0;
	Elf64_Phdr *ph = b + h->e_phoff;
	for (int i = 0; i < n; i += 1){
		if (ph[i].p_type == PT_LOAD && 
				entry > ph[i].p_vaddr && entry < ph[i].p_vaddr + ph[i].p_filesz){
			*e = ph[i].p_offset + (entry - ph[i].p_vaddr);
			return TRUE;
		}
	}
	return FALSE;
}

void elf_replace_s(char **s, size_t *len, char *shellcode, int slen){

	int x = 0;
	find_last_loaded_segment(*s, *len, &x);

	Elf64_Ehdr *h = (*s);
	Elf64_Phdr *ph = (*s) + h->e_phoff;

	//get entry point of shellcode:
	*(int*)(shellcode + 0xab) = h->e_entry;

	size_t pos = ph[x].p_offset + ph[x].p_filesz;
	insert(s, len, pos, shellcode, slen);
	h = (*s);
	ph = (*s) + h->e_phoff;


	size_t entry;
	find_entry_in_file(shellcode, slen, &entry);
	h->e_entry = ph[x].p_vaddr + ph[x].p_filesz + entry + 1;

	ph[x].p_filesz += slen;
	ph[x].p_memsz += slen;
	ph[x].p_flags = PF_X | PF_W | PF_R;


	elf64_update_header(*s, *len, pos, slen);
	elf64_update_sections_header(*s, *len, pos, slen);
	elf64_update_segments(*s, *len, pos, slen);
}

void inject_binary(char *s, size_t n){
	char *b;size_t blen;
	if (!fget("shl", &b, &blen)) {
		printf("cant open\n");
		return ;
	}
	elf_replace_s(&s, &n, b, blen);
	printf("DONE\n");
	fput("woody", s, n);
	//return TRUE;
}
