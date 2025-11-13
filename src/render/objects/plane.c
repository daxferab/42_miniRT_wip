#include "minirt.h"

void	intersect_planes(t_scene *scene, t_point *point)
{
	t_plane	*plane;
	double	intersection;

	plane = scene->plane_list;
	while (plane)
	{
		intersection = solve_plane(
				plane, scene->camera->coords, point->cam_ray);
		if (is_closer(scene, point, intersection))
		{
			point->normal = plane->normal;
			if (v3_dot_product(point->normal, point->cam_ray) > 0)
				point->normal = v3_scale(point->normal, -1);
			change_color(&(point->color),
				plane->color.red, plane->color.green, plane->color.blue);
		}
		plane = plane->next;
	}
}

bool	crash_with_planes(t_scene *scene, t_point *point)
{
	t_plane	*plane;
	double	intersection;

	plane = scene->plane_list;
	while (plane)
	{
		intersection = solve_plane(
				plane, point->coords_adjusted, point->light_ray);
		if (intersection > 0 && intersection < point->light_distance)
			return (true);
		plane = plane->next;
	}
	return (false);
}
