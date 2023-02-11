#include "raycast.h"

int	exit_raycast(t_screen *screen)
{
	mlx_destroy_image(screen->mlx, screen->img.mlx_img);
	mlx_destroy_image(screen->mlx, screen->walls[W_NORTH].mlx_img);
	mlx_destroy_image(screen->mlx, screen->walls[W_SOUTH].mlx_img);
	mlx_destroy_image(screen->mlx, screen->walls[W_WEST].mlx_img);
	mlx_destroy_image(screen->mlx, screen->walls[W_EAST].mlx_img);
	mlx_destroy_window(screen->mlx, screen->window);
	mlx_destroy_display(screen->mlx);
	free(screen->mlx);
	exit(0);
}
