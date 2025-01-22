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

char	*get_next_line(int fd)
{
	static char	*remaining;
	char		buff[2];
	char		*line = NULL;
	char		*newline;
	ssize_t		bytes_read;

	while ((bytes_read = read(fd, buff, 1)) > 0)
	{
		buff[1] = '\0';
		if (!line)
			line = ft_strdup(buff);
		else
			line = ft_strjoin(line, buff);
		if (buff[0] == '\n')
			break;
	}
	if (bytes_read < 0 || !line)
		return (free(line), NULL);
	newline = ft_strchr(line, '\n');
	if (newline)
	{
		*newline = '\0';
		remaining = ft_strdup(newline + 1);
	}
	return (line);
}

void	parse_ambient(t_rt *rt, char *line)
{
	(void)rt;
	(void)line;
	//
}

void	parse_camera(t_rt *rt, char *line)
{
	(void)rt;
	(void)line;
	//
}

void	parse_light(t_rt *rt, char *line)
{
	(void)rt;
	(void)line;
	//
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

int	parse(t_rt *rt)
{
	char	*line;
	int		i;

	i = 0;
	while ((line = get_next_line(rt->file_fd)) != NULL)
	{
		if (line[0] == 'A')
			parse_ambient(rt, line);
		else if (line[0] == 'C')
			parse_camera(rt, line);
		else if (line[0] == 'L')
			parse_light(rt, line);
		else if (line[0] == 's' && line[1] == 'p')
			parse_sphere(rt, line);
		else if (line[0] == 'p' && line[1] == 'l')
			parse_plane(rt, line);
		else if (line[0] == 'c' && line[1] == 'y')
			parse_cylinder(rt, line);
		else
			return (1);
		free(line);
		i++;
	}
	return (0);
}