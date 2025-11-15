#ifndef MINIRT_H
# define MINIRT_H

# include "libft.h"
# include "MLX42.h"
# include <errno.h>
# include <math.h>

/******************************************************************************/
/*                                   MACROS                                   */
/******************************************************************************/

# define FILE_EXTENSION	".rt"
# define PATH_SEPARATOR	'/'

# define INT_MAX_COLOR	255
# define INT_MAX_FOV	180

# define WIDTH	1080
# define HEIGHT	720

# define PITCH_CHANGE	5
# define YAW_CHANGE		10

# define EPSILON	0.001

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

/******************************************************************************/
/*                                   TYPES                                    */
/******************************************************************************/

typedef enum e_error		t_error;

typedef struct s_scene		t_scene;

typedef struct s_color		t_color;
typedef struct s_v3			t_coords;
typedef struct s_v3			t_vector;
typedef struct s_v3			t_v3;
typedef struct s_point		t_point;

typedef struct s_ambient	t_ambient;
typedef struct s_camera		t_camera;
typedef struct s_light		t_light;
typedef struct s_sphere		t_sphere;
typedef struct s_plane		t_plane;
typedef struct s_cylinder	t_cylinder;

/******************************************************************************/
/*                                   ENUMS                                    */
/******************************************************************************/

enum e_error
{
	OK = 0,
	ERR_ALLOC,
	ERR_ARGUMENTS,
	ERR_EXTENSION,
	ERR_OPEN,
	ERR_IDENTIFIER,
	ERR_END_LINE,
	ERR_SPACES,
	ERR_AMB_NONE,
	ERR_AMB_SEVERAL,
	ERR_AMB_RATIO,
	ERR_AMB_COLOR,
	ERR_CAM_NONE,
	ERR_CAM_SEVERAL,
	ERR_CAM_COORDS,
	ERR_CAM_ORIENTATION,
	ERR_CAM_FOV,
	ERR_LIG_NONE,
	ERR_LIG_SEVERAL,
	ERR_LIG_COORDS,
	ERR_LIG_RATIO,
	ERR_LIG_COLOR,
	ERR_PLA_COORDS,
	ERR_PLA_NORMAL,
	ERR_PLA_COLOR,
	ERR_SPH_COORDS,
	ERR_SPH_DIAMETER,
	ERR_SPH_COLOR,
	ERR_CYL_COORDS,
	ERR_CYL_AXIS,
	ERR_CYL_DIAMETER,
	ERR_CYL_HEIGHT,
	ERR_CYL_COLOR,
	ERR_MLX_INIT,
	ERR_MLX_IMG,
	ERR_MLX_PRINT,
	MAX_ERR_CODE,
};

/******************************************************************************/
/*                                  STRUCTS                                   */
/******************************************************************************/

struct s_color
{
	int	red;
	int	green;
	int	blue;
};

struct s_v3
{
	double	x;
	double	y;
	double	z;
};

struct s_ambient
{
	double	ratio;
	t_color	color;
};

struct s_camera
{
	t_coords	coords;
	t_vector	orientation;
	t_vector	forward;
	t_vector	right;
	t_vector	up;
	double		pitch;
	double		pitch_to_change;
	double		yaw;
	double		yaw_to_change;
	int			fov;
};

struct s_light
{
	t_coords	coords;
	double		ratio;
};

struct s_sphere
{
	t_coords	coords;
	double		radius;
	t_color		color;
	t_sphere	*next;
};

struct s_plane
{
	t_coords	coords;
	t_vector	normal;
	t_color		color;
	t_plane		*next;
};

struct s_cylinder
{
	t_coords	coords;
	t_vector	axis;
	double		radius;
	double		half_height;
	t_color		color;
	t_cylinder	*next;
};

struct s_scene
{
	t_ambient	*ambient;
	t_camera	*camera;
	t_light		*light;
	t_sphere	*sphere_list;
	t_plane		*plane_list;
	t_cylinder	*cylinder_list;
	char		*line;
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_color		background_color;
	t_vector	movement;
	t_vector	world_east;
	t_vector	world_up;
	t_vector	world_south;
};

