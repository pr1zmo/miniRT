#include "../minirt.h"

int	array_length(char **arr)
{
	int	length;

	length = 0;
	while (arr[length])
		length++;
	return (length);
}

int	arg_error(char *msg)
{
	printf("Error: too many arguments for %s\n", msg);
	return (0);
}