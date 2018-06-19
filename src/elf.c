#include "packer.h"

void _elf_shift_offset_ph(void *s, uint32_t n, uint32_t pos, uint32_t add){
	Elf64_Ehdr *h = s;
	Elf64_Phdr *ph = s + h->e_phoff;
	for (int i = 0; i < h->e_phnum; i += 1)
		if (ph[i].p_offset > pos) {
			ph[i].p_offset += add;
			ph[i].p_vaddr += add;
			ph[i].p_paddr += add;
		}
}

void _elf_shift_offset_sh(void *s, uint32_t n, uint32_t pos, uint32_t add){
	Elf64_Ehdr *h = s;
	Elf64_Shdr *sh = s + h->e_shoff;
	for (int i = 0; i < h->e_shnum; i += 1)
		if (sh[i].sh_offset > pos){
			sh[i].sh_offset += add;
			sh[i].sh_addr += add;
		}
}

void elf_shift_offset(void *s, uint32_t n, uint32_t pos, uint32_t add){
	Elf64_Ehdr *h = s;
	if (pos < h->e_shoff) h->e_shoff += add;
	if (pos < h->e_phoff) h->e_phoff += add;
	_elf_shift_offset_ph(s, n, pos, add);
	_elf_shift_offset_sh(s, n, pos, add);
}

uint32_t elf_offset_entry(char *s, uint32_t n){
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = s + h->e_phoff;

	uint32_t entry = h->e_entry;
	for (int i = 0; i < h->e_phnum; i += 1){
		if (ph[i].p_type == PT_LOAD &&
				entry > ph[i].p_vaddr && entry < ph[i].p_vaddr + ph[i].p_filesz){
			return ph[i].p_offset + (entry - ph[i].p_vaddr);
		}
	}
	return fail("entry is not in the file!");
}

void elf_update_flags_of_load_segments(char *s, uint32_t n){
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = s + h->e_phoff;

	for (int i = 0; i < h->e_phnum; i += 1)
		if (ph[i].p_type == PT_LOAD)
			ph[i].p_flags = PF_X | PF_W | PF_R;
}

int elf_last_load_segment(char *s, uint32_t n){
	int x;
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = s + h->e_phoff;

	x = -1;
	for (int i = 0; i < h->e_phnum; i += 1)
		if (ph[i].p_type == PT_LOAD)
			x = i;
	if (x == -1) return fail("no load segment in the binary\n");
	return x;
}

int elf_first_load_segment(void *s, uint32_t n){
	Elf64_Ehdr *h = s;
	Elf64_Phdr *ph = s + h->e_phoff;

	for (int i = 0; i < h->e_phnum; i += 1)
		if (ph[i].p_type == PT_LOAD)
			return i;
	return fail("no load segment in the binary\n");
}

uint32_t elf_offset_after_last_load_segment(void *s, uint32_t n){
	int x;
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = s + h->e_phoff;

	x = elf_last_load_segment(s, n);
	return ph[x].p_offset + ph[x].p_filesz;
}

void elf_change_size_last_load_segment(void *s, uint32_t n, int add){
	int x;
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = s + h->e_phoff;

	x = elf_last_load_segment(s, n);
	ph[x].p_filesz += add;
	ph[x].p_memsz += add;
}

int is_elf(char *s, uint32_t n){
	return (*((uint32_t*)s) == 0x464c457f) && (s[4] == 2);
}

int elf_set_off_entry(char *s, uint32_t n, uint32_t off_entry){
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = s + h->e_phoff;

	for (int i = 0; i < h->e_phnum; i += 1){
		if (ph[i].p_type == PT_LOAD &&
				off_entry > ph[i].p_offset && off_entry < ph[i].p_offset + ph[i].p_filesz){
			h->e_entry = ph[i].p_vaddr + (off_entry - ph[i].p_offset);
			return TRUE;
		}
	}
	fail("wasn\'t able to change the entry");
	return FALSE;
}
