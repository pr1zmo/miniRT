#include "../minirt.h"

void	set_cylinder(char **line, t_object *object)
{
	t_cylinder	*cylinder;

	cylinder = (t_cylinder *)malloc(sizeof(t_cylinder));
	cylinder->diameter = atoi_double(line[3]);
	cylinder->height = atoi_double(line[4]);
	set_direction(line[1], &cylinder->position);
	set_direction(line[2], &cylinder->direction);
	set_rgb(line[5], &cylinder->color);
	object->type = CYLINDER;
	object->object = (t_object *)cylinder;
//	object->next = NULL;
}

void	set_plane(char **line, t_object *object)
{
	t_plane	*plane;

	plane = (t_plane *)malloc(sizeof(t_plane));
	set_direction(line[1], &plane->position);
	set_direction(line[2], &plane->direction);
	set_rgb(line[3], &plane->color);
	object = (t_object *)malloc(sizeof(t_object));
	object->type = PLANE;
	object->object = (void *)plane;
	ft_add_back(&object, object);
}

void	set_sphere(char **line, t_object *object)
{
	t_sphere	*sphere;

	sphere = (t_sphere *)malloc(sizeof(t_sphere));
	sphere->diameter = atoi_double(line[2]);
	set_direction(line[1], &sphere->position);
	set_rgb(line[3], &sphere->color);
	object->type = SPHERE;
	object->object = (t_object *)sphere;
//	object->next = NULL;
}
