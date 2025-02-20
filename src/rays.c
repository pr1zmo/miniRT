#include "../minirt.h"

t_vector cross_product(t_vector a, t_vector b)
{
    t_vector result;
    
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    
    return result;
}

double	deg_2_rad(double degrees)
{
	return (degrees * M_PI / 180);
}

void	generate_ray(t_rt *rt, int x, int y)
{
	t_vector forward = vector_normalize(vector_subtract(rt->camera.orientation, rt->camera.position));
	t_vector right = vector_normalize(cross_product(forward, rt->camera.orientation));
	t_vector up_corrected = cross_product(right, forward);

	// Compute viewport dimensions in world units
	double fov_rad = rt->camera.fov * deg_2_rad(1.0);
	double viewport_height = 2.0 * tan(fov_rad / 2.0);
	double viewport_width = viewport_height * ((double)rt->width / rt->height);

	// Map pixel (x,y) to normalized viewport coordinates (u,v) in [0,1]
	double u = ((double)x + 0.5) / rt->width;
	double v = ((double)y + 0.5) / rt->height;

	// Convert (u,v) to viewport space coordinates where the center is (0,0)
	double viewport_x = (u - 0.5) * viewport_width;
	double viewport_y = (0.5 - v) * viewport_height; // Flip v to match screen coordinates

	// Compute the world-space position on the viewport
	// The center of the viewport is at: camera.position + forward
	t_vector pixel_world = vector_add(
								vector_add(rt->camera.position, forward),
								vector_add(vector_scale(right, viewport_x), vector_scale(up_corrected, viewport_y))
							);

	rt->ray->origin = rt->camera.position;
	rt->ray->direction = vector_normalize(vector_subtract(pixel_world, rt->camera.position));
}