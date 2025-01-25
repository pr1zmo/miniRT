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
		if (temp->type == SPHERE)
			free(temp->object);
		free(temp);
	}
}
