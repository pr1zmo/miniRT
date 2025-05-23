/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:42:52 by zelbassa          #+#    #+#             */
/*   Updated: 2025/05/23 16:16:54 by zelbassa         ###   ########.fr       */
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
#define SPACE 32
#define WIDTH 1366
#define BOUNCES 3
#define ASPECT_RATIO (16.0 / 9.0)

extern int global_fd;

#define HEIGHT (int)(WIDTH / ASPECT_RATIO)
// double viewport_height = 2.0 * tan(fov / 2.0);
// double viewport_width = viewport_height * ((double)WIDTH / HEIGHT);

typedef struct s_object t_object;
typedef struct s_light t_light;

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

typedef struct s_vec4
{
	double x;
	double y;
	double z;
	double w;
}	t_vec4;

typedef struct s_color
{
	double	r;
	double	g;
	double	b;
}	t_color;

typedef struct s_ray
{
	t_vector	origin;
	t_vector	orientation;
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
	t_light		*next;
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
	t_vector	p1;
	t_vector	p2;
	t_color		color;
}	t_cylinder;

typedef struct s_triangle
{
	t_vector	p1;
	t_vector	p2;
	t_vector	p3;
	t_color		color;
}	t_triangle;

typedef struct s_object
{
	t_type				type;
	t_vector			position;
	t_color				color;
	double				ambient;
	double				diffuse;
	double				shininess;
	double				specular;
	double				emissive;
	void				*object;
	t_object			*next;
}	t_object;

/*
typedef struct s_object {
    t_type      type;
    void        *object;
    t_object    *next;
} t_object;
*/

typedef struct s_hit_info
{
	int			hit;
	double		dist;
	t_vector	hit_point;
	t_vector	normal;
	t_color		color;
	t_object	*closest_object;
}	t_hit_info;

typedef struct s_rt
{
	void		*mlx;
	void		*win;
	int			file_fd;
	int			object_count;
	char		*file;
	int			height;
	int			width;
	int			anti_aliasing;
	int			rendered;
	int			rendering;
	double		info;
	t_ray		*ray;
	t_ambient	ambient;
	t_camera	camera;
	t_img		img;
	t_light		*light;
	t_object	*object;
}	t_rt;

typedef struct s_thread_data
{
	t_rt	*rt;
	int		start_row;
	int		end_row;
}	t_thread_data;

unsigned int	random_int(void);
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
int				parse_triangle(t_rt *rt, char *line);
void			set_triangle(char **line, t_object **object);
void			set_camera(char **line, t_camera *camera);

// Debugging functions
void			show_sphere(t_sphere *sphere);
void			show_plane(t_object *object);
void			show_cylinder(t_object *object);
void			list_objects(t_rt *rt);

//vectors
t_vector		vec_sub(t_vector a, t_vector b);
t_vector		vec_add(t_vector a, t_vector b);
t_vector		vec_scale(t_vector v, double scale);
double			vec_dot(t_vector a, t_vector b);
t_vector		vec_norm(t_vector v);
t_vector		vec_cro(t_vector a, t_vector b);
t_vector		scale_vector(double s, t_vector v);

//ray tracer
void			r_trace(t_rt *rt, int x, int y);
int				intersect(t_rt *rt, int x, int y);
int				compute_lighting(t_rt *rt, t_hit_info *closest_hit);

// events
int				handle_mouse_movements(int x, int y, t_rt *rt);

// scene

t_ray			get_ray(t_rt *rt, int x, int y, t_ray temp_ray);

// intersection

void			set_hit_info(t_hit_info *closest, t_ray *ray, t_object *temp, double t);
t_hit_info		find_closest_object(t_rt *rt, t_ray *ray);
int				sphere_intersect(t_object *object, t_ray *ray, double *t);
int				plane_intersect(t_object *object, t_ray *ray, double *t, t_hit_info *hit);
int				cylinder_intersect(t_object *object, t_ray *ray, double *t);
int				triangle_intersect(t_object *object, t_ray *ray, double *t);

// math
double			scale(int number, int low, int high, int old_low, int old_high);
int				rgb_to_int(t_color c);
double			vec_length(t_vector v);