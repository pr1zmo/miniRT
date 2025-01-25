#include "../minirt.h"

int	parse_cylinder(t_rt *rt, char *line)
{
	char	**line_data;

	line_data = ft_split(line, ' ');
	if (line_data[6])
		return (free_array(line_data), arg_error("cylinder"));
	if (check_range(line_data[1], 3, NULL)
		&& check_range(line_data[2], 3, (int[]){-1, 1})
		&& check_range(line_data[3], 1, (int[]){-1, INT_MAX})
		&& check_range(line_data[4], 1, (int[]){0, INT_MAX})
		&& check_range(line_data[5], 3, (int[]){0, 255}))
	{
		set_cylinder(line_data, (t_object *)&rt->object);
		free_array(line_data);
		rt->object_count ++;
		ft_add_back(&rt->object, (t_object *)rt->object);
		return (1);
	}
	return (0);
}

int	parse_plane(t_rt *rt, char *line)
{
	char	**line_data;

	line_data = ft_split(line, ' ');
	if (line_data[4])
		return (free_array(line_data), arg_error("plane"));
	if (check_range(line_data[1], 3, NULL)
		&& check_range(line_data[2], 3, (int[]){-1, 1})
		&& check_range(line_data[3], 3, (int[]){0, 255}))
	{
		set_plane(line_data, rt->object);
		free_array(line_data);
		rt->object_count ++;
		ft_add_back(&rt->object, (t_object *)&rt->object);
		return (1);
	}
	return (0);
}

int	parse_sphere(t_rt *rt, char *line)
{
	char	**line_data;

	line_data = ft_split(line, ' ');
	if (line_data[4])
		return (free_array(line_data), arg_error("sphere"));
	if (check_range(line_data[1], 3, NULL)
		&& check_range(line_data[2], 1, (int[]){0, INT_MAX})
		&& check_range(line_data[3], 3, (int[]){0, 255}))
	{
		set_sphere(line_data, (t_object *)&rt->object);
		free_array(line_data);
		rt->object_count ++;
		ft_add_back(&rt->object, (t_object *)&rt->object);
		return (1);
	}
	free_array(line_data);
	return (0);
}
