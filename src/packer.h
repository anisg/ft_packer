#ifndef PACKER_H
# define PACKER_H

#include <stdio.h> //printf
#include <stdlib.h> //exit
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
void insert(char **s1, uint64_t *n1, uint64_t pos, char *s2, uint64_t n2);
int fail(char *reason);
int str_equal(char *s1, char *s2);

/*
** crypt.c
*/
#define DELTA 0x9e3779b9
uint32_t	*random_key(); //16 bytes key
void encryption(char *s, uint64_t n, uint32_t *key);

/*
** elf.c
*/
void elf_shift_offset(void *s, uint64_t n, uint64_t pos, uint64_t add);
uint64_t elf_off_text_section(void *s, uint64_t n);
uint64_t elf_offset_entry(char *s, uint64_t n);
void elf_update_flags_of_load_segments(char *s, uint64_t n);
int elf_last_load_segment(char *s, uint64_t n);
int elf_first_load_segment(void *s, uint64_t n);
uint64_t elf_offset_to_addr(void *s, uint64_t n, uint64_t off);
uint64_t elf_offset_after_last_load_segment(void *s, uint64_t n);
void elf_change_size_last_load_segment(void *s, uint64_t n, int add);
int is_elf(char *s, uint64_t n);
int elf_set_off_entry(char *s, uint64_t n, uint64_t off_entry);
int elf_check_valid(char *s, uint64_t n);


/*
** inject.c
*/
#include <elf.h>
int inject_binary(char **s, uint64_t *n, uint64_t *l, uint64_t *r, uint32_t *k);

/*
** main.c
*/

#endif
