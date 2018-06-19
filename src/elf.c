#include "packer.h"

void _elf_shift_offset_ph(void *s, uint64_t n, uint64_t pos, uint64_t add){
	Elf64_Ehdr *h = s;
	Elf64_Phdr *ph = s + h->e_phoff;
	for (int i = 0; i < h->e_phnum; i += 1)
		if (ph[i].p_offset > pos) {
			ph[i].p_offset += add;
			ph[i].p_vaddr += add;
			ph[i].p_paddr += add;
		}
}

void _elf_shift_offset_sh(void *s, uint64_t n, uint64_t pos, uint64_t add){
	Elf64_Ehdr *h = s;
	Elf64_Shdr *sh = s + h->e_shoff;
	for (int i = 0; i < h->e_shnum; i += 1)
		if (sh[i].sh_offset > pos){
			sh[i].sh_offset += add;
			sh[i].sh_addr += add;
		}
}

void elf_shift_offset(void *s, uint64_t n, uint64_t pos, uint64_t add){
	Elf64_Ehdr *h = s;
	if (pos < h->e_shoff) h->e_shoff += add;
	if (pos < h->e_phoff) h->e_phoff += add;
	_elf_shift_offset_ph(s, n, pos, add);
	_elf_shift_offset_sh(s, n, pos, add);
}

uint64_t elf_off_text_section(void *s, uint64_t n){
	Elf64_Ehdr *h = s;
	Elf64_Shdr *sh = s + h->e_shoff;

	char *strs = s + (sh[h->e_shstrndx]).sh_offset;
	for (int i = 0; i < h->e_shnum; i += 1){
		if (str_equal(strs+sh[i].sh_name, ".text"))
			return sh[i].sh_offset;
	}
	return fail("text section not found");
}


uint64_t elf_offset_entry(char *s, uint64_t n){
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = s + h->e_phoff;

	uint64_t entry = h->e_entry;
	for (int i = 0; i < h->e_phnum; i += 1){
		if (ph[i].p_type == PT_LOAD &&
				entry > ph[i].p_vaddr && entry < ph[i].p_vaddr + ph[i].p_filesz){
			return ph[i].p_offset + (entry - ph[i].p_vaddr);
		}
	}
	return fail("entry is not in the file!");
}

void elf_update_flags_of_load_segments(char *s, uint64_t n){
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = s + h->e_phoff;

	for (int i = 0; i < h->e_phnum; i += 1)
		if (ph[i].p_type == PT_LOAD)
			ph[i].p_flags = PF_X | PF_W | PF_R;
}

int elf_last_load_segment(char *s, uint64_t n){
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

int elf_first_load_segment(void *s, uint64_t n){
	Elf64_Ehdr *h = s;
	Elf64_Phdr *ph = s + h->e_phoff;

	for (int i = 0; i < h->e_phnum; i += 1)
		if (ph[i].p_type == PT_LOAD)
			return i;
	return fail("no load segment in the binary\n");
}

uint64_t elf_offset_to_addr(void *s, uint64_t n, uint64_t off){
	Elf64_Ehdr *h = s;
	Elf64_Phdr *ph = s + h->e_phoff;

	for (int i = 0; i < h->e_phnum; i += 1){
		if (ph[i].p_type == PT_LOAD &&
				off >= ph[i].p_offset && off <= ph[i].p_offset + ph[i].p_filesz){
			return ph[i].p_vaddr + (off - ph[i].p_offset);
		}
	}
	return fail("wasn\'t able to convert an offset to an memory address.");
}

uint64_t elf_offset_after_last_load_segment(void *s, uint64_t n){
	int x;
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = s + h->e_phoff;

	x = elf_last_load_segment(s, n);
	return ph[x].p_offset + ph[x].p_filesz;
}

void elf_change_size_last_load_segment(void *s, uint64_t n, int add){
	int x;
	Elf64_Ehdr *h = (void*)s;
	Elf64_Phdr *ph = s + h->e_phoff;

	x = elf_last_load_segment(s, n);
	ph[x].p_filesz += add;
	ph[x].p_memsz += add;
}

int is_elf(char *s, uint64_t n){
	return (*((uint32_t*)s) == 0x464c457f) && (s[4] == 2);
}

int elf_set_off_entry(char *s, uint64_t n, uint64_t off_entry){
	Elf64_Ehdr *h = (void*)s;

	h->e_entry = elf_offset_to_addr(s,n,off_entry);
	return TRUE;
}
