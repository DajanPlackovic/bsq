#include <unistd.h>

void ft_print_bn(char num)
{
	int i = 0;
	char c;
	while (i < 8)
	{
		c = !!((num << i) & 0x80) + '0';
		write(1, &c, 1);
		i++;
	}
	write(1, " ", 1);
}

void parse_line(char *num, char *in)
{
	int i = 0;
	char mod;
	while (i < 8)
	{
		if (in[i] == '-')
		{
			mod = ~(1 << (7 - i));
			*num = *num & mod;
		}
		i++;
	}
}

int find_first_obstacle(char num)
{
	int i = 0;

	while (!(num & 0x80))
	{
		num = num << 1;
		i++;
	}
	return i;
}

#include <stdio.h>

int main()
{
	char num = ~0;
	parse_line(&num, "--x-x--x");
	write(1, "--x-x--x\n", 9);
	ft_print_bn(num);
	printf("%d", find_first_obstacle(num));
}
