#include "packer.h"

uint64_t random_key(){
	int		fd;
	uint64_t k;

	if ((fd = open("/dev/urandom", O_RDONLY)) == -1)
		return FALSE;
	read(fd, (char *)&k, 8);
	return k;
}

void		encryption(unsigned char *s, size_t n, uint64_t key){
	for (int i = 0; i < n; i += 1){
		s[i] += 13;
	}
}
