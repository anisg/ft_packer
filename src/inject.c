#include "packer.h"

void elf_replace_s(char *b, size_t len, char *bin, int n){
	//char *s = copy(*b);
	int pos_length = 0x4cb;
	int pos_data = 0x574;
	printf("b:%p,len:%zu\n", b, len);
	b[pos_data+1] = 'X';
	b[pos_data+2] = 'X';
	b[pos_data+3] = 'X';
}

void inject_binary(char *s, int n){
	char *b;size_t blen;
	fget("bin_template", &b, &blen);
	//if (is_elf_format(b,blen) == FALSE)
	//	return FALSE;
	elf_replace_s(b, blen, s, n);
	fput("woody", b, blen);
	//return TRUE;
}
