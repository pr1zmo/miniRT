#include "../minirt.h"

char	*ft_strchr(const char *s, int c)
{
	char	to_find;
	int		i;

	i = 0;
	to_find = (char)c;
	while (s[i] != '\0')
	{
		if (s[i] == to_find)
			return ((char *)s + i);
		i ++;
	}
	if (to_find == '\0')
		return ((char *)s + i);
	return (NULL);
}

static double	process_integer_part(char *str, int *i, int *sign)
{
	double	result;

	result = 0;
	while (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			*sign *= -1;
		(*i)++;
	}
	while (str[*i] == '0')
		(*i)++;
	if (str[*i] != '0' && ft_isdigit(str[*i]))
		result += (str[*i] - '0');
	while (str[*i] && str[*i] != '.')
		(*i)++;
	(*i)++;
	return (result);
}

double	recursive_num(int k)
{
	double	i;

	i = 1;
	while (k-- > 0)
		i *= 0.1;
	return (i);
}

static double	process_fractional_part(char *str, int *i)
{
	double	result;
	int		k;

	result = 0;
	k = 1;
	while (str[*i] && ft_isdigit(str[*i]))
	{
		if (ft_isdigit(str[*i]))
			result += (str[*i] - '0') * recursive_num(k);
		(*i)++;
		k++;
	}
	return (result);
}

double	atoi_double(char *str)
{
	int		i;
	int		sign;
	double	result;

	i = 0;
	sign = 1;
	result = process_integer_part(str, &i, &sign);
	if (ft_strchr(str, '.') != NULL)
		result += process_fractional_part(str, &i);
	return (result * sign);
}