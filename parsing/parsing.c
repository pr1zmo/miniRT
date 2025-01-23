/*
Parsing rules:
- Each line Starts with a letter that defines the type of the element.
- Each element is separated by a space.
- Each element has a specific number of parameters.
Mandatory elements:
- Ambient lighting: A 0.0 to 1.0 ratio. Example: A 0.2 255,255,255
- Camera: The position of the camera, the normalized direction vector, and the field of view in degrees. Example: C 0,0,0 0,0,1 60
- Light: The position of the light, the brightness ratio, and the color in RGB. Example: L 0,0,0 0.6 255,255,255
- Sphere: The position of the sphere, the diameter, and the color in RGB. Example: sp 0,0,20 20 255,0,0
- Plane: The position of the plane, the normalized direction vector, and the color in RGB. Example: pl 0,0,0 0,0,1 255,255,255
- Cylinder: The position of the cylinder, the normalized direction vector, the diameter, the height, and the color in RGB. Example: cy 0,0,0 0,0,1 20 40 255,255,255

File examples:
- A 0.2 255,255,255
- C 0,0,0 0,0,1 60
- L 0,0,0 0.6 255,255,255
- sp 0,0,20 20 255,0,0
- pl 0,0,0 0,0,1 255,255,255
- cy 0,0,0 0,0,1 20 40 255,255,255
 */

#include "../minirt.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *get_next_line(int fd)
{
	char buff[2];
	char *line = NULL;
	char *temp;
	ssize_t bytes_read;

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
	return line;
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
	colors->r = ft_atoi(rgb[1]);
	colors->r = ft_atoi(rgb[2]);
	free_array(rgb);
}

void	set_camera(char **line, t_camera *camera)
{
	camera->fov = ft_atoi(line[2]);
	set_direction(line[1], &camera->orientation);
	set_direction(line[2], &camera->position);
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

int	is_int(const char *c)
{
	int	i;

	i = 0;
	while (c[i])
	{
		if (c[i] < '0' || c[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_range(char *line, int count, const int range[2])
{
	int		i;
	char	**line_arr;

	i = 1;
	line_arr = ft_split(line, ',');
	if (!range)
	{
		while (line_arr[i] && i < count)
		{
			if (!is_int(line_arr[i]))
				return (free_array(line_arr), 0);
			i++;
		}
	}
	else
	{
		i = 1;
		while (line_arr[i] && i < count)
		{
			if (!is_int(line_arr[i])
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
	if (check_range(line_data[1], 1, (int[]){0, 1})
		&& check_range(line_data[2], 3, (int[]){0, 255}))
	{
		set_ambient(line_data, &rt->light.ambient);
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

void	parse_sphere(t_rt *rt, char *line)
{
	(void)rt;
	(void)line;
	//
}

void	parse_plane(t_rt *rt, char *line)
{
	(void)rt;
	(void)line;
	//
}

void	parse_cylinder(t_rt *rt, char *line)
{
	(void)rt;
	(void)line;
	//
}

void	parsing_error(t_rt *rt, char *msg)
{
	ft_putstr_fd("Error: ", 2);
	ft_putendl_fd(msg, 2);
	destroy(rt);
}

int	parse(t_rt *rt)
{
	char	*line;

	while (1)
	{
		line = get_next_line(rt->file_fd);
		if (!line)
			break;
		if (line[0] == 'A' && !parse_ambient(rt, line))
			return (free(line), parsing_error(rt, "AMBIENT"), 1);
		if (line[0] == 'C' && !parse_camera(rt, line))
			return (free(line), parsing_error(rt, "CAMERA"), 1);
		if (line[0] == 'L' && !parse_light(rt, line))
			return (free(line), parsing_error(rt, "LIGHT"), 1);
		if (line[0] == 's' && line[1] == 'p')
			parse_sphere(rt, line);
		else if (line[0] == 'p' && line[1] == 'l')
			parse_plane(rt, line);
		else if (line[0] == 'c' && line[1] == 'y')
			parse_cylinder(rt, line);
		free(line);
	}
	free(line);
	return (0);
}