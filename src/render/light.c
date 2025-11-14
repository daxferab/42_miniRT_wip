#include "minirt.h"

static void	apply_ambient(t_ambient *ambient, t_color *color);
static void	apply_light(
				t_light *light, double distance, t_color color, t_color *final);
static void	clamp_values(t_color *color);

void	apply_lights(t_point *point,
		t_ambient *ambient, t_light *light, bool in_shadow)
{
	t_color	final;
	t_color	add_light;

	change_color(
		&final, point->color.red, point->color.green, point->color.blue);
	apply_ambient(ambient, &final);
	if (!in_shadow)
	{
		change_color(&add_light,
			point->color.red, point->color.green, point->color.blue);
		apply_light(light, point->light_distance, add_light, &final);
	}
	clamp_values(&final);
	point->color = final;
}

static void	apply_ambient(t_ambient *ambient, t_color *color)
{
	color->red *= ambient->color.red * ambient->ratio / 255;
	color->green *= ambient->color.green * ambient->ratio / 255;
	color->blue *= ambient->color.blue * ambient->ratio / 255;
}

static void	apply_light(
		t_light *light, double distance, t_color color, t_color *final)
{
	double	attenuation;
	double	intensity;

	attenuation = 1.0 / (1.0 + 0.1 * distance);
	intensity = light->ratio * attenuation;
	final->red += color.red * intensity;
	final->green += color.green * intensity;
	final->blue += color.blue * intensity;
}

static void	clamp_values(t_color *color)
{
	if (color->red > 255)
		color->red = 255;
	if (color->green > 255)
		color->green = 255;
	if (color->blue > 255)
		color->blue = 255;
}
