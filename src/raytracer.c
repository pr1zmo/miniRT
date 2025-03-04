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

t_ray get_ray(t_rt *rt, int x, int y, t_vector origin, t_vector orientation)
{
	t_ray		ray;
	t_vector	right, up, forward, screen_pixel;
	double		px_x, px_y, fov_adjustment;

	forward = vec_norm(orientation);
	ray.origin = origin;
	right = vec_norm(vec_cro(forward, (t_vector){0, 1, 0}));
	up = vec_cro(right, forward);

	fov_adjustment = tan((rt->camera.fov * M_PI / 180.0) / 2.0);

	px_x = (2 * ((x + 0.5) / rt->width) - 1) * ASPECT_RATIO * fov_adjustment;
	px_y = (1 - 2 * ((y + 0.5) / rt->height)) * fov_adjustment;

	screen_pixel = vec_add(vec_add(vec_scale(right, px_x), vec_scale(up, px_y)), forward);
	ray.direction = vec_norm(screen_pixel);
	return (ray);
}

// t_ray get_ray(t_rt *rt, int x, int y)
// {
// 	t_ray		ray;
// 	t_vector	right, up, forward, screen_pixel;
// 	double		px_x, px_y, fov_adjustment;

// 	ray.origin = rt->camera.position;
// 	forward = vec_norm(rt->camera.orientation);

// 	right = vec_norm(vec_cro(forward, (t_vector){0, 1, 0}));
// 	up = vec_cro(right, forward);

// 	fov_adjustment = tan((rt->camera.fov * M_PI / 180.0) / 2.0);

// 	px_x = (2 * ((x + 0.5) / rt->width) - 1) * ASPECT_RATIO * fov_adjustment;
// 	px_y = (1 - 2 * ((y + 0.5) / rt->height)) * fov_adjustment;

// 	screen_pixel = vec_add(vec_add(vec_scale(right, px_x), vec_scale(up, px_y)), forward);
// 	ray.direction = vec_norm(screen_pixel);
// 	return (ray);
// }

/*
	Phong reflection model:
	- Ambient light: light that is scattered in all directions
	- Diffuse light: light that is scattered uniformly in all directions
	- Specular light: light that is reflected in a single direction
	Equation: I = Ia + Id + Is
	- Ia: ambient light
	- Id: diffuse light
	- Is: specular light
	- I: intensity of the light
	formula in code: I = fmax(light.brightness, vec_dot(normal, light_dir))
*/

t_vector reflect_light(t_vector normal, t_vector light_dir)
{
    t_vector	reflect_dir;
    double		dot_product;
    t_vector	scaled_normal;

    dot_product = vec_dot(normal, light_dir);
    scaled_normal = vec_scale(normal, 2.0 * dot_product);
    reflect_dir = vec_sub(scaled_normal, light_dir);
    reflect_dir = vec_norm(reflect_dir);
    return (reflect_dir);
}

t_color	mix_color(t_color c1, float p1, t_color c2, float p2)
{
	t_color	dst;

	dst.r = c1.r * p1 + c2.r * p2;
	dst.g = c1.g * p1 + c2.g * p2;
	dst.b = c1.b * p1 + c2.b * p2;
	return (dst);
}

int	rgb_to_int(t_color color)
{
	return ((int)(color.r) << 16 | (int)(color.g) << 8 | (int)(color.b));
}