struct s_point
{
	t_coords	coords;
	t_color		color;
	double		closest;
	t_vector	cam_ray;
	t_vector	light_ray;
	double		light_distance;
	t_vector	normal;
	t_coords	coords_adjusted;
};

/******************************************************************************/
/*                                 FUNCTIONS                                  */
/******************************************************************************/

double		deg_to_rad(double x);
void		free_exit(t_scene *scene, t_error error);
double		rad_to_deg(double x);
uint32_t	rgb_to_uint(t_color *color);

/******************************************************************************/
/*                              FUNCTIONS - MLX                               */
/******************************************************************************/

void		close_hook(void *param);
void		key_hook(mlx_key_data_t keydata, void *param);
void		loop_hook(void *param);

/******************************************************************************/
/*                             FUNCTIONS - PARSE                              */
/******************************************************************************/

void		parse_file(t_scene *scene, char *path);
void		parse_line(t_scene *scene, char *line);

void		parse_ambient(t_scene *scene, char **line);
void		parse_camera(t_scene *scene, char **line);
void		parse_light(t_scene *scene, char **line);

void		parse_plane(t_scene *scene, char **line);
void		parse_sphere(t_scene *scene, char **line);
void		parse_cylinder(t_scene *scene, char **line);

/******************************************************************************/
/*                           FUNCTIONS - PARSE/READ                           */
/******************************************************************************/

bool		read_color(char **line, t_color *color);
bool		read_double_positive_halfed(char **line, double *result);
bool		read_double_ratio(char **line, double *result);
bool		read_double(char **line, double *result);
bool		read_int_maxed(char **line, int *result, int max_range);
bool		read_v3_normalized(char **line, t_v3 *v3);
bool		read_v3(char **line, t_v3 *v3);
bool		skip_spaces(char **line);

/******************************************************************************/
/*                             FUNCTIONS - RENDER                             */
/******************************************************************************/

void		apply_lights(t_point *point,
				t_ambient *ambient, t_light *light, bool in_shadow);

void		render(t_scene *scene);
void		rotate_camera(t_scene *scene);
void		move_camera(t_scene *scene);
void		update_camera_axis(t_scene *scene, t_camera *cam);

t_vector	get_ray_direction(t_scene *scene, int i, int j);
t_coords	ray_at(t_coords origin, t_vector direction, double distance);

bool		is_in_height(t_cylinder *cylinder,
				t_coords origin, t_vector direction, double distance);
bool		is_closer(t_scene *scene,
				t_point *point, double intersection, t_color color);

/******************************************************************************/
/*                        FUNCTIONS - RENDER/OBJECTS                          */
/******************************************************************************/

void		intersect_cylinders(t_scene *scene, t_point *point);
bool		crash_with_cylinders(t_scene *scene, t_point *point);

void		intersect_planes(t_scene *scene, t_point *point);
bool		crash_with_planes(t_scene *scene, t_point *point);

void		intersect_spheres(t_scene *scene, t_point *point);
bool		crash_with_spheres(t_scene *scene, t_point *point);

double		solve_plane(t_plane *plane, t_coords origin, t_vector direction);
double		solve_sphere(t_sphere *sphere, t_coords origin, t_vector direction);
double		solve_cylinder(t_cylinder *cylinder, t_coords origin, t_vector dir);
double		solve_caps(t_cylinder *cyl, t_coords origin, t_vector dir);
/******************************************************************************/
/*                             FUNCTIONS - VECTOR                             */
/******************************************************************************/

t_v3		v3_add(t_v3 a, t_v3 b);
t_v3		v3_build(double x, double y, double z);
t_v3		v3_cross_product(t_v3 a, t_v3 b);
double		v3_dot_product(t_v3 a, t_v3 b);
double		v3_magnitude(t_v3 v3);
t_v3		v3_normalize(t_v3 v3);
t_v3		v3_scale(t_v3	v3, double scale);
t_v3		v3_substract(t_v3 a, t_v3 b);

#endif