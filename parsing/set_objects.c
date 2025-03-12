#include "../minirt.h"
/* 
void	set_properties(char *line, t_sphere *sphere)
{
	char	**properties;

	properties = ft_split(line, ',');
	if (!properties)
		return;
	sphere->specular = atoi_double(properties[0]);
	sphere->reflective = atoi_double(properties[1]);
	sphere->transparency = atoi_double(properties[2]);
	sphere->refractive_index = atoi_double(properties[3]);
	ft_free_split(properties);
} */

void	set_cylinder(char **line, t_object **list)
{
	t_cylinder	*cylinder;
	t_object	*new_object;

	cylinder = (t_cylinder *)malloc(sizeof(t_cylinder));
	if (!cylinder)
		return;
	cylinder->diameter = atoi_double(line[3]);
	cylinder->height = atoi_double(line[4]);
	set_direction(line[1], &cylinder->position);
	set_direction(line[2], &cylinder->direction);
	// set_properties(line[6], &cylinder);
	set_rgb(line[5], &cylinder->color);
	// *cylinder = transform_cylinder(cylinder, 0.3);
	new_object = (t_object *)malloc(sizeof(t_object));
	if (!new_object)
		return;
	new_object->type = CYLINDER;
	new_object->object = (void *)cylinder;
	new_object->color = cylinder->color;
	new_object->position = cylinder->position;
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
	// set_properties(line[5], &plane);
	set_rgb(line[3], &plane->color);
	// *plane = transform_plane(plane, 0.3);
	new_object = (t_object *)malloc(sizeof(t_object));
	if (!new_object)
		return;
	new_object->type = PLANE;
	new_object->object = (void *)plane;
	new_object->color = plane->color;
	new_object->position = plane->position;
	new_object->next = NULL;
	ft_add_back(list, new_object, PLANE);
}

void set_sphere(char **line, t_object **list)
{
	t_sphere *sphere;
	t_object *new_object;

	sphere = (t_sphere *)malloc(sizeof(t_sphere));
	if (!sphere)
		return;
	sphere->diameter = atoi_double(line[2]);
	set_direction(line[1], &sphere->position);
	set_rgb(line[3], &sphere->color);
	// set_properties(line[4], &sphere);
	// *sphere = transform_sphere(sphere, 0.3);
	new_object = (t_object *)malloc(sizeof(t_object));
	if (!new_object)
		return;
	new_object->type = SPHERE;
	new_object->object = (void *)sphere;
	new_object->color = sphere->color;
	new_object->position = sphere->position;
	new_object->next = NULL;
	ft_add_back(list, new_object, SPHERE);
}