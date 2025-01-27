/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:42:52 by zelbassa          #+#    #+#             */
/*   Updated: 2025/01/25 14:25:43 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "mlx/mlx.h"
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

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
#define WIDTH 1360
#define HEIGHT 800

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

typedef struct s_object
{
	t_type		type;
	void		*object;
	t_object	*next;
}	t_object;

typedef struct s_rt
{
	void		*mlx;
	void		*win;
	int			file_fd;
	int			object_count;
	char		*file;
	t_ambient	ambient;
	t_camera	camera;
	t_img		img;
	t_light		light;
	t_object	*object;
}	t_rt;

/*
unsigned int	random_int(int state);
void			my_mlx_pixel_put(t_img *data, int x, int y, int color);
int				check_collision(t_object *object, int x, int y);
void			check_hit(t_rt *rt, int x, int y);
void			init_rays(t_rt *rt, int width, int height);
void			init(t_rt *rt, int width, int height);
void			render(t_rt *rt, int width, int height);
int				key_hook(int keycode, t_rt *rt);
int				is_hit(t_rt *rt, int x, int y);
void			init_scene(t_rt *rt);*/
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
void			parsing_error(t_rt *rt, char *msg);
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

