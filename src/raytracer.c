#include "../minirt.h"

//HEIGHT: X
//WIDHT: Y

int	k = 0;
int	z = 0;

double dot(t_vector a, t_vector b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

// Subtract two vectors: result = a - b
t_vector subtract(t_vector a, t_vector b)
{
    t_vector result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return (result);
}

int	sphere_intersect(t_sphere *sphere, int x, int y)
{
	int	r;
	int	x1;
	int	y1;

	r = sphere->diameter / 2;
	x1 = sphere->position.x;
	y1 = sphere->position.y;

	if ((x - x1) * (x - x1) + (y - y1) * (y - y1) <= r * r)
		return (1);
	return (0);
}

t_vector normalize(t_vector v)
{
	double length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return (t_vector){v.x / length, v.y / length, v.z / length};
}

int	check_sphere(t_rt *rt, int x, int y)
{
	t_sphere	*sphere;
	t_ray		camera_ray;
	t_vector	pixel_pos, ray_direction;

	sphere = (t_sphere *)rt->object->object;

	// Compute ray from camera to pixel
	pixel_pos = (t_vector){(double)x, (double)y, 1.0}; // Position in screen space
	ray_direction = normalize(subtract(pixel_pos, rt->camera.position)); // Correct direction

	camera_ray = (t_ray){rt->camera.position, ray_direction}; // Assign to ray

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

int set_color(t_rt *rt, void *object, int x, int y)
{
    t_color  color;
    t_ambient ambient = rt->ambient;
    t_sphere *sphere = (t_sphere *)object;
    t_vector intersection, normal, light_dir;
    double angle;

    // Compute intersection point assuming ray = (camera.position, direction)
    t_vector pixel_pos = (t_vector){(double)x, (double)y, 1.0}; 
    intersection = pixel_pos; // Approximate, needs actual ray intersection

    // Compute normal at intersection point
    normal = normalize(subtract(intersection, sphere->position));

    // Compute light direction
    light_dir = normalize(subtract(rt->light.position, intersection));

    // Compute Lambertian angle (clamped to [0,1])
    angle = fmax(0, dot(normal, light_dir));

    // Apply shading
    color = sphere->color;
    color.r = fmin(255, color.r * ambient.lighting * angle) > 255 ? 255 : color.r * ambient.lighting * angle;
    color.g = fmin(255, color.g * ambient.lighting * angle) > 255 ? 255 : color.g * ambient.lighting * angle;
    color.b = fmin(255, color.b * ambient.lighting * angle) > 255 ? 255 : color.b * ambient.lighting * angle;

    return ((int)color.r << 16 | (int)color.g << 8 | (int)color.b);
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