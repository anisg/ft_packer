#include <stdio.h>
int main(int ac, char **av)
{
	printf("[");
	for (int i = 0; i < ac; i += 1)	
		printf("%s,", av[i]);
	printf("]\n");
}
