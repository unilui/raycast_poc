# include <stdlib.h>
# include <fcntl.h>
# include <mlx.h>
# include <X11/keysym.h>
# include <math.h>
#include <stdio.h>
#include <unistd.h>

enum	e_wall
{
	W_NORTH,
	W_SOUTH,
	W_WEST,
	W_EAST,
	W_COUNT,
};

typedef union u_color
{
	int		color;
	char	rgb[4];
}	t_rgb;

typedef struct s_img
{
	void	*mlx_img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_screen
{
	void		*mlx;
	void		*window;
	t_img		img;
	t_img		walls[W_COUNT];
}	t_screen;


t_screen	load_mlx(int width, int height);
void	load_hooks(t_screen *screen);
int	render(t_screen *screen);
int	background(t_img *img, int color);
void	pixel_put(t_img *img, int x, int y, int color);
int	pixel_get(t_img *img, int x, int y);
int	exit_raycast(t_screen *screen);
