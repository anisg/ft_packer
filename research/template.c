
char *decrypt(char *s, int key);

decompress();

int main()
{
	char *s = "abc";int n = 3;
	int key = 20;
	printf("....WOODY....\n");
	execve( decompress(decrypt(s, key)) );
	return (0);
}
