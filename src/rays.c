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

/*
1. **Field of View Adjustment**: The `fov_adjustment` is calculated using the tangent of half the field of view angle (converted to radians). This helps in adjusting the ray direction based on the camera's field of view.
2. **Normalized Device Coordinates (NDC)**: The `ndc_x` and `ndc_y` are calculated to convert the pixel coordinates `(x, y)` to a normalized range, considering the aspect ratio and field of view.
3. **Forward Vector**: The `forward` vector is the normalized direction from the camera's position to its orientation.
4. **Right Vector**: The `right` vector is perpendicular to the `forward` vector and the world's up vector `(0, 1, 0)`.
5. **Up Vector**: The `up` vector is perpendicular to both the `right` and `forward` vectors.
6. **Ray Direction**: The `ray_dir` is calculated by combining the `forward`, `right`, and `up` vectors, scaled by `ndc_x` and `ndc_y`.
7. **Return Ray**: Finally, a `t_ray` is returned with the camera's position as the origin and the calculated `ray_dir` as the direction.
*/

t_ray get_ray(t_rt *rt, int x, int y)
{
    // Calculate the field of view adjustment factor
    double fov_adjustment = tan(rt->camera.fov * M_PI / 180 / 2);
    
    // Convert screen coordinates (x, y) to normalized device coordinates (NDC)
    double ndc_x = (2 * ((x + 0.5) / rt->width) - 1) * ASPECT_RATIO * fov_adjustment;
    double ndc_y = (1 - 2 * ((y + 0.5) / rt->height)) * fov_adjustment;
    
    // Calculate the forward direction vector of the camera
    t_vector forward = vector_normalize(vector_subtract(rt->camera.orientation, rt->camera.position));
    
    // Calculate the right direction vector of the camera
    t_vector right = vector_normalize(vector_cross(forward, (t_vector){0, 1, 0}));
    
    // Calculate the up direction vector of the camera
    t_vector up = vector_cross(right, forward);
    
    // Calculate the direction of the ray
    t_vector ray_dir = vector_normalize(vector_add(forward, vector_add(scale_vector(ndc_x, right), scale_vector(ndc_y, up))));
    
    // Return the ray with the camera's position as the origin and the calculated direction
    return (t_ray){rt->camera.position, ray_dir};
}