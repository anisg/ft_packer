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

int fget(char *filename, char **ptr, uint64_t *l);
int fput(char *filename, char *ptr, uint64_t l);
void insert(char **s1, uint64_t *n1, int pos, char *s2, uint64_t n2);
int fail(char *reason);
int str_equal(char *s1, char *s2);

/*
** crypt.c
*/
#define DELTA 0x9e3779b9
uint32_t	*random_key(); //16 bytes key
void encryption(char *s, uint64_t n, uint32_t *key);

/*
** inject.c
*/
#include <elf.h>
int inject_binary(char **s, uint64_t *n, uint64_t *l, uint64_t *r, uint32_t *k);

/*
** main.c
*/

#endif
