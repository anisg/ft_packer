#include "packer.h"

extern char *template;

int fail(char *reason){
	printf("Error: %s\n", reason);
	return -1;
}

int usage(char *name){
	printf("Usage: %s bin\n", name);
	return -1;
}

int packer(char *filename){
	char *s; size_t n; uint32_t k;

	if (fget(filename, &s, &n) == FALSE) return fail("can\'t open file");

	k = random_key();
	encrypt_bin((uint32_t *)s, n, k);
	//inject_binary(s,n);
	//inject_key(k);
	printf("k: %u\n", s, k);

	//if (fput(filename, s, n) == FALSE) return fail("can\'t save new file");
	return 0;
}

int main(int ac, char **av){
	if (ac != 2) return usage(av[0]);
	return packer(av[1]);
}
