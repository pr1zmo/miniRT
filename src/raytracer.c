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

t_ray	get_ray(t_rt *rt, int x, int y, t_ray temp_ray)
{
	t_ray		ray;
	t_vector	right, up, forward, screen_pixel;
	double		px_x, px_y, fov_adjustment;
	t_vector	origin = temp_ray.origin;
	t_vector	orientation = temp_ray.orientation;

	forward = vec_norm(orientation);
	ray.origin = origin;
	right = vec_norm(vec_cro(forward, (t_vector){0, 1, 0}));
	up = vec_cro(right, forward);

	fov_adjustment = tan(rt->camera.fov * M_PI / 180.0) / 2;

	px_x = (2 * ((x + .5) / rt->width) - 1) * ASPECT_RATIO * fov_adjustment;
	px_y = (1 - 2 * ((y + .5) / rt->height)) * fov_adjustment;

	screen_pixel = vec_add(vec_add(vec_scale(right, px_x), vec_scale(up, px_y)), forward);
	ray.direction = vec_norm(screen_pixel);
	return (ray);
}

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

int	rgb_to_int(t_color c)
{
	int	color;

	color = (int)(c.r * 255) << 16;
	color += (int)(c.g * 255) << 8;
	color += (int)(c.b * 255);
	return (color);
}

double	vec_length(t_vector v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

t_vector	scale_vector(double x, t_vector vec)
{
	t_vector	result;

	result.x = vec.x * x;
	result.y = vec.y * x;
	result.z = vec.z * x;
	return (result);
}

/*
3️⃣ Specular Lighting (Shiny Highlights)
Ispecular=ksIlmax⁡(0,R⋅V)α
Ispecular​=ks​Il​max(0,R⋅V)α

    sk​ = Specular reflectivity (material property, range [0,1])
    α = Shininess factor (higher values create sharper highlights)
    V = View direction (unit vector from surface to camera)
    R = Reflection vector of LL about NN, calculated as:
    R=2(N⋅L)N−L
*/

t_color	normalize_color(t_color color)
{
	t_color	normalized;
	
	normalized.r = color.r / 255.0;
	normalized.g = color.g / 255.0;
	normalized.b = color.b / 255.0;
	return (normalized);
}

/*
t_color	diffuse_light(t_rays *r, t_light *light)
{
	t_color	color;
	float	dot_p;
	
	color = r->hit.color;
	dot_p = dot_prod(r->shadowray.dir, r->hit.nhit);
	color = add_light(color, light->color, light->brightness * dot_p);
	return (color);
	}
	*/

int compute_lighting(t_rt *rt, t_hit_info *closest_hit)
{
	t_color final_color = {0, 0, 0};
	t_light *light = rt->light;
	t_object *obj = closest_hit->closest_object;

	t_color obj_color = normalize_color(obj->color);
	final_color.r = rt->ambient.color.r * rt->ambient.lighting * obj_color.r;
	final_color.g = rt->ambient.color.g * rt->ambient.lighting * obj_color.g;
	final_color.b = rt->ambient.color.b * rt->ambient.lighting * obj_color.b;
	while (light)
	{
		t_vector light_dir = vec_norm(vec_sub(light->position, closest_hit->hit_point));
		double diffuse_coef = vec_dot(light_dir, closest_hit->normal) * light->brightness;
		if (diffuse_coef > 0)
		{
			final_color.r += diffuse_coef * light->color.r * obj_color.r;
			final_color.g += diffuse_coef * light->color.g * obj_color.g;
			final_color.b += diffuse_coef * light->color.b * obj_color.b;
		}
		
		// printf("Current data: diffuse_coef: %f, final_color: {%f, %f, %f}\n", diffuse_coef, final_color.r * 255, final_color.g * 255, final_color.b * 255);
		light = light->next;
	}
	final_color.r = fmin(1, final_color.r) * 255;
	final_color.g = fmin(1, final_color.g) * 255;
	final_color.b = fmin(1, final_color.b) * 255;
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
	my_mlx_pixel_put(&rt->img, x, y, result);
	// if (result >= 0)
	// {
	// 	if (rt->anti_aliasing)
	// 	{
	// 		// result = compute_pixel_color(rt, x, y);
	// 	}
	// }
}
