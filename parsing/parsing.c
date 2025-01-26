#include "../minirt.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *get_next_line(int fd)
{
	char		buff[2];
	char	*line = NULL;
	char		*temp;
	ssize_t		bytes_read;

	while ((bytes_read = read(fd, &buff[0], 1)) > 0)
	{
		buff[1] = '\0';
		if (!line)
			line = ft_strdup(buff);
		else
		{
			temp = ft_strjoin(line, buff);
			if (!temp)
				return (free(line), NULL);
			free(line);
			line = temp;
		}
		if (buff[0] == '\n')
			break;
	}
	if (bytes_read < 0)
		return (free(line), NULL);
	return (line);
}

void	set_direction(char *line, t_vector *directions)
{
	char		**xyz;

	xyz = ft_split(line, ',');
	directions->x = atoi_double(xyz[0]);
	directions->y = atoi_double(xyz[1]);
	directions->z = atoi_double(xyz[2]);
	free_array(xyz);
}

void	set_rgb(char	*line, t_color *colors)
{
	char	**rgb;

	rgb = ft_split(line, ',');
	colors->r = ft_atoi(rgb[0]);
	colors->g = ft_atoi(rgb[1]);
	colors->b = ft_atoi(rgb[2]);
	free_array(rgb);
}

void	set_camera(char **line, t_camera *camera)
{
	camera->fov = ft_atoi(line[3]);
	set_direction(line[1], &camera->position);
	set_direction(line[2], &camera->orientation);
}

void	set_light(char **line, t_light *light)
{
	set_direction(line[1], &light->position);
	light->brightness = atoi_double(line[2]);
}

void	set_ambient(char **line, t_ambient *ambient)
{
	ambient->lighting = atoi_double(line[1]);
	set_rgb(line[2], &ambient->color);
}

int	is_float(const char *c)
{
	int	i;
	int	float_flag;

	i = 0;
	float_flag = 0;
	while (c[i] == '-' || c[i] == '+')
		i++;
	while (c[i])
	{
		if (c[i] == '.')
		{
			if (float_flag)
				return (0);
			float_flag = 1;
		}
		else if (!ft_isdigit(c[i]))
			return (0);
		i++;
	}
	return (1);
}

int	check_range(char *line, int count, const int range[2])
{
	int		i;
	char	**line_arr;

	i = 0;
	line_arr = ft_split(line, ',');
	if (!range)
	{
		while (line_arr[i] && i < count)
		{
			if (!is_float(line_arr[i]))
				return (free_array(line_arr), 0);
			i++;
		}
	}
	else
	{
		i = 0;
		while (line_arr[i] && i < count)
		{
			if (!is_float(line_arr[i])
				&& (ft_atoi(line_arr[i]) < range[0] || ft_atoi(line_arr[i]) > range[1]))
				return (free_array(line_arr), 0);
			i++;
		}
	}
	free_array(line_arr);
	return (1);
}

int	parse_ambient(t_rt *rt, char *line)
{
	char	**line_data;

	line_data = ft_split(line, ' ');
	if (line_data[3])
		return (free_array(line_data), arg_error("ambient"));
	if (check_range(line_data[1], 1, (int[]){0, 1})
		&& check_range(line_data[2], 3, (int[]){0, 255}))
	{
		set_ambient(line_data, &rt->ambient);
		free_array(line_data);
		return (1);
	}
	free_array(line_data);
	return (0);
}

int	parse_camera(t_rt *rt, char *line)
{
	char	**line_data;

	line_data = ft_split(line, ' ');
	if (line_data[4])
		return (free_array(line_data), arg_error("camera"));
	if (check_range(line_data[1], 3, NULL)
		&& check_range(line_data[2], 3, (int[]){-1, 1})
		&& check_range(line_data[3], 1, (int[]){0, 180}))
	{
		set_camera(line_data, &rt->camera);
		free_array(line_data);
		return (1);
	}
	free_array(line_data);
	return (0);
}

int	parse_light(t_rt *rt, char *line)
{
	char	**line_data;

	line_data = ft_split(line, ' ');
	if (line_data[3])
		return (free_array(line_data), arg_error("light"));
	if (check_range(line_data[1], 3, NULL)
		&& check_range(line_data[2], 2, (int[]){0, 1}))
	{
		set_light(line_data, &rt->light);
		free_array(line_data);
		return (1);
	}
	free_array(line_data);
	return (0);
}

void	parsing_error(t_rt *rt, char *msg)
{
	ft_putstr_fd("Error: ", 2);
	ft_putendl_fd(msg, 2);
	destroy(rt);
}

int	valid_line(char *arg)
{
	if (!arg)
		return (0);
	if (ft_strncmp(arg, "A", 0) || ft_strncmp(arg, "C", 0)
		|| ft_strncmp(arg, "L", 0) || ft_strncmp(arg, "sp", 0)
		|| ft_strncmp(arg, "pl", 0) || ft_strncmp(arg, "cy", 0))
		return (1);
	return (0);
}

int	parse(t_rt *rt)
{
	char	*line;
	char	*first_arg;

	rt->object = NULL;
	while (1) {
		line = get_next_line(rt->file_fd);
		if (!line)
			break;
		first_arg = ft_substr(line, 0, ft_strchr(line, ' ') - line);
		if (!valid_line(first_arg))
			return (free(line), free(first_arg), parsing_error(rt, "INVALID LINE"), 1);
		else {
			if (!ft_strncmp(first_arg, "A", 0) && !parse_ambient(rt, line))
				return (free(line), free(first_arg), parsing_error(rt, "AMBIENT"), 1);
			else if (!ft_strncmp(first_arg, "C", 0) && !parse_camera(rt, line))
				return (free(line), free(first_arg), parsing_error(rt, "CAMERA"), 1);
			else if (!ft_strncmp(first_arg, "L", 0) && !parse_light(rt, line))
				return (free(line), free(first_arg), parsing_error(rt, "LIGHT"), 1);
			else if (!ft_strncmp(first_arg, "sp", 0) && !parse_sphere(rt, line))
				return (free(line), free(first_arg), parsing_error(rt, "SPHERE"), 1);
			else if (!ft_strncmp(first_arg, "pl", 0) && !parse_plane(rt, line))
				return (free(line), free(first_arg), parsing_error(rt, "PLANE"), 1);
			else if (!ft_strncmp(first_arg, "cy", 0) && !parse_cylinder(rt, line))
				return (free(line), free(first_arg), parsing_error(rt, "CYLINDER"), 1);
		}
	}
	free(line);
	free(first_arg);
	printf("OK\n");
	printf("Current type should be 0: %d\n", rt->object->type);
//	list_objects(rt);
	return (0);
}