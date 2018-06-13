#include "packer.h"

int fget(char *filename, char **ptr, size_t *l){
	struct stat		buf;
	int				fd;

	if ((fd = open(filename, O_RDONLY)) < 0)
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

int fput(char *filename, char *ptr, size_t l){
	int fd;

	if ((fd = open(filename, O_WRONLY | O_CREAT, 0755)) < 0)
		return FALSE;
	write(fd, ptr, l);
	close(fd);
	return TRUE;
}
