#include "../minirt.h"

int	triangle_intersect(t_object *object, t_ray *ray, int *t)
{
	t_triangle	*triangle = (t_triangle *)object->object;
	t_vector	edge1, edge2, h, s, q;
	double		a, f, u, v;
	double		det;

	edge1 = vec_sub(triangle->b, triangle
}