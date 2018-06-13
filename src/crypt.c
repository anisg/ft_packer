#include "packer.h"

uint32_t random_key(){
	int		fd;
	uint32_t k;

	if ((fd = open("/dev/urandom", O_RDONLY)) == -1)
		return FALSE;
	read(fd, (char *)&k, 4);
	return k;
}

void		encrypt_bin(uint32_t *buffer, size_t n, uint32_t key)
{
	//TEA ENCRYPT
	uint32_t	v0;
	uint32_t	v1;
	uint32_t	sum;
	uint32_t	i;
	uint8_t		k0;
	uint8_t		k1;
	uint8_t		k2;
	uint8_t		k3;

	sum = 0;
	v0 = buffer[0];
	v1 = buffer[1];
	k0 = key & 0xFF;
	k1 = (key >> 8) & 0xFF;
	k2 = (key >> 16) & 0xFF;
	k3 = (key >> 24) & 0xFF;
	i = 0;
	while (i++ < 32) {
		sum += DELTA;
		v0 += ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
		v1 += ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
	}
	buffer[0] = v0;
	buffer[1] = v1;
}

/*void	decrypt(uint32_t *buffer, uint32_t *key)
{
	//TEA_DECRYPT
	uint32_t	v0;
	uint32_t	v1;
	uint32_t	i;
	uint8_t		k0;
	uint8_t		k1;
	uint8_t		k2;
	uint8_t		k3;
	uint32_t	sum;

	sum = 0;
	v0 = buffer[0];
	v1 = buffer[1];
	k0 = *key & 0xFF;
	k1 = (*key >> 8) & 0xFF;
	k2 = (*key >> 16) & 0xFF;
	k3 = (*key >> 24) & 0xFF;
	sum = SUM;
	i = 0;
	while (i++ < 32)
	{
		v1 -= ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
		v0 -= ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
		sum -= DELTA;
	}
	buffer[0] = v0;
	buffer[1] = v1;
}*/
