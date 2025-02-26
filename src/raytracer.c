#include "../minirt.h"

int	k = 0;
int	z = 0;

double	scale(int number, int low, int high, int old_low, int old_high)
{
	double	result;

	if (old_high == old_low)
		return 0;
	result = low + ((double)(number - old_low) * (high - low)) / (old_high - old_low);
	return (result);
}

// int	compute_sphere_lighting(t_rt *rt, t_vector point, t_vector normal, t_sphere *sphere)
// {
// 	(void)normal;
// 	t_vector	light_dir;
// 	double		intensity;
// 	t_color		color;

// 	light_dir = vector_normalize(vec_sub(rt->light.position, point));
// 	intensity = fmax(rt->light.brightness, vector_dot(normal, light_dir));
// 	color = sphere->color;
// 	color.r *= intensity;
// 	color.g *= intensity;
// 	color.b *= intensity;
// 	return ((int)color.r << 16 | (int)color.g << 8 | (int)color.b);
// }

// void	debug(t_ray *ray)
// {
// 	if (k == 0)
// 		dprintf(global_fd, "ray origin: %f %f %f\n", ray->origin.x, ray->origin.y, ray->origin.z);
// 	k++;
// 	dprintf(global_fd, "ray direction: %f %f %f\n", ray->direction.x, ray->direction.y, ray->direction.z);
// }

// /*
// t_hit_info {
// 	int			hit_count;
// 	double		dist;
// 	t_vector	hit_point;
// 	t_vector	normal;
// 	t_color		color;
// 	t_object	*closest_object;
// }
// */

// t_hit_info	*get_hit_info(t_ray *ray, t_rt *rt, int x, int y)
// {
// 	t_object	*temp;
// 	t_hit_info	*result;
// 	int			t;

// 	t = 0;
// 	temp = rt->object;
// 	while (temp)
// 	{
// 		if (temp->type == SPHERE && sphere_intersect(temp, ray, x, y))
// 			result = set_hit(x, y);
// 		else if (temp->type = PLANE && plane_intersect(temp, ray, x, y))
// 			result = set_hit(x, y);
// 		else if (temp->type = CYLINDER && cylinder_intersect(temp, ray, x, y))
// 			result = set_hit(x, y);
// 		temp = temp->next;
// 	}
// }

// int intersect(t_rt *rt, int x, int y)
// {
// 	t_sphere	*sphere = (t_sphere *)rt->object->object;
// 	double		t;
// 	t_hit_info	*hit_info;
// 	t_ray		ray;

// 	ray = get_ray(rt, x, y);
// 	hit_info = get_hit_info(&ray, rt, x, y);
// 	return (compute_lighting(rt, hit_info->hit_point, hit_info->normal, sphere));
// }

int	plane_intersect(t_object *object, t_ray *ray, int *t)
{
	t_plane	*plane;
	double	denom;

	plane = (t_plane *)object->object;
	denom = vec_dot(vec_norm(plane->position), ray->direction);
	if (fabs(denom) > 0.0001)
	{
		*t = vec_dot(vec_sub(plane->position, ray->origin), vec_norm(plane->position)) / denom;
		if (*t >= 0)
			return (1);
	}
	return (0);
}

int	sphere_intersect(t_object *object, t_ray *ray, int *t)
{
	t_sphere	*sphere = (t_sphere *)object->object;
	t_vector	oc;
	double		a;
	double		b;
	double		c;
	double		discriminant;

	oc = vec_sub(ray->origin, sphere->position);
	a = vec_dot(ray->direction, ray->direction);
	b = 2.0 * vec_dot(oc, ray->direction);
	c = vec_dot(oc, oc) - (sphere->diameter * sphere->diameter);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);
	*t = (-b - sqrt(discriminant)) / (2 * a);
	if (*t < 0)
		return (0);
	return (1);
}

t_ray	get_ray(t_rt *rt, int x, int y)
{
	t_ray		ray;
	t_vector	right, up, forward, screen_pixel;
	double		px_x, px_y, aspect_ratio, fov_adjustment;

	ray.origin = rt->camera.position;
	forward = vec_norm(rt->camera.orientation);
	right = vec_norm(vec_cro((t_vector){0, 1, 0}, forward));
	up = vec_cro(forward, right);
	aspect_ratio = (double)rt->width / (double)rt->height;
	fov_adjustment = tan((rt->camera.fov * M_PI / 180.0) / 2.0);
	px_x = (2 * ((x + 0.5) / rt->width) - 1) * aspect_ratio * fov_adjustment;
	px_y = (1 - 2 * ((y + 0.5) / rt->height)) * fov_adjustment;
	screen_pixel = vec_add(vec_add(vec_scale(right, px_x), vec_scale(up, px_y)), forward);
	ray.direction = vec_norm(screen_pixel);
	return (ray);
}

int	compute_lighting(t_rt *rt, t_ray ray, t_object *object)
{
	t_vector point = vec_add(ray.origin, vec_scale(ray.direction, 1));
	t_vector normal = vec_norm(vec_sub(point, object->position));
	t_vector light_dir = vec_norm(vec_sub(rt->light.position, point));
	double intensity = fmax(rt->light.brightness, vec_dot(normal, light_dir));
	t_color color = object->color;
	color.r *= intensity;
	color.g *= intensity;
	color.b *= intensity;
	return ((int)color.r << 16 | (int)color.g << 8 | (int)color.b);
}

void	set_hit_info(t_hit_info *closest, t_ray *ray, t_object *temp, double t)
{
	closest->hit = 1;
	closest->dist = t;
	closest->closest_object = temp;
	closest->hit_point = vec_add(ray->origin, vec_scale(ray->direction, t));
	closest->normal = vec_norm(vec_sub(closest->hit_point, temp->position));
	closest->color = temp->color;
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
		// if (closest_hit.hit)
		// 	printf("CLoses hit object %d\n", closest_hit.closest_object->type);
		temp = temp->next;
	}
	return (closest_hit);
}


int	intersect(t_rt *rt, int x, int y)
{
	t_ray		ray;
	t_hit_info	closest_hit;

	ray = get_ray(rt, x, y);
	closest_hit = find_closest_object(rt, &ray);
	if (closest_hit.hit)
		return (compute_lighting(rt, ray, closest_hit.closest_object));
	return (-1);
}

void	r_trace(t_rt *rt, int x, int y)
{
	int	color;

	color = intersect(rt, x, y);
	if (color >= 0)
		my_mlx_pixel_put(&rt->img, x, y, color);
}
