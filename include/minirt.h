#ifndef MINIRT_H
# define MINIRT_H

# include "libft.h"
# include <errno.h>

/******************************************************************************/
/*                                   MACROS                                   */
/******************************************************************************/

# define FILE_EXTENSION	".rt"
# define PATH_SEPARATOR	'/'

# define INT_MAX_COLOR	255
# define INT_MAX_FOV	180

/******************************************************************************/
/*                                   TYPES                                    */
/******************************************************************************/

typedef enum e_error		t_error;

typedef struct s_scene		t_scene;

typedef struct s_color		t_color;
typedef struct s_v3			t_coords;
typedef struct s_v3			t_vector;

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
};

/******************************************************************************/
/*                                  STRUCTS                                   */
/******************************************************************************/

struct s_scene
{
	t_ambient	*ambient;
	t_camera	*camera;
	t_light		*light;
	t_sphere	*sphere_list;
	t_plane		*plane_list;
	t_cylinder	*cylinder_list;
};

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
	double		diameter;
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
	double		diameter;
	double		height;
	t_color		color;
	t_cylinder	*next;
};

/******************************************************************************/
/*                                 FUNCTIONS                                  */
/******************************************************************************/

void	free_exit(t_scene *scene, t_error error);
void	parse_file(t_scene *scene, char *path);
void	parse_line(t_scene *scene, char *line);

#endif