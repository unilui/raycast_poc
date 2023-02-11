#include "raycast.h"

int	main(int argc, char **argv)
{
	t_screen screen;

	screen = load_mlx(1280, 720);
	load_hooks(&screen);
	mlx_loop(screen.mlx);
	return (0);
}
