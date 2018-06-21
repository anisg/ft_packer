#include "packer.h"

uint32_t *random_key(){
	int		fd;

	uint32_t *k = mmap(0, sizeof(*k) * 4, PROT_WRITE, MAP_PRIVATE |  MAP_ANON, -1, 0);
	if ((fd = open("/dev/urandom", O_RDONLY)) == -1)
		return FALSE;
	read(fd, (char *)k, sizeof(*k)*4);
	return k;
}

void encrypt_block(uint32_t* v, uint32_t *k) {
	uint32_t v0=v[0], v1=v[1], sum=0, i;           /* initialisation */
    uint32_t delta=0x9e3779b9;                     /* constantes de clef */
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* mise en cache de la clef */
    for (i=0; i < 32; i++) {                       /* boucle principale */
        sum += delta;
        v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
    }
    v[0] = v0;
	v[1]=v1;
}

void encryption(char *s, uint64_t n, uint32_t *key){
	for (uint64_t i = 0; i < n; i += 8){
		if (i + 7 <= n){
			encrypt_block((uint32_t*)(s+i), key);
		}
	}
}