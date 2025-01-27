#include "../minirt.h"

int	destroy(t_rt *rt)
{
	if (rt == NULL)
	{
		printf("Error: rt is NULL\n");
		return (1);
	}
	if (rt->img.img != NULL)
	{
		mlx_destroy_image(rt->mlx, rt->img.img);
		rt->img.img = NULL;
	}
	if (rt->win != NULL)
	{
		mlx_destroy_window(rt->mlx, rt->win);
		rt->win = NULL;
	}
	if (rt->mlx != NULL)
		mlx_destroy_display(rt->mlx);
	if (rt->mlx)
		free(rt->mlx);
	if (rt->object)
		free_objects(rt->object);
	if (rt->file_fd > 0)
		close(rt->file_fd);
	free(rt);
	exit(0);
}

unsigned int random_int(int state)
{
	(void)state;
	static unsigned int lcg_seed = 0xACE1u;
	lcg_seed = (1103515247 * lcg_seed + 12345) % (1 << 31);
	return (lcg_seed % 0xFFFFFF);
}

void	my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char *pxl;

	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		pxl = data->addr + (y * data->line_length
				+ x * (data->bits_per_pixel / 8));
		*(unsigned int *)pxl = color;
	}
}

int	draw_sphere(t_object *object, int x, int y)
{
	t_sphere	*sphere;
	double		r;
	double		x1;
	double		y1;

	sphere = (t_sphere *)object->object;
	r = sphere->diameter / 2;
	x1 = sphere->position.x;
	y1 = sphere->position.y;
	if ((x - x1) * (x - x1) + (y - y1) * (y - y1) <= r * r)
		return (1);
	return (0);
}

int	draw_plane(t_object *object, int x, int y)
{
	t_plane	*plane;
	double		x1;
	double		y1;
	double		z1;
	double		d;

	plane = (t_plane *)object->object;
	x1 = plane->position.x;
	y1 = plane->position.y;
	z1 = plane->position.z;
	d = plane->direction.x * x1 + plane->direction.y * y1 + plane->direction.z * z1;
	if (plane->direction.x * x + plane->direction.y * y + plane->direction.z * z1 == d)
		return (1);
	return (0);
}

int	draw_cylinder(t_object *object, int x, int y)
{
	t_cylinder	*cylinder;
	double		x1;
	double		y1;
	double		z1;
	double		d;

	cylinder = (t_cylinder *)object->object;
	x1 = cylinder->position.x;
	y1 = cylinder->position.y;
	z1 = cylinder->position.z;
	d = cylinder->direction.x * x1 + cylinder->direction.y * y1 + cylinder->direction.z * z1;
	if (cylinder->direction.x * x + cylinder->direction.y * y + cylinder->direction.z * z1 == d)
		return (1);
	return (0);
}

int	check_collision(t_object *object, int x, int y)
{
	if (object != NULL)
	{
		if (object->type == SPHERE)
			draw_sphere(object, x, y);
		else if (object->type == PLANE)
			draw_plane(object, x, y);
		else if (object->type == CYLINDER)
			draw_cylinder(object, x, y);
	}
	return (0);
}

t_vector	vector_subtract(t_vector a, t_vector b)
{
	return ((t_vector){a.x - b.x, a.y - b.y, a.z - b.z});
}

t_vector	vector_add(t_vector a, t_vector b)
{
	return ((t_vector){a.x + b.x, a.y + b.y, a.z + b.z});
}

t_vector	vector_scale(t_vector v, double scale)
{
	return ((t_vector){v.x * scale, v.y * scale, v.z * scale});
}

