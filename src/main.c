#include "packer.h"

int usage(char *name){
	printf("Usage: %s bin\n", name);
	return -1;
}

int packer(char *filename){
	char *s; uint64_t n; uint32_t *k;
	uint64_t l,r; //l as left, r as right

	if (fget(filename, &s, &n) == FALSE) return fail("can\'t open file");
	if (is_elf(s,n) == FALSE) return fail("file is not a valid elf64 file");
	elf_update_flags_of_load_segments(s, n);
	k = random_key();
	inject_binary(&s, &n, &l, &r, k);
	//printf("KEY:[%d,%d,%d,%d], l:%d r:%d\n", k[0],k[1],k[2],k[3],l,r);
	//encryption(s + l, r-l+1, k);
	fput("woody", s, n);
	return 0;
}

int main(int ac, char **av){
	if (ac != 2) return usage(av[0]);
	return packer(av[1]);
}
