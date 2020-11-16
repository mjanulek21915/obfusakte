#include "obfuscate.h"

void enc_displace_filler(unsigned char *str, int n)
{
	int fd = open("/dev/random", O_RDONLY);
	unsigned char rnd;
	while (n--)
		read(fd, str++, 1);
}

int main()
{
	char str[100];
	char *temp;
	temp = (char *)str;
	int len = 10;

	enc_displace_filler(str, len);
	while (len--)
	{
		printf("%d\n", (int)(*temp));
		temp++;
	}
}