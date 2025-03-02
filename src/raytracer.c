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

int	plane_intersect(t_object *object, t_ray *ray, int *t)
{
	t_plane	*plane;
	double	denom;

	plane = (t_plane *)object->object;
	denom = vec_dot(vec_norm(plane->direction), ray->direction);
	if (fabs(denom) > 0.0001)
	{
		*t = vec_dot(vec_sub(plane->position, ray->origin), vec_norm(plane->direction)) / denom;
		if (*t >= 0)
			return (1);
	}
	return (0);
}

// int	sphere_intersect(t_object *object, t_ray *ray, int *t)
// {
// 	t_sphere	*sphere = (t_sphere *)object->object;
// 	t_vector	oc;
// 	double		a;
// 	double		b;
// 	double		c;
// 	double		discriminant;

// 	oc = vec_sub(ray->origin, sphere->position);
// 	a = vec_dot(ray->direction, ray->direction);
// 	b = 2.0 * vec_dot(oc, ray->direction);
// 	c = vec_dot(oc, oc) - (sphere->diameter * sphere->diameter);
// 	discriminant = b * b - 4 * a * c;
// 	if (discriminant < 0)
// 		return (0);
// 	*t = (-b - sqrt(discriminant)) / (2 * a);
// 	if (*t < 0)
// 		return (0);
// 	return (1);
// }

int sphere_intersect(t_object *object, t_ray *ray, int *t)
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

