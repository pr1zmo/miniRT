#include "../minirt.h"

void	free_array(char *arr[])
{
	int	i;

	i = 0;
	if (arr)
	{
		while (arr[i])
			free(arr[i++]);
		free(arr);
	}
}

void	free_objects(t_object *object)
{
	t_object	*temp;

	while (object)
	{
		temp = object;
		object = object->next;
		free(temp->object);
		free(temp);
	}
}

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
	// if (rt->ray)
	// 	free(rt->ray);
	if (rt->file_fd > 0)
		close(rt->file_fd);
	free(rt);
	exit(0);
}
