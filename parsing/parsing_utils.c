#include "../minirt.h"

int	is_float(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i])
	{
		if (ft_isdigit(str[i]) != 1 && str[i] != '-' && str[i] != '.')
			return (0);
		i++;
	}
	return (1);
}

int	array_length(char **arr)
{
	int	length;

	length = 0;
	while (arr[length])
		length++;
	return (length);
}

int	parse_vector(char *str, t_vector *vect)
{
	int		i;
	int		ret;
	char	**nbrs;

	i = -1;
	ret = 0;
	nbrs = ft_split(str, ',');
	while (nbrs && nbrs[++i])
		if (!is_float(nbrs[i]))
			ret = 1;
	if (array_length(nbrs) != 3)
		ret = 1;
	else
	{
		vect->x = atoi_double(nbrs[0]);
		vect->y = atoi_double(nbrs[1]);
		vect->z = atoi_double(nbrs[2]);
	}
	free_array(nbrs);
	return (ret);
}
