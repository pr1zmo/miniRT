#include "../minirt.h"

int	intersect(t_rt *rt, int x, int y)
{
	t_ray		ray;
	t_ray		temp_ray;
	t_hit_info	closest_hit;

	temp_ray.origin = rt->camera.position;
	temp_ray.orientation = rt->camera.orientation;
	ray = get_ray(rt, x, y, temp_ray);
	closest_hit = find_closest_object(rt, &ray);
	if (closest_hit.hit)
	{
		// return (rgb_to_int(closest_hit.color));
		return (compute_lighting(rt, &closest_hit));
	}
	return (0);
}

t_hit_info find_closest_object(t_rt *rt, t_ray *ray)
{
	t_object	*temp;
	t_hit_info	closest_hit;
	double		t;
	
	closest_hit.hit = 0;
	closest_hit.dist = INFINITY;

	temp = rt->object;
	while (temp)
	{
		if (temp->type == PLANE && plane_intersect(temp, ray, &t, &closest_hit))
		{
			if (t > 0 && t < closest_hit.dist)
				set_hit_info(&closest_hit, ray, temp, t);
		}
		else if (temp->type == SPHERE && sphere_intersect(temp, ray, &t))
		{
			if (t > 0 && t < closest_hit.dist)
				set_hit_info(&closest_hit, ray, temp, t);
		}
		else if (temp->type == CYLINDER && cylinder_intersect(temp, ray, &t))
		{
			if (t > 0 && t < closest_hit.dist)
				set_hit_info(&closest_hit, ray, temp, t);
		}
		temp = temp->next;
	}
	return (closest_hit);
}

t_vector	vec_mult(t_vector vec, double scalar)
{
	t_vector	result;

	result.x = vec.x * scalar;
	result.y = vec.y * scalar;
	result.z = vec.z * scalar;
	return (result);
}

int	cylinder_intersect(t_object *object, t_ray *ray, double *t_hit)
{
	t_cylinder *cyl = (t_cylinder *)object->object;
	t_vector hit_point;
	double a, b, c, discriminant;
	double t_body = INFINITY, t_cap = INFINITY, t_temp;
	double proj;

	t_vector oc = vec_sub(ray->origin, cyl->position);
	
	a = vec_dot(ray->direction, ray->direction) - pow(vec_dot(ray->direction, cyl->direction), 2);
	b = 2.0 * (vec_dot(ray->direction, oc) - vec_dot(ray->direction, cyl->direction) * vec_dot(oc, cyl->direction));
	c = vec_dot(oc, oc) - pow(vec_dot(oc, cyl->direction), 2) - pow(cyl->diameter / 2.0, 2);
	
	discriminant = b * b - 4 * a * c;
	if (discriminant >= 0)
	{
		t_temp = (-b - sqrt(discriminant)) / (2 * a);
		if (t_temp > 0)
		{
			hit_point = vec_add(ray->origin, vec_mult(ray->direction, t_temp));
			proj = vec_dot(vec_sub(hit_point, cyl->position), cyl->direction);
			if (proj >= 0 && proj <= cyl->height)
				t_body = t_temp;
		}
	}

	double denom = vec_dot(ray->direction, cyl->direction);
	if (fabs(denom) > 1e-6)
	{
		t_temp = vec_dot(vec_sub(cyl->position, ray->origin), cyl->direction) / denom;
		if (t_temp > 0)
		{
			hit_point = vec_add(ray->origin, vec_mult(ray->direction, t_temp));
			if (vec_length(vec_sub(hit_point, cyl->position)) <= cyl->diameter / 2.0)
				t_cap = t_temp;
		}

		t_vector top_center = vec_add(cyl->position, vec_mult(cyl->direction, cyl->height));
		t_temp = vec_dot(vec_sub(top_center, ray->origin), cyl->direction) / denom;
		if (t_temp > 0)
		{
			hit_point = vec_add(ray->origin, vec_mult(ray->direction, t_temp));
			if (vec_length(vec_sub(hit_point, top_center)) <= cyl->diameter / 2.0)
				t_cap = fmin(t_cap, t_temp);
		}
	}

	double t_final = fmin(t_body, t_cap);
	if (t_final < INFINITY && t_final > 0)
	{
		*t_hit = t_final;
		return 1;
	}
	return 0;
}

int sphere_intersect(t_object *object, t_ray *ray, double *t)
{
	t_sphere *sphere = (t_sphere *)object->object;
	t_vector oc;
	double a, b, c, discriminant;
	double radius = sphere->diameter / 2.0;

	oc = vec_sub(ray->origin, sphere->position);
	a = vec_dot(ray->direction, ray->direction);
	b = 2 * vec_dot(oc, ray->direction);
	c = vec_dot(oc, oc) - (radius * radius);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);
	*t = (-b - sqrt(discriminant)) / (2 * a);
	if (*t < 0)
		return (0);
	return (1);
}

int plane_intersect(t_object *object, t_ray *ray, double *t, t_hit_info *hit)
{
	(void)hit;
	t_plane *plane = (t_plane *)object->object;
	t_vector normal = vec_norm(plane->direction);
	double denom = vec_dot(ray->direction, normal);

	if (fabs(denom) < 1e-6)
		return 0;
	t_vector vec_1 = vec_sub(plane->position, ray->origin);
	*t = vec_dot(vec_1, normal) / denom;
	if (*t < 0)
		return 0;
	// Flip the normal if the ray hits the backside.
	// (For lighting, you want the normal facing the ray.)
	// if (denom > 0)
	// 	hit->normal = vec_scale(normal, -1);
	// else
	// 	hit->normal = normal;
	return 1;
}

/*
int compute_lighting(t_rt *rt, t_hit_info *closest_hit)
{
    t_color final_color = {0, 0, 0};
    t_object *obj = closest_hit->closest_object;
    t_color obj_color = obj->color;  // Assume these are in 0-255.
    double ambient_factor = rt->ambient.lighting; // e.g., 0.2 or 0.3

    // Start with an ambient term (added, not multiplied later)
    final_color.r = ambient_factor * obj_color.r;
    final_color.g = ambient_factor * obj_color.g;
    final_color.b = ambient_factor * obj_color.b;

    t_light *light = rt->light;
    while (light)
    {
        t_vector light_dir = vec_norm(vec_sub(light->position, closest_hit->hit_point));
        // Diffuse: brightness * max(0, dot(normal, light_dir))
        double diffuse = light->brightness * fmax(vec_dot(closest_hit->normal, light_dir), 0.0);
        
        // Light color normalized to [0,1] by dividing by 255:
        final_color.r += diffuse * ((double)light->color.r / 255.0) * obj_color.r;
        final_color.g += diffuse * ((double)light->color.g / 255.0) * obj_color.g;
        final_color.b += diffuse * ((double)light->color.b / 255.0) * obj_color.b;

        // You could add specular here similarly, without modulating by the object color.

        light = light->next;
    }

    // Clamp final values to [0, 255]
    final_color.r = fmin(255, final_color.r);
    final_color.g = fmin(255, final_color.g);
    final_color.b = fmin(255, final_color.b);

    return rgb_to_int(final_color);
}

*/