t_ray get_ray(t_rt *rt, int x, int y)
{
	t_ray		ray;
	t_vector	right, up, forward, screen_pixel;
	double		px_x, px_y, fov_adjustment;

	ray.origin = rt->camera.position;
	forward = vec_norm(rt->camera.orientation);

	right = vec_norm(vec_cro(forward, (t_vector){0, 1, 0}));
	up = vec_cro(right, forward);

	fov_adjustment = tan((rt->camera.fov * M_PI / 180.0) / 2.0);

	px_x = (2 * ((x + 0.5) / rt->width) - 1) * ASPECT_RATIO * fov_adjustment;
	px_y = (1 - 2 * ((y + 0.5) / rt->height)) * fov_adjustment;

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

/*
#version 300 es

precision mediump float;

 RGB material definition.
struct RGBMaterial {
    
    /// Emissive component.
    vec4 ke;
    /// Ambient component.
    vec4 ka;
    /// Diffuse component.
    vec4 kd;
    /// Specular component.
    vec4 ks;
    /// Shiness.
    float sh;
};

 RGB light definition.
 It composed of a direction and a color.

struct DirectionalRGBLight {
    
    /// Light direction.
    vec3 direction;
    /// Light rgb color.
    vec4 color;
};

/// Normal (from vertex shader) interpolated per fragment.
in vec3 normalInterp;
/// Vertex position.
in vec3 vertPos;

/// Final fragment color.
out vec4 fragmentColor;

/// View position.
uniform vec3 viewPosition;
/// Light data.
uniform DirectionalRGBLight light;
/// Material data.
uniform RGBMaterial surfaceMaterial;

void main() {
    
    //Calculate light direction and view direction.
    vec3 lightDirection = normalize(light.direction);
    vec3 viewDirection = normalize(viewPosition - vertPos);
    
    //Cosine theta diffuse lambertian component.
    float cosTheta = max(0.0, dot(normalInterp, normalize(lightDirection)));
    
    vec4 emissive = surfaceMaterial.ke * light.color;
    vec4 ambient = surfaceMaterial.ka * light.color;
    vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
    
    //Only if light is visible from the surface point.
    if(cosTheta > 0.0) {
        
        //Reflection vector around normal.
        vec3 reflectionDirection = reflect(-lightDirection, normalInterp);
        
        //Diffuse component.
        diffuse = surfaceMaterial.kd * light.color * cosTheta;
        
        //Specular component.
        specular = surfaceMaterial.ks * light.color * pow(max(0.0, dot(reflectionDirection, viewDirection)), surfaceMaterial.sh);
    }
    
    fragmentColor = emissive + ambient + diffuse + specular;
}
*/

t_color	mix_color(t_color c1, float p1, t_color c2, float p2)
{
	t_color	dst;

	dst.r = c1.r * p1 + c2.r * p2;
	dst.g = c1.g * p1 + c2.g * p2;
	dst.b = c1.b * p1 + c2.b * p2;
	return (dst);
}

/*
import numpy as np

def phong_reflection_point_light(point, normal, light_pos, view_pos, 
                                 ambient_color, diffuse_color, specular_color, shininess):
    """
    Compute the color at a point using the Phong reflection model with a point light source.

    Parameters:
    - point: The 3D point on the surface (numpy array)
    - normal: Normal vector at the point (numpy array, normalized)
    - light_pos: Position of the light source (numpy array)
    - view_pos: Position of the camera/viewer (numpy array)
    - ambient_color: Ambient light color (numpy array)
    - diffuse_color: Diffuse light color (numpy array)
    - specular_color: Specular light color (numpy array)
    - shininess: Shininess coefficient for specular reflection

    Returns:
    - Computed color (numpy array)
    """

    # Normalize normal
    normal = normal / np.linalg.norm(normal)

    # Compute light direction (from point to light)
    light_dir = light_pos - point
    light_dir = light_dir / np.linalg.norm(light_dir)

    # Compute view direction (from point to camera)
    view_dir = view_pos - point
    view_dir = view_dir / np.linalg.norm(view_dir)

    # Ambient component
    ambient = ambient_color

    # Diffuse component (Lambertian reflection)
    diff = max(np.dot(normal, light_dir), 0.0)
    diffuse = diffuse_color * diff

    # Specular component (Phong reflection)
    reflect_dir = 2 * np.dot(normal, light_dir) * normal - light_dir
    spec = max(np.dot(view_dir, reflect_dir), 0.0) ** shininess
    specular = specular_color * spec

    # Final color
    return ambient + diffuse + specular

# Example usage
point = np.array([0, 0, 0])    # Surface point at origin
normal = np.array([0, 0, 1])   # Normal facing up
light_pos = np.array([1, 1, 1]) # Light source at (1,1,1)
view_pos = np.array([0, 0, 1])  # Camera at (0,0,1)

ambient_color = np.array([0.1, 0.1, 0.1])  # Low ambient light
diffuse_color = np.array([0.8, 0.2, 0.2])  # Reddish diffuse light
specular_color = np.array([1.0, 1.0, 1.0]) # White specular highlight
shininess = 32  # Higher values = sharper highlights

color = phong_reflection_point_light(point, normal, light_pos, view_pos, 
                                     ambient_color, diffuse_color, specular_color, shininess)
print("Resulting Color:", color)
*/

int	rgb_to_int(t_color color)
{
	return ((int)(color.r) << 16 | (int)(color.g) << 8 | (int)(color.b));
}

t_color phong_model(t_hit_info hit, t_light light, t_camera camera)
{
	t_vector light_dir, view_dir, reflect_dir;
	t_color ambient, diffuse, specular, final_color;
	double diff_intensity, spec_intensity;
	double shininess = 32;

	// Normalize the normal
	hit.normal = vec_norm(hit.normal);

	// Compute light direction (from hit point to light)
	light_dir = vec_sub(light.position, hit.hit_point);
	light_dir = vec_norm(light_dir);

	// Compute view direction (from hit point to camera)
	view_dir = vec_sub(camera.position, hit.hit_point);
	view_dir = vec_norm(view_dir);

	// Ambient component (global illumination)
	ambient.r = hit.color.r * light.color.r * 0.1;
	ambient.g = hit.color.g * light.color.g * 0.1;
	ambient.b = hit.color.b * light.color.b * 0.1;

	// Diffuse component (Lambertian reflection)
	diff_intensity = fmax(vec_dot(hit.normal, light_dir), 0.0);
	diffuse.r = hit.color.r * light.color.r * diff_intensity;
	diffuse.g = hit.color.g * light.color.g * diff_intensity;
	diffuse.b = hit.color.b * light.color.b * diff_intensity;

	// Specular component (Phong reflection)
	reflect_dir = vec_sub(vec_scale(hit.normal, 2 * vec_dot(hit.normal, light_dir)), light_dir);
	reflect_dir = vec_norm(reflect_dir);
	spec_intensity = pow(fmax(vec_dot(view_dir, reflect_dir), 0.0), shininess);

	specular.r = light.color.r * spec_intensity;
	specular.g = light.color.g * spec_intensity;
	specular.b = light.color.b * spec_intensity;

	// Final color = Ambient + Diffuse + Specular
	final_color.r = ambient.r + diffuse.r + specular.r;
	final_color.g = ambient.g + diffuse.g + specular.g;
	final_color.b = ambient.b + diffuse.b + specular.b;

	// Clamp color values to [0,1] range
	final_color.r = fmin(final_color.r, 1.0);
	final_color.g = fmin(final_color.g, 1.0);
	final_color.b = fmin(final_color.b, 1.0);

	return final_color;
}

// int	compute_lighting(t_rt *rt, t_vector point, t_vector normal, t_vector view_dir, t_object *obj)
// {
// 	(void)rt;
// 	(void)point;
// 	(void)normal;
// 	(void)view_dir;
// 	t_color	phong;
// 	double	ambient;
// 	double	diffuse;
// 	double	reflect;
// 	// double	specular;

// 	ambient = rt->ambient.lighting;
// 	diffuse = fmax(0.0, vec_dot(normal, vec_norm(vec_sub(rt->light.position, point))));
// 	reflect = fmax(0.0, vec_dot(view_dir, reflect_light(normal, vec_norm(vec_sub(rt->light.position, point)))));
// 	double coef = ambient + diffuse + reflect;
// 	phong.r = fmin(255, obj->color.r * coef);
// 	phong.g = fmin(255, obj->color.g * coef);
// 	phong.b = fmin(255, obj->color.b * coef);
// 	return (rgb_to_int(phong));
// }

double	vec_length(t_vector v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

int compute_lighting(t_rt *rt, t_vector point, t_vector normal, t_vector view_dir, t_object *obj)
{
	t_color phong;
	t_vector light_dir;
	t_light	*light;
	double ambient, diffuse, specular;
	double n_dot_l, r_dot_v;

	light = &rt->light;	
	while (light)
	{
		light_dir = vec_norm(vec_sub(light->position, point));
		n_dot_l = vec_dot(normal, light_dir);
		r_dot_v = vec_dot(reflect_light(normal, light_dir), view_dir);
		ambient = light->brightness;
		diffuse = fmax(n_dot_l, 0.0);
		specular = pow(fmax(r_dot_v, 0.0), 32);
		phong = mix_color(obj->color, ambient, obj->color, diffuse);
		phong = mix_color(phong, 1.0, (t_color){1, 1, 1}, specular);
		light = light->next;
	}
	return (rgb_to_int(phong));
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

t_hit_info find_closest_object(t_rt *rt, t_ray *ray)
{
	t_object	*temp;
	t_hit_info	closest_hit;
	int			t;
	
	closest_hit.hit = 0;
	closest_hit.dist = INFINITY;

	temp = rt->object;
	while (temp)
	{
		if (temp->type == PLANE && plane_intersect(temp, ray, &t))
		{
			if (t > 0 && t < closest_hit.dist)
				set_hit_info(&closest_hit, ray, temp, t);
		}
		if (temp->type == SPHERE && sphere_intersect(temp, ray, &t))
		{
			if (t > 0 && t < closest_hit.dist)
				set_hit_info(&closest_hit, ray, temp, t);
		}
		// if (closest_hit.hit)
		// 	printf("CLoses hit object %d\n", closest_hit.closest_object->type);
		temp = temp->next;
	}
	return (closest_hit);
}

int	intersect(t_rt *rt, int x, int y)
{
	t_ray		ray;
	t_hit_info	closest_hit;

	ray = get_ray(rt, x, y);
	closest_hit = find_closest_object(rt, &ray);
	if (closest_hit.hit)
		return (compute_lighting(rt, closest_hit.hit_point, closest_hit.normal, vec_norm(vec_sub(rt->camera.position, closest_hit.hit_point)), closest_hit.closest_object));
	return (0);
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
	int	result;

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
