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

void	init_rays(t_rt *rt, int width, int height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			r_trace(rt, i, j);
			if (i == rt->light.position.x || j == rt->light.position.y)
				my_mlx_pixel_put(&rt->img, j, i, 0xffffff);
		}
	}
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

void	render(t_rt *rt, int width, int height)
{
	mlx_clear_window(rt->mlx, rt->win);
	rt->light.color = (t_color){255, 255, 255};
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

int main(int ac, char **av)
{
	t_rt	*rt;

	global_fd = open("logs", O_CREAT | O_RDWR | O_TRUNC);
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
	rt->object_count = 0;
	render(rt, WIDTH, HEIGHT);
	mlx_key_hook(rt->win, key_hook, rt);
	mlx_hook(rt->win, 17, 0, destroy, rt);
	mlx_loop(rt->mlx);
	return (0);
}
