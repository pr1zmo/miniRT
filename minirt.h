/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:42:52 by zelbassa          #+#    #+#             */
/*   Updated: 2025/02/25 12:52:10 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "mlx/mlx.h"
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <X11/Xlib.h>

#define C 99
#define ESCAPE 65307
#define LEFT_ARROW 65361
#define RIGHT_ARROW 65363
#define DOWN_ARROW 65364
#define UP_ARROW 65362
#define EVENT_KEY_DOWN 2
#define ENTER 65293
#define LEFT_SHIFT 65505
#define H 104
#define W 119
#define A 97
#define S 115
#define D 100
#define WIDTH 1260
#define BOUNCES 3
#define ASPECT_RATIO (16.0 / 9.0)

extern int global_fd;

#define HEIGHT (int)(WIDTH / ASPECT_RATIO)
// double viewport_height = 2.0 * tan(fov / 2.0);
// double viewport_width = viewport_height * ((double)WIDTH / HEIGHT);

typedef struct s_object t_object;

typedef enum e_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	TRIANGLE
}	t_type;

typedef struct s_vector
{
	double	x;
	double	y;
	double	z;
}	t_vector;

typedef struct s_color
{
	double	r;
	double	g;
	double	b;
}	t_color;

typedef struct s_ray
{
	t_vector	origin;
	t_vector	direction;
}	t_ray;

typedef struct s_ambient
{
	double	lighting;
	t_color	color;
}	t_ambient;

typedef struct s_light
{
	t_vector	position;
	t_color		color;
	double		brightness;
}	t_light;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_camera
{
	t_vector	position;
	t_vector	orientation;
	double		fov;
}	t_camera;

typedef struct s_sphere
{
	t_vector	position;
	double		diameter;
	t_color		color;
}	t_sphere;

typedef struct s_plane
{
	t_vector	position;
	t_vector	direction;
	t_color		color;
}	t_plane;

typedef struct s_cylinder
{
	t_vector	position;
	t_vector	direction;
	double		diameter;
	double		height;
	t_color		color;
}	t_cylinder;

typedef struct s_object_interface t_object_interface;

typedef struct s_object
{
	t_type				type;
	void				*object;
	t_object_interface	*interface;
	t_object			*next;
}	t_object;

typedef struct s_hit_info
{
	int			hit_count;
	double		dist;
	t_vector	hit_point;
	t_vector	normal;
	t_color		color;
	t_object	*closest_object;
}	t_hit_info;

typedef struct s_object_interface
{
	int	(*check_hit)(t_object *object, int x, int y);
	int	(*set_color)(t_object *object, int x, int y);
}	t_object_interface;

typedef struct s_rt
{
	void		*mlx;
	void		*win;
	int			file_fd;
	int			object_count;
	char		*file;
	int			height;
	int			width;
	t_ray		*ray;
	t_ambient	ambient;
	t_camera	camera;
	t_img		img;
	t_light		light;
	t_object	*object;
}	t_rt;

unsigned int	random_int(int state);
void			my_mlx_pixel_put(t_img *data, int x, int y, int color);
// int				check_collision(t_object *object, int x, int y);
// void			check_hit(t_rt *rt, int x, int y);
void*			init_rays(void *thread_data);
// void			init(t_rt *rt, int width, int height);
void			render(t_rt *rt);
int				key_hook(int keycode, t_rt *rt);
void			ft_add_back(t_object **list, t_object *new, int type);
int				check_file(t_rt *rt);
void			list_objects(t_rt *rt);
int				arg_error(char *msg);
int				is_rt_file(char *path);
int				destroy(t_rt *rt);
void			open_file(t_rt *rt, char *path);
void			free_objects(t_object *object);
int				main(int ac, char **av);
int				parse(t_rt *rt);
double			atoi_double(char *str);
void			free_array(char *arr[]);
int				array_length(char **arr);
void			parsing_error(char *msg);
int				is_float(const char *c);
int				check_range(char *line, int count, const int range[2]);
void			set_direction(char *line, t_vector *directions);
void			set_rgb(char *line, t_color *colors);
int				parse_sphere(t_rt *rt, char *line);
void			set_sphere(char **line, t_object **object);
int				parse_plane(t_rt *rt, char *line);
void			set_plane(char **line, t_object **object);
int				parse_cylinder(t_rt *rt, char *line);
void			set_cylinder(char **line, t_object **object);
int				parse_ambient(t_rt *rt, char *line);
void			set_ambient(char **line, t_ambient *ambient);
int				parse_camera(t_rt *rt, char *line);
void			set_camera(char **line, t_camera *camera);

// Debugging functions
void			show_sphere(t_sphere *sphere);
void			show_plane(t_object *object);
void			show_cylinder(t_object *object);
void			list_objects(t_rt *rt);

//vectors
t_vector		vector_subtract(t_vector a, t_vector b);
t_vector		vector_add(t_vector a, t_vector b);
t_vector		vector_scale(t_vector v, double scale);
double			vector_dot(t_vector a, t_vector b);
t_vector		vector_normalize(t_vector v);
t_vector		vector_cross(t_vector a, t_vector b);
t_vector		scale_vector(double s, t_vector v);

//ray tracer
void			r_trace(t_rt *rt, int x, int y);

// events
int				handle_mouse_movements(int x, int y, t_rt *rt);

// scene

t_ray			get_ray(t_rt *rt, int x, int y);

// int check_sphere(t_rt *rt, int x, int y)
// {
// 	t_sphere *sphere;
// 	double r, a, b, c, discriminant;
// 	t_vector d, sub;
// 	t_vector Q = {0, 0, 0}; // Ray origin at (0,0,0)
// 	t_vector curr = {x, y, 1}; // Current pixel location in 3D
//
// 	// Get sphere object
// 	sphere = (t_sphere *)rt->object;
// 	r = sphere->diameter / 2;
//
// 	// Compute ray direction d (from camera to pixel)
// 	d = vector_subtract(curr, Q);
// 	d = vector_normalize(d); // Ensure d is a unit vector
//
// 	// Compute coefficients for quadratic equation
// 	sub = vector_subtract(Q, sphere->position);
// 	a = vector_dot(d, d);
// 	b = 2 * vector_dot(d, sub);
// 	if (vector_dot(sub, sub) < pow(r, 2))
// 		return (1);
// 	c = vector_dot(sub, sub) - (r * r);
// 	// Compute discriminant
// 	discriminant = (b * b) - (4 * a * c);
//
// 	// Check if ray intersects the sphere
// 	if (discriminant < 0)
// 		return (0); // No intersection
// 	return (0); // Intersection detected
// }