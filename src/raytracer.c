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

int	compute_sphere_lighting(t_rt *rt, t_vector point, t_vector normal, t_sphere *sphere)
{
	(void)normal;
	t_vector	light_dir;
	double		intensity;
	t_color		color;

	light_dir = vector_normalize(vector_subtract(rt->light.position, point));
	intensity = fmax(rt->light.brightness, vector_dot(normal, light_dir));
	color = sphere->color;
	color.r *= intensity;
	color.g *= intensity;
	color.b *= intensity;
	return ((int)color.r << 16 | (int)color.g << 8 | (int)color.b);
}

void	debug(t_ray *ray)
{
	if (k == 0)
		dprintf(global_fd, "ray origin: %f %f %f\n", ray->origin.x, ray->origin.y, ray->origin.z);
	k++;
	dprintf(global_fd, "ray direction: %f %f %f\n", ray->direction.x, ray->direction.y, ray->direction.z);
}

/*
t_hit_info {
	int			hit_count;
	double		dist;
	t_vector	hit_point;
	t_vector	normal;
	t_color		color;
	t_object	*closest_object;
}
*/

t_hit_info	*get_hit_info(t_ray *ray, t_rt *rt, int x, int y)
{
	t_object	*temp;
	t_hit_info	*result;
	int			t;

	t = 0;
	temp = rt->object;
	while (temp)
	{
		if (temp->type == SPHERE && sphere_intersect(temp, ray, x, y))
			result = set_hit(x, y);
		else if (temp->type = PLANE && plane_intersect(temp, ray, x, y))
			result = set_hit(x, y);
		else if (temp->type = CYLINDER && cylinder_intersect(temp, ray, x, y))
			result = set_hit(x, y);
		temp = temp->next;
	}
}

int intersect(t_rt *rt, int x, int y)
{
	t_sphere	*sphere = (t_sphere *)rt->object->object;
	double		t;
	t_hit_info	*hit_info;
	t_ray		ray;

	ray = get_ray(rt, x, y);
	hit_info = get_hit_info(&ray, rt, x, y);
	return (compute_lighting(rt, hit_info->hit_point, hit_info->normal, sphere));
}

void	r_trace(t_rt *rt, int x, int y)
{
	int	color;

	color = intersect(rt, x, y);
	if (color >= 0)
		my_mlx_pixel_put(&rt->img, x, y, color);
}
