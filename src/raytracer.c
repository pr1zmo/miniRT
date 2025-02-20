#include "../minirt.h"

int	k = 0;
int	z = 0;

int	check_sphere(t_sphere *sphere, int x, int y)
{
	double		r, discriminant, a;
	t_vector	sub;
	t_vector	curr;

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

// Compute the direction of a ray from the camera through a pixel
// void	get_ray(t_rt *rt, t_ray *ray, int x, int y)
// {
// 	double aspect_ratio = (double)rt->width / (double)rt->height;
// 	double fov_adjustment = tan(rt->camera.fov / 2 * M_PI / 180);

// 	double px = (2 * ((x + 0.5) / rt->width) - 1) * aspect_ratio * fov_adjustment;
// 	double py = (1 - 2 * ((y + 0.5) / rt->height)) * fov_adjustment;
	
// 	t_vector ray_dir = vector_normalize((t_vector){px, py, -1});
// 	ray->direction = ray_dir;
// 	ray->origin = rt->camera.position;
// }

void	get_ray(t_rt *rt, t_ray *ray, int x, int y)
{
	double aspect_ratio = (double)rt->width / (double)rt->height;
	double fov_adjustment = tan(rt->camera.fov / 2 * M_PI / 180);

	double px = (2 * ((x + 0.5) / rt->width) - 1) * aspect_ratio * fov_adjustment;
	double py = (1 - 2 * ((y + 0.5) / rt->height)) * fov_adjustment;
	
	t_vector ray_dir = vector_normalize((t_vector){px, py, -1});
	ray->direction = ray_dir;
	ray->origin = rt->camera.position;
}

// Sphere-ray intersection
int sphere_intersect(t_sphere *sphere, t_ray *ray, double *t)
{
	t_vector oc = vector_subtract(ray->origin, sphere->position);
	double a = vector_dot(ray->direction, ray->direction);
	double b = 2 * vector_dot(oc, ray->direction);
	double c = vector_dot(oc, oc) - (sphere->diameter / 2) * (sphere->diameter / 2);
	double discriminant = b * b - 4 * a * c;
	
	if (discriminant < 0)
		return (0);
	
	double t0 = (-b - sqrt(discriminant)) / (2 * a);
	double t1 = (-b + sqrt(discriminant)) / (2 * a);
	
	*t = (t0 > 0) ? t0 : t1;
	return (*t > 0);
}

// Compute shading using Lambert's law
int compute_lighting(t_rt *rt, t_vector point, t_vector normal, t_sphere *sphere)
{
	t_vector light_dir = vector_normalize(vector_subtract(rt->light.position, point));
	double intensity = fmax(0, vector_dot(normal, light_dir));
	
	t_color color = sphere->color;
	color.r *= intensity;
	color.g *= intensity;
	color.b *= intensity;
	
	return ((int)color.r << 16 | (int)color.g << 8 | (int)color.b);
}

void	debug(t_ray *ray)
{
	printf("ray origin: %f %f %f\n", ray->origin.x, ray->origin.y, ray->origin.z);
	printf("ray direction: %f %f %f\n", ray->direction.x, ray->direction.y, ray->direction.z);
}

// Check for intersection and return pixel color
// int intersect(t_rt *rt, int x, int y)
// {
// 	t_sphere	*sphere = (t_sphere *)rt->object->object;
// 	// debug(rt->ray);
// 	double	t;
// 	(void)x;
// 	(void)y;

// 	if (sphere_intersect(sphere, rt->ray, &t))
// 	{
// 		t_vector hit_point = {rt->ray->origin.x + t * rt->ray->direction.x, 
// 							rt->ray->origin.y + t * rt->ray->direction.y, 
// 							rt->ray->origin.z + t * rt->ray->direction.z};
// 		t_vector normal = vector_normalize(vector_subtract(hit_point, sphere->position));
// 		return compute_lighting(rt, hit_point, normal, sphere);
// 	}
// 	return (-1);
// }

int intersect(t_rt *rt, int x, int y)
{
	t_vector	hit_point;
	t_vector	normal;
	t_sphere	*sphere;
	double		t;
	
	sphere = (t_sphere *)rt->object->object;
	get_ray(rt, rt->ray, x, y);
	if (sphere_intersect(sphere, rt->ray, &t))
	{
		hit_point.x = rt->ray->origin.x + t * rt->ray->direction.x;
		hit_point.y = rt->ray->origin.y + t * rt->ray->direction.y;
		hit_point.z = rt->ray->origin.z + t * rt->ray->direction.z;
		normal = vector_normalize(vector_subtract(hit_point, sphere->position));
		return (compute_lighting(rt, hit_point, normal, sphere));
	}
	return (-1);
}

// Cast ray and set pixel color
void r_trace(t_rt *rt, int x, int y)
{
	int color;
	
	color = intersect(rt, x, y);
	if (color >= 0)
		my_mlx_pixel_put(&rt->img, x, y, color);
}
