#include "raycast.h"

void	pixel_put(t_img *img, int x, int y, int color)
{
	char	*addr;
	int		*pixel;
	int		y_map;
	int		x_map;

	y_map = y * img->line_len;
	x_map = x * (img->bpp / 8);
	addr = img->addr + y_map + x_map;
	pixel = (int *)addr;
	*pixel = color;
}

int	pixel_get(t_img *img, int x, int y)
{
	char	*px;

	px = img->addr + y * img->line_len + (x * (img->bpp / 8));
	return (*(unsigned int *)px);
}

int	background(t_img *img, int color)
{
	int	x;
	int	y;

	y = 0;
	while (y <= 720)
	{
		x = 0;
		while (x <= 1280)
			pixel_put(img, x++, y, color);
		y++;
	}
	return (0);
}
