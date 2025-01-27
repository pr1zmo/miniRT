#include "../minirt.h"

void	show_sphere(t_sphere *sphere)
{
	printf("Sphere: %p\n", sphere);
	printf("Into the sphere\n");
	printf("Sphere: position=(%.2f, %.2f, %.2f)\n\tdiameter=%.2f\n\tcolor=(%.2f, %.2f, %.2f)\n",
		sphere->position.x, sphere->position.y, sphere->position.z,
		sphere->diameter, sphere->color.r, sphere->color.g, sphere->color.b);
}

void	show_plane(t_object *object)
{
	t_plane	*plane;

	plane = (t_plane *)object->object;
	printf("Plane: %p\n", object);
	printf("Plane: position=(%.2f, %.2f, %.2f)\n\tdirection=(%.2f, %.2f, %.2f)\n\tcolor=(%.2f, %.2f, %.2f)\n",
		plane->position.x, plane->position.y, plane->position.z,
		plane->direction.x, plane->direction.y, plane->direction.z,
		plane->color.r, plane->color.g, plane->color.b);
}

void	show_cylinder(t_object *object)
{
	t_cylinder	*clynder;

	clynder = (t_cylinder *)object->object;
	printf("Cylinder: %p\n", object);
	printf("Cylinder: position=(%.2f, %.2f, %.2f)\n\tdirection=(%.2f, %.2f, %.2f)\n\tdiameter=%.2f\n\theight=%.2f\n\tcolor=(%.2f, %.2f, %.2f)\n",
		clynder->position.x, clynder->position.y, clynder->position.z,
		clynder->direction.x, clynder->direction.y, clynder->direction.z,
		clynder->diameter, clynder->height,
		clynder->color.r, clynder->color.g, clynder->color.b);
}

void	list_objects(t_rt *rt)
{
	t_object	*object;
	t_camera	camera;

	camera = rt->camera;
	printf("camera fov: %f\n\tcamera'a orientation: {%f, %f, %f}\n\tcamera's position: {%f, %f, %f}\n\t", camera.fov, camera.orientation.x, camera.orientation.y, camera.orientation.x, camera.position.x, camera.position.y, camera.position.z);
	object = rt->object;
	if (!object)
	{
		printf("No objects\n");
		return;
	}
	int i = 0;
	while (i < rt->object_count)
	{
		printf("The type from the list_object function: %d\n", object->type);
		if (object->type == SPHERE)
			show_sphere(object->object);
		else if (object->type == PLANE)
			show_plane(object);
		else if (object->type == CYLINDER)
			show_cylinder(object);
		else
			printf("Unknown object\n");
		object = object->next;
		i++;
	}
}