double	vector_dot(t_vector a, t_vector b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vector	vector_normalize(t_vector v)
{
	double	length;

	length = sqrt(vector_dot(v, v));
	return ((t_vector){v.x / length, v.y / length, v.z / length});
}

t_color calculate_lighting(t_vector point, t_vector normal, t_vector view, t_rt *rt, t_color object_color)
{
	t_light light = rt->light;
	t_ambient ambient = rt->ambient;

	t_color	result = {0, 0, 0};
	t_vector	light_dir = vector_normalize(vector_subtract(light.position, point));
	t_vector	reflect_dir = vector_subtract(vector_scale(normal, 2 * vector_dot(normal, light_dir)), light_dir);

	// Ambient
	double	ambient_strength = ambient.lighting;
	result.r += object_color.r * ambient_strength;
	result.g += object_color.g * ambient_strength;
	result.b += object_color.b * ambient_strength;

	// Diffuse
	double	diffuse_strength = fmax(vector_dot(normal, light_dir), 0.0);
	result.r += object_color.r * light.color.r * light.brightness * diffuse_strength / 255;
	result.g += object_color.g * light.color.g * light.brightness * diffuse_strength / 255;
	result.b += object_color.b * light.color.b * light.brightness * diffuse_strength / 255;

	// Specular
	double	specular_strength = 0.5;
	double	specular_factor = pow(fmax(vector_dot(view, reflect_dir), 0.0), 32);
	result.r += light.color.r * light.brightness * specular_strength * specular_factor;
	result.g += light.color.g * light.brightness * specular_strength * specular_factor;
	result.b += light.color.b * light.brightness * specular_strength * specular_factor;

	// Ensure the values are within the valid range [0, 255]
	if (result.r > 255) result.r = 255;
	if (result.g > 255) result.g = 255;
	if (result.b > 255) result.b = 255;

	return result;
}

t_color	set_color(int x, int y, t_object *object, t_rt *rt)
{
	t_color	color;
	t_sphere	*sphere;
	(void)x;
	(void)y;

	sphere = (t_sphere *)object->object;
	// printf("Object color: (%.2f, %.2f, %.2f)\n", sphere->color.r, sphere->color.g, sphere->color.b);
	color = calculate_lighting(sphere->position, (t_vector){0, 0, 1}, (t_vector){0, 0, 1}, rt, sphere->color);
	return (color);
}

int create_trgb(t_color color)
{
	return (0 << 24 | (int)color.r << 16 | (int)color.g << 8 | (int)color.b);
}

void	check_hit(t_rt *rt, int x, int y)
{
	t_object	*object;
	// t_sphere	*sphere;
	// int			angle;
	int		color;

	object = rt->object;
	// sphere = (t_sphere *)object->object;
	for (int i = 0; i < 10; i++)
	{
		for (int k = 0; k < 10; k++)
			my_mlx_pixel_put(&rt->img, x + i, y + k, 0x00FF0000);
	}
	while (object)
	{
		if (check_collision(object, x, y))
		{
			color = create_trgb(set_color(x, y, object, rt));
			printf("Setting pixel at (%d, %d) with color: %d\n", x, y, color);
			my_mlx_pixel_put(&rt->img, x, y, color);
			return;
		}
		object = object->next;
	}
}

void	init_rays(t_rt *rt, int width, int height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			check_hit(rt, i, j);
		}
	}
}

void	debug()
{
	printf("Into the debug function\n");
}

void ft_add_back(t_object **list, t_object *new, int type)
{
	t_object *temp;

	if (new == NULL)
		return;
	new->type = type;
	if (*list == NULL)
	{
		*list = new;
		new->next = NULL;
	}
	else
	{
		temp = *list;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
		new->next = NULL;
	}
}

void	show_sphere(t_sphere *sphere)
{
	printf("Sphere: %p\n", sphere);
	printf("Into the sphere\n");
	printf("Sphere: position=(%.2f, %.2f, %.2f)\n\tdiameter=%.2f\n\tcolor=(%.2f, %.2f, %.2f)\n",
		sphere->position.x, sphere->position.y, sphere->position.z,
		sphere->diameter, sphere->color.r, sphere->color.g, sphere->color.b);
}

void	show_plane(t_object *object)
{
	t_plane	*plane;

	plane = (t_plane *)object->object;
	printf("Plane: %p\n", object);
	printf("Plane: position=(%.2f, %.2f, %.2f)\n\tdirection=(%.2f, %.2f, %.2f)\n\tcolor=(%.2f, %.2f, %.2f)\n",
		plane->position.x, plane->position.y, plane->position.z,
		plane->direction.x, plane->direction.y, plane->direction.z,
		plane->color.r, plane->color.g, plane->color.b);
}

