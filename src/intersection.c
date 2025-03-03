#include "../minirt.h"

int	intersect(t_rt *rt, int x, int y)
{
	t_ray		ray;
	t_hit_info	closest_hit;

	ray = get_ray(rt, x, y);
	closest_hit = find_closest_object(rt, &ray);
	if (closest_hit.hit)
		return (compute_lighting(rt, &closest_hit, vec_norm(vec_sub(rt->camera.position, closest_hit.hit_point))));
	return (0);
}

t_hit_info find_closest_object(t_rt *rt, t_ray *ray)
{
	t_object	*temp;
	t_hit_info	closest_hit;
	int			t;
	
	closest_hit.hit = 0;
	closest_hit.dist = INFINITY;

	temp = rt->object;
	while (temp)
	{
		if (temp->type == PLANE && plane_intersect(temp, ray, &t))
		{
			if (t > 0 && t < closest_hit.dist)
				set_hit_info(&closest_hit, ray, temp, t);
		}
		if (temp->type == SPHERE && sphere_intersect(temp, ray, &t))
		{
			if (t > 0 && t < closest_hit.dist)
				set_hit_info(&closest_hit, ray, temp, t);
		}
		if (temp->type == CYLINDER && cylinder_intersect(temp, ray, &t))
		{
			if (t > 0 && t < closest_hit.dist)
				set_hit_info(&closest_hit, ray, temp, t);
		}
		temp = temp->next;
	}
	return (closest_hit);
}

int	cylinder_intersect(t_object *object, t_ray *ray, int *t)
{
	t_cylinder	*cylinder = (t_cylinder *)object->object;
	t_vector	oc;
	double		a, b, c, discriminant;

	oc = vec_sub(ray->origin, cylinder->position);
	a = vec_dot(ray->direction, ray->direction) - pow(vec_dot(ray->direction, cylinder->direction), 2);
	b = 2 * (vec_dot(ray->direction, oc) - vec_dot(ray->direction, cylinder->direction) * vec_dot(oc, cylinder->direction));
	c = vec_dot(oc, oc) - pow(vec_dot(oc, cylinder->direction), 2) - pow(cylinder->diameter / 2, 2);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);
	*t = (-b - sqrt(discriminant)) / (2 * a);
	if (*t < 0)
		return (0);
	return (1);
}

int sphere_intersect(t_object *object, t_ray *ray, int *t)
{
	t_sphere *sphere = (t_sphere *)object->object;
	t_vector oc;
	double a, b, c, discriminant;
	double radius = sphere->diameter / 2.0;

	oc = vec_sub(ray->origin, sphere->position);
	a = vec_dot(ray->direction, ray->direction);
	b = 2 * vec_dot(oc, ray->direction);
	c = vec_dot(oc, oc) - (radius * radius);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);
	*t = (-b - sqrt(discriminant)) / (2 * a);
	if (*t < 0)
		return (0);
	return (1);
}

int	plane_intersect(t_object *object, t_ray *ray, int *t)
{
	t_plane	*plane;
	double	denom;

	plane = (t_plane *)object->object;
	denom = vec_dot(vec_norm(plane->direction), ray->direction);
	if (fabs(denom) > 0.0001)
	{
		*t = vec_dot(vec_sub(plane->position, ray->origin), vec_norm(plane->direction)) / denom;
		if (*t >= 0)
			return (1);
	}
	return (0);
}