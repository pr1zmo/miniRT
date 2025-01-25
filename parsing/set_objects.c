#include "../minirt.h"

void	set_cylinder(char **line, t_object **list)
{
	t_cylinder	*cylinder;
	t_object	*new_object;

	cylinder = (t_cylinder *)malloc(sizeof(t_cylinder));
	cylinder->diameter = atoi_double(line[3]);
	cylinder->height = atoi_double(line[4]);
	set_direction(line[1], &cylinder->position);
	set_direction(line[2], &cylinder->direction);
	set_rgb(line[5], &cylinder->color);

	new_object = (t_object *)malloc(sizeof(t_object));
	new_object->type = CYLINDER;
	new_object->object = (void *)cylinder;
	new_object->next = NULL;
	ft_add_back(list, new_object, CYLINDER);
}

void set_plane(char **line, t_object **list)
{
	t_plane		*plane;
	t_object	*new_object;

	plane = (t_plane *)malloc(sizeof(t_plane));
	if (!plane)
		return;
	set_direction(line[1], &plane->position);
	set_direction(line[2], &plane->direction);
	set_rgb(line[3], &plane->color);

	new_object = (t_object *)malloc(sizeof(t_object));
	if (!new_object)
		return;
	new_object->type = PLANE;
	new_object->object = (void *)plane;
	new_object->next = NULL;
	ft_add_back(list, new_object, PLANE);
}

/*
void	set_sphere(char **line, t_object **list)
{
	t_sphere	*sphere;

	sphere = (t_sphere *)malloc(sizeof(t_sphere));
	sphere->diameter = atoi_double(line[2]);
	set_direction(line[1], &sphere->position);
	set_rgb(line[3], &sphere->color);
	ft_add_back(list, (t_object *)sphere, SPHERE);
}*/

void set_sphere(char **line, t_object **list)
{
	t_sphere *sphere;
	t_object *new_object;

	sphere = (t_sphere *)malloc(sizeof(t_sphere));
	sphere->diameter = atoi_double(line[2]);
	set_direction(line[1], &sphere->position);
	set_rgb(line[3], &sphere->color);

	new_object = (t_object *)malloc(sizeof(t_object));
	new_object->type = SPHERE;
	new_object->object = (void *)sphere;
	new_object->next = NULL;

	ft_add_back(list, new_object, SPHERE);
}