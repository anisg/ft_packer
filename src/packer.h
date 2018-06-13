#ifndef PACKER_H
# define PACKER_H

#include <stdio.h> //printf
#include <stdint.h>

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
** crypt.c
*/
#define DELTA 0x9e3779b9
uint32_t	random_key();
void		encrypt_bin(uint32_t *buffer, size_t n, uint32_t key);

/*
** inject.c
*/
void inject_binary(char *s, int n);


/*
** main.c
*/


#endif