void	show_cylinder(t_object *object)
{
	t_cylinder	*clynder;

	clynder = (t_cylinder *)object->object;
	printf("Cylinder: %p\n", object);
	printf("Cylinder: position=(%.2f, %.2f, %.2f)\n\tdirection=(%.2f, %.2f, %.2f)\n\tdiameter=%.2f\n\theight=%.2f\n\tcolor=(%.2f, %.2f, %.2f)\n",
		clynder->position.x, clynder->position.y, clynder->position.z,
		clynder->direction.x, clynder->direction.y, clynder->direction.z,
		clynder->diameter, clynder->height,
		clynder->color.r, clynder->color.g, clynder->color.b);
}

void	list_objects(t_rt *rt)
{
	t_object	*object;
	t_camera	camera;

	camera = rt->camera;
	printf("camera fov: %f\n\tcamera'a orientation: {%f, %f, %f}\n\tcamera's position: {%f, %f, %f}\n\t", camera.fov, camera.orientation.x, camera.orientation.y, camera.orientation.x, camera.position.x, camera.position.y, camera.position.z);
	object = rt->object;
	if (!object)
	{
		printf("No objects\n");
		return;
	}
	int i = 0;
	while (i < rt->object_count)
	{
		printf("The type from the list_object function: %d\n", object->type);
		if (object->type == SPHERE)
			show_sphere(object->object);
		else if (object->type == PLANE)
			show_plane(object);
		else if (object->type == CYLINDER)
			show_cylinder(object);
		else
			printf("Unknown object\n");
		object = object->next;
		i++;
	}
}

void	render(t_rt *rt, int width, int height)
{
	mlx_clear_window(rt->mlx, rt->win);
	list_objects(rt);
	init_rays(rt, width, height);
	printf("Rendering...\n");
	mlx_put_image_to_window(rt->mlx, rt->win, rt->img.img, 0, 0);
}

int	key_hook(int keycode, t_rt *rt)
{
	printf("keycode: %d\n", keycode);
	if (keycode == 65307)
		destroy(rt);
	else if (keycode == LEFT_ARROW)
		rt->camera.position.x -= 10;
	else if (keycode == RIGHT_ARROW)
		rt->camera.position.x += 10;
	else if (keycode == UP_ARROW)
		rt->camera.position.y -= 10;
	else if (keycode == DOWN_ARROW)
		rt->camera.position.y += 10;
	render(rt, WIDTH, HEIGHT);
	return (0);
}

int	is_hit(t_rt *rt, int x, int y)
{
	(void)rt;
	int	r;
	int	x1;
	int	y1;

	r = 100;
	x1 = WIDTH / 2;
	y1= HEIGHT / 2;

	if ((x - x1) * (x - x1) + (y - y1) * (y - y1) <= r * r)
		return (1);
	return (0);
}

int main(int ac, char **av)
{
	t_rt	*rt;

	if (ac != 2)
	{
		ft_putstr_fd("Error: invalid number of arguments\n", 2);
		exit(EXIT_FAILURE);
	}
	rt = (t_rt *)malloc(sizeof(t_rt));
	if (rt == NULL)
	{
		perror("Failed to allocate memory for t_rt");
		exit(EXIT_FAILURE);
	}
	rt->mlx = mlx_init();
	rt->win = mlx_new_window(rt->mlx, WIDTH, HEIGHT, "miniRT");
	rt->img.img = mlx_new_image(rt->mlx, WIDTH, HEIGHT);
	rt->img.addr = mlx_get_data_addr(rt->img.img, &rt->img.bits_per_pixel,
			&rt->img.line_length, &rt->img.endian);
	rt->object_count = 0;
	open_file(rt, av[1]);
	render(rt, WIDTH, HEIGHT);
	mlx_key_hook(rt->win, key_hook, rt);
	mlx_hook(rt->win, 17, 0, destroy, rt);
	mlx_loop(rt->mlx);
	return (0);
}
