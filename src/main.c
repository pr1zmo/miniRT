#include "../minirt.h"

int global_fd;

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

void	show_light(t_rt *rt)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (i == rt->light.position.x || j == rt->light.position.y)
				my_mlx_pixel_put(&rt->img, j, i, 0xffffff);
		}
	}
}

void	build_rays(t_rt *rt, t_ray *ray, double x, double y)
{
	ray->direction = rt->camera.orientation;
	ray->origin = rt->camera.position;
}

void	*init_rays(void	*thread_data)
{
	// char	*progress;
	t_rt	*rt = (t_rt *)thread_data;

	show_light(rt);
	for (int i = 0; i < rt->height; i++)
	{
		// char *temp = ft_strdup("progress: ");
		// progress = ft_strjoin(temp, ft_itoa(i));
		// mlx_string_put(rt->mlx, rt->win, WIDTH / 2, HEIGHT / 2, 0xffffff, progress);
		// free(progress);
		// free(temp);
		for (int j = 0; j < rt->width; j++)
		{
			build_rays(rt, rt->ray, i, j);
			r_trace(rt, j, i);
		}
	}
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

void	render(t_rt *rt)
{
	pthread_t	thread[20];

	mlx_clear_window(rt->mlx, rt->win);
	rt->light.color = (t_color){255, 255, 255};
	for (int i = 0; i < 20; i++) {
		pthread_create(&thread[i], NULL, init_rays, rt);
	}
	for (int k = 0; k < 20; k++) {
		pthread_join(thread[k], NULL);
	}
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
	render(rt);
	return (0);
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
	open_file(rt, av[1]);
	rt->height = HEIGHT;
	rt->width = WIDTH;
	rt->mlx = mlx_init();
	rt->win = mlx_new_window(rt->mlx, WIDTH, HEIGHT, "miniRT");
	rt->img.img = mlx_new_image(rt->mlx, WIDTH, HEIGHT);
	rt->img.addr = mlx_get_data_addr(rt->img.img, &rt->img.bits_per_pixel,
			&rt->img.line_length, &rt->img.endian);
	rt->object_count = 0;
	render(rt);
	mlx_key_hook(rt->win, key_hook, rt);
	// mlx_mouse_hook(rt->win, handle_mouse_movements, rt);
	mlx_hook(rt->win, 17, 0, destroy, rt);
	mlx_loop(rt->mlx);
	return (0);
}
