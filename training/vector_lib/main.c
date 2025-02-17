#include <math.h>

typedef struct s_vector
{
	double	x;
	double	y;
	double	z;
}	t_vector;

t_vector	vector_add(t_vector a, t_vector b)
{
	return ((t_vector){a.x + b.x, a.y + b.y, a.z + b.z});
}

t_vector	vector_sub(t_vector a, t_vector b)
{
	return ((t_vector){a.x - b.x, a.y - b.y, a.z - b.z});
}

double	vector_dot(t_vector a, t_vector b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vector	vector_scale(t_vector a, t_vector b)
{
	return ((t_vector){a.x * b.x, a.y * b.y, a.z * b.z});
}

t_vector	vector_cross(t_vector a, t_vector b)
{
	//
}

t_vector	vector_normalize(t_vector v)
{
	double	length;

	length = sqrt(vector_dot(v, v));
	return ((t_vector){v.x / length, v.y / length, v.z / length});
}

int main()
{
	return (0);
}