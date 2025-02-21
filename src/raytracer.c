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

t_vector	scale_vector(double s, t_vector v)
{
	t_vector	result;

	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;
	return (result);
}

t_vector	vector_cross(t_vector a, t_vector b)
{
	t_vector	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}

t_ray get_ray(t_rt *rt, int x, int y)
{
    double aspect_ratio = (double)rt->width / rt->height;
    double fov_adjustment = tan(rt->camera.fov * M_PI / 180 / 2);

    // Map pixel coordinate (x,y) to normalized device coordinates [-1, 1]
    double ndc_x = (2 * ((x + 0.5) / rt->width) - 1) * aspect_ratio * fov_adjustment;
    double ndc_y = (1 - 2 * ((y + 0.5) / rt->height)) * fov_adjustment;

    // Compute camera basis vectors
    t_vector forward = vector_normalize(vector_subtract(rt->camera.orientation, rt->camera.position));
    t_vector right = vector_normalize(vector_cross(forward, (t_vector){0, 1, 0}));
    t_vector up = vector_cross(right, forward);

    // Compute the ray direction in world space
    t_vector ray_dir = vector_normalize(vector_add(forward, vector_add(scale_vector(ndc_x, right),
                                                     scale_vector(ndc_y, up))));
    
    return (t_ray){rt->camera.position, ray_dir};
}

int	sphere_intersect(t_sphere *sphere, t_ray ray, double *t)
{
	t_vector	oc;
	double		a, b, c, discriminant, t1, t0;

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

int	compute_lighting(t_rt *rt, t_vector point, t_vector normal, t_sphere *sphere)
{
	(void)normal;
	t_vector	light_dir;
	// double		intensity;
	t_color		color;

	light_dir = vector_normalize(vector_subtract(rt->light.position, point));
	// intensity = fmax(0, vector_dot(normal, light_dir));
	color = sphere->color;
	color.r *= rt->light.brightness;
	color.g *= rt->light.brightness;
	color.b *= rt->light.brightness;
	return ((int)color.r << 16 | (int)color.g << 8 | (int)color.b);
}

void	debug(t_ray *ray)
{
	if (k == 0)
		dprintf(global_fd, "ray origin: %f %f %f\n", ray->origin.x, ray->origin.y, ray->origin.z);
	k++;
	dprintf(global_fd, "ray direction: %f %f %f\n", ray->direction.x, ray->direction.y, ray->direction.z);
}

int intersect(t_rt *rt, int x, int y)
{
    t_sphere *sphere = (t_sphere *)rt->object->object;
    double t;
    t_ray ray = get_ray(rt, x, y);  // Use a local variable
    
    if (sphere_intersect(sphere, ray, &t))
    {
		if (t == -1)
			return (0xffffff);
        t_vector hit_point = vector_add(ray.origin, scale_vector(t, ray.direction));
        t_vector normal = vector_normalize(vector_subtract(hit_point, sphere->position));
        return (compute_lighting(rt, hit_point, normal, sphere));
    }
    return (0);
}

void	r_trace(t_rt *rt, int x, int y)
{
	int	color;

	color = intersect(rt, x, y);
	if (color >= 0)
		my_mlx_pixel_put(&rt->img, x, y, color);
}
