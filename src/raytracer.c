#include "../minirt.h"

//HEIGHT: X
//WIDHT: Y

int	k = 0;
int	z = 0;


int	sphere_intersect(t_sphere *sphere, int x, int y)
{
	double	r;
	double	x1;
	double	y1;

	r = sphere->diameter / 2;
	x1 = sphere->position.x;
	y1 = sphere->position.y;

	if ((x - x1) * (x - x1) + (y - y1) * (y - y1) <= r * r)
		return (1);
	return (0);
}

int	check_sphere(t_rt *rt, int x, int y)
{
	t_sphere	*sphere;
	// t_ray		camera_ray;
	// t_vector	pixel_pos;
	// t_vector	ray_direction;

	sphere = (t_sphere *)rt->object->object;

	// pixel_pos = (t_vector){(double)x, (double)y, 1.0}; // Position in screen space
	// ray_direction = normalize(subtract(pixel_pos, rt->camera.position)); // Correct direction

	// camera_ray = (t_ray){rt->camera.position, ray_direction}; // Assign to ray
	if (sphere_intersect(sphere, x, y))
	{
		return (1);
	}
	return (0);
}


// int	check_plane(t_object *object, int x, int y)
// {
// 	(void)y;
// 	t_plane	*plane;

// 	plane = (t_plane *)object;
// 	if (x >= plane->position.x && x < plane->position.y)
// 	{
// 		return (1);
// 	}
// 	return (0);
// }

// int	check_cylinder(t_object *object, int x, int y)
// {
// 	(void)y;
// 	t_cylinder	*cylinder;

// 	cylinder = (t_cylinder *)object;
// 	if (x >= cylinder->position.x && x <= cylinder->position.x + cylinder->diameter)
// 	{
// 		return (1);
// 	}
// 	return (0);
// }

/*
int	set_color(t_rt *rt, void *object, int x, int y)
{
	t_color		color;
	t_ambient	ambient;
	int			int_color;
	t_sphere	*sphere;
	double		angle;
	(void)rt;

	ambient = rt->ambient;
	sphere = (t_sphere *)object;
	// angle = dot((t_vector){0, 0, 1}, (t_vector){x - sphere->position.x, y - sphere->position.y, 0});
	// printf("angle: %d\n", angle);
	color = sphere->color;
	color.r = color.r * (ambient.lighting * angle) > 255 ? 255 : color.r * (ambient.lighting * angle);
	color.g = color.g * (ambient.lighting * angle) > 255 ? 255 : color.g * (ambient.lighting * angle);
	color.b = color.b * (ambient.lighting * angle) > 255 ? 255 : color.b * (ambient.lighting * angle);
	int_color = (int)color.r << 16 | (int)color.g << 8 | (int)color.b;
	return (int_color);	
}
*/

double scale(int number, int low, int high, int old_low, int old_high)
{
	double result;

	// Ensure the old range is not zero to avoid division by zero.
	if (old_high == old_low)
		return 0; // or handle the error as needed

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

t_vector reflect(t_vector I, t_vector N) {
	// R = I - 2 * (I dot N) * N
	double dotIN = vector_dot(I, N);
	t_vector R = vector_subtract(I, scale_vector(2.0 * dotIN, N));
	return R;
}

double reflection_angle(t_vector incident, t_vector normal, t_vector lightPos, t_vector intersection) {
	// Compute reflection vector R from the incident vector and the surface normal
	t_vector R = reflect(incident, normal);
	R = vector_normalize(R);

	// Compute the direction from the intersection point to the light
	t_vector L = vector_subtract(lightPos, intersection);
	L = vector_normalize(L);

	// Compute the cosine of the angle between R and L
	double cosAngle = vector_dot(R, L);
	// Clamp the value to the valid range for acos
	if (cosAngle > 1.0) cosAngle = 1.0;
	if (cosAngle < -1.0) cosAngle = -1.0;

	// The angle in radians
	double angle = acos(cosAngle);
	return (angle);
}

#include <math.h>

int set_color(t_rt *rt, void *object, int x, int y)
{
	t_color		color;
	t_sphere	*sphere;
	t_vector	normal, light_dir, view_dir, reflect_dir;
	double		diffuse, specular, intensity;
	int			int_color;
	double		shine = 50; // Shininess factor

	sphere = (t_sphere *)object;
	t_vector intersection = (t_vector){(double)x, (double)y, 0}; // Approximate
	normal = vector_normalize(vector_subtract(intersection, sphere->position));
	light_dir = vector_normalize(vector_subtract(rt->light.position, intersection));
	view_dir = vector_normalize(vector_subtract(rt->camera.position, intersection));
	reflect_dir = vector_subtract(scale_vector(2 * vector_dot(normal, light_dir), normal), light_dir);
	diffuse = fmax(0, vector_dot(normal, light_dir));
	specular = pow(fmax(0, vector_dot(reflect_dir, view_dir)), shine);
	intensity = rt->ambient.lighting + diffuse + specular;
	color = sphere->color;
	color.r = fmin(255, color.r * intensity);
	color.g = fmin(255, color.g * intensity);
	color.b = fmin(255, color.b * intensity);
	int_color = ((int)color.r << 16) | ((int)color.g << 8) | (int)color.b;
	return (int_color);
}




int	intersect(t_rt *rt, int x, int y)
{
	t_object	*temp;

	temp = rt->object;
	while (temp)
	{
		if (check_sphere(rt, x, y))
		{
			return (set_color(rt, temp->object, x, y));
		}
		// if (check_plane(temp->object, x, y))
		// {
		// 	return (set_color(temp->object));
		// }
		// if (check_cylinder(temp->object, x, y))
		// {
		// 	return (set_color(temp->object));
		// }
		temp = temp->next;
	}
	return (-1);
}

void	r_trace(t_rt *rt, int x, int y)
{
	int	color;

	color = intersect(rt, x, y);
	if (color > 0)
		my_mlx_pixel_put(&rt->img, y, x, color);
}