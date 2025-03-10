#include "../minirt.h"

int	global_fd;

unsigned int random_int(void)
{
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

void	show_light(t_rt *rt)
{
	t_light *temp = rt->light;
	int		i = 0;
	while (temp)
	{
		printf("light: %f %f | Color : {%f, %f, %f}\n", temp->position.x, temp->position.y, temp->color.r, temp->color.g, temp->color.b);
		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{
				if (i == temp->position.x || j == temp->position.y)
					my_mlx_pixel_put(&rt->img, j, i, 0xffffff);
			}
		}
		i++;
		temp = temp->next;
	}
	printf("Total lights: %d\n", i);
}

void	*init_rays(void	*thread_data)
{
	// char	*progress;
	t_rt	*rt = (t_rt *)thread_data;

	rt->rendering = 1;
	// rt->ray = (t_ray *)malloc(sizeof(t_ray));
	// if (rt->ray == NULL)
	// {
	// 	perror("Failed to allocate memory for ray");
	// 	exit(EXIT_FAILURE);
	// }
	for (int i = 0; i < rt->height; i++)
	{
		// char *temp = ft_strdup("progress: ");
		// progress = ft_strjoin(temp, ft_itoa(i));
		// mlx_string_put(rt->mlx, rt->win, WIDTH / 2, HEIGHT / 2, 0xffffff, progress);
		// free(progress);
		// free(temp);
		for (int j = 0; j < rt->width; j++)
		{
			r_trace(rt, j, i);
		}
	}
	// show_light(rt);
	rt->rendering = 0;
	return (NULL);
}

void ft_add_back(t_object **list, t_object *new, int type)
{
	t_object	*temp;

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

void	*monitoring(void	*data)
{
	t_rt *rt;

	rt = (t_rt *)data;
	if (rt->rendering)
		printf("Rending...\n");
	return (NULL);
}

// void	render(t_rt *rt)
// {
// 	pthread_t	thread[20];
// 	pthread_t	monitor;

// 	mlx_clear_window(rt->mlx, rt->win);
// 	pthread_create(&monitor, NULL, monitoring, rt);
// 	for (int i = 0; i < 20; i++) {
// 		pthread_create(&thread[i], NULL, init_rays, rt);
// 	}
// 	for (int k = 0; k < 20; k++) {
// 		pthread_join(thread[k], NULL);
// 	}
// 	pthread_join(monitor, NULL);
// 	mlx_put_image_to_window(rt->mlx, rt->win, rt->img.img, 0, 0);
// 	rt->rendering = 0;
// }

void	show_objects(t_rt *rt)
{
	t_object *temp = rt->object;

	while (temp)
	{
		if (temp->type == SPHERE)
		{
			t_sphere *sphere = (t_sphere *)temp->object;
			printf("Sphere: %f %f %f | Color : {%f, %f, %f}\n", sphere->position.x, sphere->position.y, sphere->position.z, sphere->color.r, sphere->color.g, sphere->color.b);
		}
		else if (temp->type == PLANE)
		{
			t_plane *plane = (t_plane *)temp->object;
			printf("Plane: %f %f %f | Color : {%f, %f, %f}\n", plane->position.x, plane->position.y, plane->position.z, plane->color.r, plane->color.g, plane->color.b);
		}
		else if (temp->type == CYLINDER)
		{
			t_cylinder *cylinder = (t_cylinder *)temp->object;
			printf("Cylinder: %f %f %f | Color : {%f, %f, %f}\n", cylinder->position.x, cylinder->position.y, cylinder->position.z, cylinder->color.r, cylinder->color.g, cylinder->color.b);
		}
		temp = temp->next;
	}
}

void	render(t_rt *rt)
{
	mlx_clear_window(rt->mlx, rt->win);
	// monitoring(rt);
	// show_objects(rt);
	init_rays(rt);
	mlx_put_image_to_window(rt->mlx, rt->win, rt->img.img, 0, 0);
	rt->rendering = 0;
}

void move_sphere(t_rt *rt, int keycode)
{
	t_object *temp;

	temp = rt->object;
	while (temp)
	{
		if (temp->type == SPHERE)
		{
			t_sphere *sphere = (t_sphere *)temp->object;
			if (keycode == LEFT_ARROW)
				sphere->position.x -= 10;
			else if (keycode == RIGHT_ARROW)
				sphere->position.x += 10;
			else if (keycode == UP_ARROW)
				sphere->position.y -= 10;
			else if (keycode == DOWN_ARROW)
				sphere->position.y += 10;
		}
		temp = temp->next;
	}
}

int	key_hook(int keycode, t_rt *rt)
{
	if (rt->rendering)
		return (0);
	// move_sphere(rt, keycode);
	printf("keycode: %d\n", keycode);
	if (keycode == 65307)
		destroy(rt);
	// else if (keycode == LEFT_ARROW)
	// 	rt->camera.position.x -= 10;
	// else if (keycode == RIGHT_ARROW)
	// 	rt->camera.position.x += 10;
	// else if (keycode == UP_ARROW)
	// 	rt->camera.position.y -= 10;
	// else if (keycode == DOWN_ARROW)
	// 	rt->camera.position.y += 10;
	else if (keycode == SPACE)
		rt->anti_aliasing = !rt->anti_aliasing;
	render(rt);
	return (0);
}

int get_width(void) {
	Display* d = XOpenDisplay(NULL);
	Screen*  s = DefaultScreenOfDisplay(d);
	int width = s->width;
	XCloseDisplay(d);
	return (width / 4);
}

int get_current_color(t_rt *rt, int x, int y)
{
	char *pxl;
	int color;

	pxl = rt->img.addr + (y * rt->img.line_length + x * (rt->img.bits_per_pixel / 8));
	color = *(unsigned int *)pxl;
	return (color);
}

// int	handle_mouse_movements(int x, int y, t_rt *rt)
// {
// 	int	color;
// 	if (!rt || rt->rendering)
// 		return (0);
// 	if (rt->rendered)
// 	{
// 		color = get_current_color(rt, x, y);
// 		printf("color: %d\n", color);
// 	}
// 	return (0);
// }

void	init_rt(t_rt *rt)
{
	rt->rendered = 0;
	rt->rendering = 0;
	rt->height = HEIGHT;
	rt->width = WIDTH;
	rt->mlx = mlx_init();
	rt->win = mlx_new_window(rt->mlx, WIDTH, HEIGHT, "miniRT");
	rt->img.img = mlx_new_image(rt->mlx, WIDTH, HEIGHT);
	rt->img.addr = mlx_get_data_addr(rt->img.img, &rt->img.bits_per_pixel,
			&rt->img.line_length, &rt->img.endian);
	rt->object_count = 0;
}

int main(int ac, char **av)
{
	t_rt	*rt;

	// global_fd = open("logs", O_CREAT | O_RDWR | O_TRUNC);
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
	rt->light = NULL;
	open_file(rt, av[1]);
	init_rt(rt);
	render(rt);
	printf("Finished!\n");
	mlx_key_hook(rt->win, key_hook, rt);
	// mlx_mouse_hook(rt->win, handle_mouse_movements, rt);
	mlx_hook(rt->win, 17, 0, destroy, rt);
	mlx_loop(rt->mlx);
	return (0);
}
