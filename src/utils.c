#include "packer.h"

int fget(char *filename, char **ptr, uint64_t *l){
	struct stat		buf;
	int				fd;

	if ((fd = open(filename, O_RDWR)) < 0)
		return FALSE;
	if (fstat(fd, &buf) < 0)
		return FALSE;
	if (((*ptr) = mmap(NULL, buf.st_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0))
		== MAP_FAILED)
		return FALSE;
	(*l) = buf.st_size;
	close(fd);
	return TRUE;
}

int fput(char *filename, char *ptr, uint64_t l){
	int fd;

	if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0755)) < 0)
		return FALSE;
	write(fd, ptr, l);
	close(fd);
	return TRUE;
}

void insert(char **s1, uint64_t *n1, int pos, char *s2, uint64_t n2){
	char *ns = malloc( (*n1) + n2 );
	size_t i,j,l;
	for (i = 0; i <= pos; i += 1){ ns[i] = (*s1)[i]; }
	for (j = 0; j < n2; j += 1){ns[i+j] = s2[j]; }
	for (l = 0; i+l < *n1; l += 1){ ns[i+j+l] = (*s1)[i+l]; }
	//TODO: clean here *s
	*s1 = ns;
	*n1 = (*n1) + n2;
}

int fail(char *reason){
	dprintf(2, "Error: %s\n", reason);
	exit(-1);
	return -1;
}

int     str_equal(char *s1, char *s2)
{
	int i;

	i = 0;
	if (!s1 || !s2)
		return FALSE;
	while (s1[i] && s2[i]
		&& (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]) == 0;
}
