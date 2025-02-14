#include "../minirt.h"

int	k = 0;
int	z = 0;

int	check_sphere(t_rt *rt, int x, int y)
{
	t_sphere	*sphere;
	double		r, discriminant, a;
	t_vector	sub;
	t_vector	curr;

	sphere = (t_sphere *)rt->object->object;
	curr.x = x;
	curr.y = y;
	curr.z = 1;
	r = sphere->diameter / 2;
	sub = vector_subtract(sphere->position, curr);
	a = vector_dot(sub, sub);
	discriminant = a - pow(r, 2);
	if (discriminant < 0)
		return (1);
	return (0);
}

double scale(int number, int low, int high, int old_low, int old_high)
{
	double result;
	if (old_high == old_low)
		return 0;

	result = low + ((double)(number - old_low) * (high - low)) / (old_high - old_low);
	return result;
}

t_vector scale_vector(double s, t_vector v)
{
	t_vector result;
	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;
	return result;
}

// int set_color(t_rt *rt, void *object, int x, int y)
// {
// 	t_color		color;
// 	t_sphere	*sphere;
// 	t_vector	normal, light_dir, view_dir, reflect_dir;
// 	double		diffuse, specular, intensity;
// 	int			int_color;
// 	double		shine = 50; // Shininess factor

// 	sphere = (t_sphere *)object;
// 	t_vector intersection = (t_vector){(double)x, (double)y, 0}; // Approximate
// 	normal = vector_normalize(vector_subtract(intersection, sphere->position));
// 	light_dir = vector_normalize(vector_subtract(rt->light.position, intersection));
// 	view_dir = vector_normalize(vector_subtract(rt->camera.position, intersection));
// 	reflect_dir = vector_subtract(scale_vector(2 * vector_dot(normal, light_dir), normal), light_dir);
// 	diffuse = fmax(0, vector_dot(normal, light_dir));
// 	specular = pow(fmax(0, vector_dot(reflect_dir, view_dir)), shine);
// 	intensity = rt->ambient.lighting + diffuse + specular;
// 	color = sphere->color;
// 	color.r = fmin(255, color.r * intensity);
// 	color.g = fmin(255, color.g * intensity);
// 	color.b = fmin(255, color.b * intensity);
// 	int_color = ((int)color.r << 16) | ((int)color.g << 8) | (int)color.b;
// 	return (int_color);
// }

int intersect(t_rt *rt, int x, int y)
{
	(void)rt;
	(void)x;
	(void)y;
	return (0xf93955);
}

void	r_trace(t_rt *rt, int x, int y)
{
	int		color;
	// t_ray	ray;

	color = intersect(rt, x, y);
	if (color > 0)
		my_mlx_pixel_put(&rt->img, x, y, color);
	// else
	// {
		// double a = scale(x, 0, 1, 0, HEIGHT);
		// double b = scale(y, 0, 1, 0, WIDTH);
		// double gradient = (a + b) / 2;
		// int red = (int)(0xff * gradient);
		// int green = (int)(0xff * (1 - gradient));
		// int blue = (int)(0xff * gradient);
		// int background_color = (red << 16) | (green << 8) | blue;
		// my_mlx_pixel_put(&rt->img, y, x, background_color);
	// }
}