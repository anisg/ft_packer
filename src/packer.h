#ifndef PACKER_H
# define PACKER_H

#include <stdio.h> //printf

#define TRUE 1
#define FALSE 0

/*
** utils.c
*/
#include <sys/stat.h> //fstat
#include <unistd.h> //write
#include <sys/mman.h> //mmap
#include <fcntl.h> //open
int fget(char *filename, char **ptr, size_t *l);
int fput(char *filename, char *ptr, size_t l);

/*
** main.c
*/


#endif
