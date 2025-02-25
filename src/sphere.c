#include "../minirt.h"

int	sphere_intersect(t_object *object, t_ray ray, double *t)
{
	t_sphere	*sphere;
	t_vector	oc;
	double		a, b, c, discriminant, t1, t0;

	sphere = (t_sphere *)object->object;
	oc = vector_subtract(ray.origin, sphere->position);
	a = vector_dot(ray.direction, ray.direction);
	b = 2 * vector_dot(oc, ray.direction);
	c = vector_dot(oc, oc) - (sphere->diameter / 2) * (sphere->diameter / 2);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);
	t0 = (-b - sqrt(discriminant)) / (2 * a);
	t1 = (-b + sqrt(discriminant)) / (2 * a);
	if (t0 > t1)
		t0 = t1;
	if (t0 < 0)
		return (0);
	*t = t0;
	return (*t > 0);
}