t_color phong_model(t_hit_info hit, t_light light, t_camera camera)
{
	t_color	final_color = hit.color;
	(void)camera;
	(void)light;
	(void)hit;
	// t_vector light_dir, view_dir, reflect_dir;
	// t_color ambient, diffuse, specular;
	// double diff_intensity, spec_intensity;
	// double shininess = 32;

	// hit.normal = vec_norm(hit.normal);
	// light_dir = vec_sub(light.position, hit.hit_point);
	// light_dir = vec_norm(light_dir);

	// view_dir = vec_sub(camera.position, hit.hit_point);
	// view_dir = vec_norm(view_dir);

	// ambient.r = hit.color.r * light.color.r * 0.1;
	// ambient.g = hit.color.g * light.color.g * 0.1;
	// ambient.b = hit.color.b * light.color.b * 0.1;

	// diff_intensity = fmax(vec_dot(hit.normal, light_dir), 0.0);
	// diffuse.r = hit.color.r * light.color.r * diff_intensity;
	// diffuse.g = hit.color.g * light.color.g * diff_intensity;
	// diffuse.b = hit.color.b * light.color.b * diff_intensity;

	// reflect_dir = vec_sub(vec_scale(hit.normal, 2 * vec_dot(hit.normal, light_dir)), light_dir);
	// reflect_dir = vec_norm(reflect_dir);
	// spec_intensity = pow(fmax(vec_dot(view_dir, reflect_dir), 0.0), shininess);

	// specular.r = light.color.r * spec_intensity;
	// specular.g = light.color.g * spec_intensity;
	// specular.b = light.color.b * spec_intensity;

	// final_color.r = ambient.r + diffuse.r + specular.r;
	// final_color.g = ambient.g + diffuse.g + specular.g;
	// final_color.b = ambient.b + diffuse.b + specular.b;

	final_color.r = fmax(final_color.r, 255.0);
	final_color.g = fmax(final_color.g, 255.0);
	final_color.b = fmax(final_color.b, 255.0);

	return final_color;
}

double	vec_length(t_vector v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}


// int	compute_illimunation(t_light *light, t_info *info,
// 		t_vars *vars, float *intensity)
// {
// 	t_vec3	lighdir;
// 	t_ray	lightray;
// 	float	lighdist;
// 	int		validint;

// 	// make a normal vector pointing the direction from intpoint to the light 
// 	lighdir = normalize(vec_sub(light->position, info->hitpoint));
// 	// calculate distance so we dont render objects behind the light
// 	lighdist = length(vec_sub(light->position, info->hitpoint));
// 	// make ray (intpoint, direction to light)
// 	lightray = new_ray(info->hitpoint, vec_add(info->hitpoint, lighdir));
// 	// test intersection of all objects in the scene
// 	validint = cast_ray(&lightray, vars, lighdist, info);
// 	if (!validint)
// 	{
// 		// calculate intensity based on angle between normal and light direction
// 		*intensity = light->brightness * fmax(dot_product(lighdir, info->localnormal), 0.0);
// 		return (1);
// 	}
// 	else
// 	{
// 		// means there was an object blocking the light on that point of the object
// 		*intensity = 0.0f;
// 		return (0);
// 	}
// 	return (0);
// }

/*
	intensity = ambient + lights(light_dir*normal)*diffuse_intensity+pow(reflection_vector*view_direction), shininess)*specular_intensity)
*/

t_color	phong(t_rt *rt, t_hit_info *closes_hit, t_color basic)
{
	double	ambient;
	double	diffuse;
	double	specular;

	ambient = rt->ambient.lighting;
	diffuse = 
}

int compute_lighting(t_rt *rt, t_hit_info *closest_hit)
{
	t_color		final_color = {0, 0, 0};
	t_light		*light = rt->light;
	t_object	*obj = closest_hit->closest_object;
	double		coef;
	t_vector	light_dir;
	
	while (light)
	{
		light_dir = vec_norm(vec_sub(light->position, closest_hit->hit_point));
		coef = rt->ambient.lighting * light->brightness * fmax(vec_dot(light_dir, closest_hit->normal), 0.0);
		final_color.r += coef * obj->color.r;
		final_color.g += coef * obj->color.g;
		final_color.b += coef * obj->color.b;
		light = light->next;
	}

	final_color = phong(rt, closest_hit, final_color);

	return (rgb_to_int(final_color));
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

t_color int_to_rgb(int color)
{
	t_color rgb;

	rgb.r = (color >> 16) & 0xFF;
	rgb.g = (color >> 8) & 0xFF;
	rgb.b = color & 0xFF;
	return (rgb);
}

void	r_trace(t_rt *rt, int x, int y)
{
	int	result = 0;

	result = intersect(rt, x, y);
	if (result >= 0)
	{
		if (rt->anti_aliasing && x + 1 < rt->width && y + 1 < rt->height && x > 1 && y > 1)
		{
			// result = compute_pixel_color(rt, x, y);
		}
		my_mlx_pixel_put(&rt->img, x, y, result);
	}
}
