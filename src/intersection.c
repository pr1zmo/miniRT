#include "../minirt.h"

int	sphere_inter(t_sphere *sphere, int x, int y)
{
	(void)sphere;
	(void)x;
	(void)y;

	return (0);
}

void	set_hit_info(t_object *object, t_hit_info *hit_info)
{

}

t_hit_info	*get_closest(t_rt *rt, int x, int y)
{
	t_object	*temp;
	t_hit_info	*hit_info;

	while (temp)
	{
		if (intersect(temp))
	}
	return (hit_info);
}