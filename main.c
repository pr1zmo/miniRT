#include "minirt.h"

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
	free_objects(rt->object);
	if (rt->file_fd > 0)
		close(rt->file_fd);
	free(rt);
	exit(0);
}

void	free_objects(t_object *object)
{
	t_object	*temp;

	while (object)
	{
		temp = object;
		object = object->next;
		if (temp->type == SPHERE)
			free(temp->object);
		free(temp);
	}
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

int	check_collision(t_object *object, int x, int y)
{
	if (object != NULL && object->type == SPHERE)
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

t_color calculate_lighting(t_vector point, t_vector normal, t_vector view, t_light light, t_color object_color)
{
	t_color	result = {0, 0, 0};
	t_vector	light_dir = vector_normalize(vector_subtract(light.position, point));
	t_vector	reflect_dir = vector_subtract(vector_scale(normal, 2 * vector_dot(normal, light_dir)), light_dir);

	// Ambient
	double	ambient_strength = 0.1;
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

	color = calculate_lighting((t_vector){x, y, 0}, (t_vector){0, 0, 1}, rt->camera.direction, rt->light, ((t_sphere *)object->object)->color);
	return (color);
}

int create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

void	check_hit(t_rt *rt, int x, int y)
{
	t_object	*object;
	// t_sphere	*sphere;
	// int			angle;
	t_color		color;

	object = rt->object;
	// sphere = (t_sphere *)object->object;
	while (object)
	{
		if (check_collision(object, x, y))
		{
			color = set_color(x, y, object, rt);
			my_mlx_pixel_put(&rt->img, x, y, create_trgb(0, (int)color.r, (int)color.g, (int)color.b));
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

void	ft_add_back(t_object **list, t_object *new)
{
	t_object *temp;

	if (new == NULL)
		return;
	if (*list == NULL)
	{
		*list = new;
		new->next = NULL;
		return;
	}
	temp = *list;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
	new->next = NULL;
}

t_object	*init_sphere(t_rt *rt)
{
	t_sphere	*sphere;
	t_object	*object;

	(void)rt;
	sphere = (t_sphere *)malloc(sizeof(t_sphere));
	if (sphere == NULL)
	{
		ft_putstr_fd("Error: malloc() failed for sphere\n", 2);
		return (NULL);
	}
	sphere->color.r = 255;
	sphere->color.g = 0;
	sphere->color.b = 0;
	sphere->diameter = 200;
	sphere->position.x = 500;
	sphere->position.y = 200;
	sphere->position.z = 0;
	object = (t_object *)malloc(sizeof(t_object));
	if (object == NULL)
	{
		ft_putstr_fd("Error: malloc() failed for object\n", 2);
		free(sphere);
		return (NULL);
	}
	object->type = SPHERE;
	object->object = sphere;
	object->next = NULL;
	return (object);
}

void init_objects(t_rt *rt, int width, int height)
{
	t_object	*object;
	int			i;

	(void)width;
	(void)height;
	i = 0;
	object = NULL;
	while (i < rt->object_count) {
		// if (rt->object->type == SPHERE)
		object = init_sphere(rt);
		ft_add_back(&rt->object, object);
		i++;
	}
}

void	show_sphere(t_object *object)
{
	t_sphere *sphere;

	sphere = (t_sphere *)object->object;
	printf("Sphere: position=(%.2f, %.2f, %.2f), diameter=%.2f, color=(%.2f, %.2f, %.2f)\n",
		sphere->position.x, sphere->position.y, sphere->position.z,
		sphere->diameter, sphere->color.r, sphere->color.g, sphere->color.b);
}

void	show_plane(t_object *object)
{
	(void)object;
}

void	show_cylinder(t_object *object)
{
	(void)object;
}

void	list_objects(t_rt *rt)
{
	t_object	*object;

	object = rt->object;
	while (object)
	{
		if (object->type == SPHERE)
			show_sphere(object);
		else if (object->type == PLANE)
			show_plane(object);
		else if (object->type == CYLINDER)
			show_cylinder(object);
		else
			printf("Unknown object\n");
		object = object->next;
	}
}

void	init(t_rt *rt, int width, int height)
{
	rt->object = NULL;
	init_scene(rt);
	init_objects(rt, width, height);
}

void	render(t_rt *rt, int width, int height)
{
	mlx_clear_window(rt->mlx, rt->win);
	list_objects(rt);
	init_rays(rt, width, height);
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

void	init_light(t_rt *rt)
{
	t_vector	position;
	t_color		color;
	double		brightness;

	position.x = 500;
	position.y = 200;
	for (int i = 0; i < HEIGHT; i++)
		my_mlx_pixel_put(&rt->img, (int)position.x, i, 0xFFFFFF);
	for (int k = 0; k < WIDTH; k++)
		my_mlx_pixel_put(&rt->img, k, (int)position.y, 0xFFFFFF);
	position.z = 300;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	brightness = 1;
	rt->light.position = position;
	rt->light.color = color;
	rt->light.brightness = brightness;
}

void	init_scene(t_rt *rt)
{
	t_vector	position;
	t_vector	direction;
	t_camera	camera;

	position.x = 0;
	position.y = 0;
	position.z = 0;
	direction.x = 0;
	direction.y = 0;
	direction.z = 1;
	camera.position = position;
	camera.direction = direction;
	camera.fov = 60;
	init_light(rt);
	rt->camera = camera;
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
	open_file(rt, av[1]);
	rt->mlx = mlx_init();
	rt->win = mlx_new_window(rt->mlx, WIDTH, HEIGHT, "miniRT");
	rt->img.img = mlx_new_image(rt->mlx, WIDTH, HEIGHT);
	rt->img.addr = mlx_get_data_addr(rt->img.img, &rt->img.bits_per_pixel,
			&rt->img.line_length, &rt->img.endian);
	rt->object_count = 1;
	init(rt, WIDTH, HEIGHT);
	render(rt, WIDTH, HEIGHT);
	mlx_key_hook(rt->win, key_hook, rt);
	mlx_hook(rt->win, 17, 0, destroy, rt);
	mlx_loop(rt->mlx);
	return (0);
}
