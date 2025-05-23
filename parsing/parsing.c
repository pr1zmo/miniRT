#include "../minirt.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static char	*append_buff(char *line, char *buff)
{
	char	*temp;

	if (!line)
		return (ft_strdup(buff));
	temp = ft_strjoin(line, buff);
	if (!temp)
	{
		free(line);
		return (NULL);
	}
	free(line);
	return (temp);
}

char	*get_next_line(int fd)
{
	char	buff[2];
	char	*line;
	ssize_t	bytes_read;

	line = NULL;
	bytes_read = read(fd, buff, 1);
	while (bytes_read > 0)
	{
		if (bytes_read < 0)
			break ;
		buff[1] = '\0';
		line = append_buff(line, buff);
		if (!line)
			return (NULL);
		if (buff[0] == '\n')
			break ;
		bytes_read = read(fd, buff, 1);
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
	colors->r = scale(atoi_double(rgb[0]), 0, 1, 0, 255);
	colors->g = scale(atoi_double(rgb[1]), 0, 1, 0, 255);
	colors->b = scale(atoi_double(rgb[2]), 0, 1, 0, 255);
	free_array(rgb);
}

void	set_camera(char **line, t_camera *camera)
{
	camera->fov = ft_atoi(line[3]);
	set_direction(line[1], &camera->position);
	set_direction(line[2], &camera->orientation);
}

void	ft_add_back_l(t_light **list, t_light *new)
{
	t_light	*temp;

	if (new == NULL)
		return ;
	if (*list == NULL)
	{
		*list = new;
		new->next = NULL;
	}
	else
	{
		temp = *list;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
		new->next = NULL;
	}
}

void	set_light(char **line, t_light **list)
{
	t_light	*light;

	light = (t_light *)malloc(sizeof(t_light));
	if (!light)
		return ;
	set_direction(line[1], &light->position);
	light->brightness = atoi_double(line[2]);
	set_rgb(line[3], &light->color);
	light->next = NULL;
	ft_add_back_l(list, light);
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

int	parse_triangle(t_rt *rt, char *line)
{
	char	**line_data;

	line_data = ft_split(line, ' ');
	if (line_data[5])
		return (free_array(line_data), arg_error("triangle"));
	if (check_range(line_data[1], 3, NULL)
		&& check_range(line_data[2], 3, NULL)
		&& check_range(line_data[3], 3, NULL)
		&& check_range(line_data[4], 3, (int[]){0, 255}))
	{
		set_triangle(line_data, &rt->object);
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
	if (line_data[4])
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

void	parsing_error(char *msg)
{
	ft_putstr_fd("Error: ", 2);
	ft_putendl_fd(msg, 2);
}

int	valid_line(char *arg)
{
	if (!arg)
		return (0);
	if (!ft_strncmp(arg, "A", 0) || !ft_strncmp(arg, "C", 0)
		|| !ft_strncmp(arg, "L", 0) || !ft_strncmp(arg, "sp", 0)
		|| !ft_strncmp(arg, "pl", 0) || !ft_strncmp(arg, "cy", 0)
		|| !ft_strncmp(arg, "#", 1) || !ft_strncmp(arg, "tr", 0))
		return (1);
	return (0);
}

int	parse_obj(t_rt *rt, char *first_arg, char *line)
{
	if (!ft_strncmp(first_arg, "sp", ft_strlen("sp"))
		&& !parse_sphere(rt, line))
		return (0);
	if (!ft_strncmp(first_arg, "pl", ft_strlen("pl"))
		&& !parse_plane(rt, line))
		return (0);
	if (!ft_strncmp(first_arg, "cy", ft_strlen("cy"))
		&& !parse_cylinder(rt, line))
		return (0);
	if (!ft_strncmp(first_arg, "tr", ft_strlen("tr"))
		&& !parse_triangle(rt, line))
		return (0);
	return (1);
}

static int	process_line(t_rt *rt, char *line)
{
	char	*first_arg;
	char	*sp;

	if (line[0] == '\n' || line[0] == '\0')
	{
		free(line);
		return (0);
	}
	sp = ft_strchr(line, ' ');
	if (sp)
		first_arg = ft_substr(line, 0, sp - line);
	else
		first_arg = ft_strdup(line);
	if (!first_arg || !valid_line(first_arg))
		return (free(line), free(first_arg), parsing_error("INVALID LINE"), 1);
	if (!ft_strncmp(first_arg, "A", ft_strlen("A")) && !parse_ambient(rt, line))
		return (free(line), free(first_arg), parsing_error("AMBIENT"), 1);
	if (!ft_strncmp(first_arg, "C", ft_strlen("C")) && !parse_camera(rt, line))
		return (free(line), free(first_arg), parsing_error("CAMERA"), 1);
	if (!ft_strncmp(first_arg, "L", ft_strlen("L")) && !parse_light(rt, line))
		return (free(line), free(first_arg), parsing_error("LIGHT"), 1);
	if (!parse_obj(rt, first_arg, line))
		return (free(line), free(first_arg), parsing_error("OBJECT"), 1);
	return (free(first_arg), free(line), 0);
}

int	parse(t_rt *rt)
{
	char	*line;

	rt->object = NULL;
	line = get_next_line(rt->file_fd);
	while (line != NULL)
	{
		if (process_line(rt, line))
			return (1);
		line = get_next_line(rt->file_fd);
	}
	return (0);
}
