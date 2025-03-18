#include "../minirt.h"

int	triangle_intersect(t_object *object, t_ray *ray, double *t)
{
	t_triangle	*triangle = (t_triangle *)object->object;
	t_vector	edge1 = vec_sub(triangle->p2, triangle->p1);
	t_vector	edge2 = vec_sub(triangle->p3, triangle->p1);
	t_vector	h = vec_cro(ray->direction, edge2);
	double		a, f, u, v;

	a = vec_dot(edge1, h);
	if (a > -1e-6 && a < 1e-6)
		return (0);
	f = 1 / a;
	t_vector	s = vec_sub(ray->origin, triangle->p1);
	u = f * vec_dot(s, h);
	if (u < 0.0 || u > 1.0)
		return (0);
	t_vector	q = vec_cro(s, edge1);
	v = f * vec_dot(ray->direction, q);
	if (v < 0.0 || u + v > 1.0)
		return (0);
	double t0 = f * vec_dot(edge2, q);
	if (t0 > 1e-6)
	{
		*t = t0;
		return (1);
	}
	return (0);
}