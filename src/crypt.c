#include "packer.h"

uint32_t random_key(){
	int		fd;
	uint32_t k;

	if ((fd = open("/dev/urandom", O_RDONLY)) == -1)
		return FALSE;
	read(fd, (char *)&k, 4);
	return k;
}

void		encryption(char *s, size_t n, uint32_t key){
	for (int i = 0; i < n; i += 1){
		s[i] += 13;
	}
}
