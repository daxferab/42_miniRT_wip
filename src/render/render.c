#include "minirt.h"

static bool	has_obstacles(t_scene *scene, t_point *point);
static bool	light_is_behind(t_point *point);

void	render(t_scene *scene)
{
	double	i;
	double	j;
	t_point	point;

	i = 0;
	while (i < WIDTH)
	{
		j = 0;
		while (j < HEIGHT)
		{
			point.closest = -1;
			point.cam_ray = get_ray_direction(scene, i, j);
			change_color(&point.color, 0, 0, 0);
			intersect_planes(scene, &point);
			intersect_spheres(scene, &point);
			intersect_cylinders(scene, &point);
			if (point.closest >= 0)
			{
				point.light_ray = v3_normalize(
						v3_substract(scene->light->coords, point.coords));
				point.light_distance = v3_magnitude(
						v3_substract(scene->light->coords, point.coords));
				point.coords_adjusted = ray_at(
						point.coords, point.normal, EPSILON);
				apply_lights(&point, scene->ambient, scene->light,
					has_obstacles(scene, &point));
			}
			mlx_put_pixel(scene->img, i, j, rgb_to_uint(&point.color));
			j++;
		}
		i++;
	}
}

static bool	light_is_behind(t_point *point)
{
	return (v3_dot_product(point->normal, point->light_ray) < 0);
}

static bool	has_obstacles(t_scene *scene, t_point *point)
{
	return (
		light_is_behind(point)
		|| crash_with_planes(scene, point)
		|| crash_with_spheres(scene, point)
		|| crash_with_cylinders(scene, point)
	);
}
