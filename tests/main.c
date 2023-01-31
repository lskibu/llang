# include <stdio.h>

void print_str(char *str);

int main()
{
	char str[] = "hello, world!\n";
	print_str(str);
	return 0;
}

void print_str(char *str)
{
	while(*str) {
	fputc(*str, stdout);
	str = str+1;
	}
}
