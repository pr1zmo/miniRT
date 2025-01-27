#include "../minirt.h"

double atoi_double(char *str)
{
	double	result = 0.0;
	double	fraction = 0.0;
	double	divisor = 1.0;
	int		sign = 1;

	if (ft_atoi(str) > INT_MAX)
		return (0.0);
	while (*str == ' ')
		str++;
	while (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		result = result * 10.0 + (*str - '0');
		str++;
	}
	if (*str == '.')
	{
		str++;
		while (ft_isdigit(*str))
		{
			fraction = fraction * 10.0 + (*str - '0');
			divisor *= 10.0;
			str++;
		}
	}
	result += fraction / divisor;
	result *= sign;
	return (result);
}