#include "raycast.h"

#define BOX_SIZE 64

#define PLAYER_Y 224
#define PLAYER_X 224

#define ALTURA 7
#define LARGURA 7

#define OUT_OF_LIMITS -1
#define NOT_HIT 1

int worldMap[ALTURA][LARGURA]=
{
	{1,1,1,1,1,1,1},
	{1,0,0,0,0,0,1},
	{1,0,0,0,0,0,1},
	{1,0,0,1,0,0,1},
	{1,0,0,0,0,0,1},
	{1,0,0,0,0,0,1},
	{1,1,1,1,1,1,1}
};

double radians(int nbr)
{
	return (nbr * (M_PI / 180));
}

typedef struct s_hitbox
{
	int	y;
	int	x;
	int	delta_y;
	int	delta_x;
} t_hitbox;

typedef struct s_point
{
	int	x;
	int	y;
} t_point;

typedef struct s_hit
{
	int	distance;
	int	side;
	int	x;
	int	y;
} t_hit;

t_hitbox	next_hhitbox (int angle) // Needs work
{
	t_hitbox	hitbox;
	int			correction;
	int			y_direction;
	int			x_direction;

	if (angle < 180) // ray facing up
	{
		correction = -1;
		y_direction = -1;
		x_direction = 1;
	}
	else // ray facing down
	{
		correction = BOX_SIZE;
		y_direction = 1;
		x_direction = -1;
	}
	hitbox.delta_y = BOX_SIZE * y_direction;
	hitbox.delta_x = BOX_SIZE / tan(radians(angle)) * x_direction;
	hitbox.y = floor((double)PLAYER_Y / BOX_SIZE) * BOX_SIZE + correction;
	hitbox.x = PLAYER_X + ((PLAYER_Y - hitbox.y) / tan(radians(angle)));
	return (hitbox);
}

t_hitbox	next_vhitbox (int angle) // Needs work
{
	t_hitbox	hitbox;
	int			correction;
	int			x_direction;
	int			y_direction;

	if (angle < 90 || angle > 270) // ray facing right
	{
		correction = BOX_SIZE;
		x_direction = 1;
		y_direction = -1;
	}
	else // ray facing left
	{
		correction = -1;
		x_direction = -1;
		y_direction = 1;
	}
	hitbox.delta_x = BOX_SIZE * x_direction;
	hitbox.delta_y = BOX_SIZE * tan(radians(angle)) * y_direction;
	hitbox.x = floor((double)PLAYER_X / BOX_SIZE) * (BOX_SIZE) + correction;
	hitbox.y = PLAYER_Y + (PLAYER_X - hitbox.x) * tan(radians(angle));
	return (hitbox);
}

t_point cast(t_hitbox init_hitbox)
{
	t_hitbox	hitbox;
	t_point		point;
	int			box_y;
	int			box_x;

	hitbox = init_hitbox;
	point.y = -1;
	point.x = -1;
	while (NOT_HIT)
	{
		box_y = hitbox.y / BOX_SIZE;
		box_x = hitbox.x / BOX_SIZE;
		if (box_x < 0 || box_x > LARGURA - 1)
			return (point);
		if (box_y < 0 || box_y > ALTURA - 1)
			return (point);
		if (worldMap[box_y][box_x] == 1)
			break ;
		hitbox.y += hitbox.delta_y;
		hitbox.x += hitbox.delta_x;
	}
	point.y = hitbox.y;
	point.x = hitbox.x;
	return (point);
}

int	h_side(int x_coordinate)
{
	if ((x_coordinate % BOX_SIZE) > (BOX_SIZE / 2))
		return (W_EAST);
	return (W_WEST);
}

char	v_side(int y_coordinate)
{
	if ((y_coordinate % BOX_SIZE) > (BOX_SIZE / 2))
		return (W_SOUTH);
	return (W_NORTH);
}

t_hit	h_ray(int angle, int player_pov)
{
	t_hitbox	hitbox;
	t_point		point;
	t_hit		hit;

	hit.distance = -1;
	hit.side = -1;
	hitbox = next_hhitbox(angle);
	point = cast(hitbox);
	if (point.x == -1 && point.y == -1)
		return (hit);
	hit.distance = abs(PLAYER_Y - point.y) / sin(radians(angle));
	hit.side = v_side(point.y);
	hit.x = point.x;
	hit.y = point.y;
	int b_angle = angle - player_pov; // Turn into function
	hit.distance = hit.distance * cos(radians(b_angle));
	hit.distance = abs(hit.distance);
	return (hit);
}

t_hit	v_ray(int angle, int player_pov)
{
	t_hitbox	hitbox;
	t_point		point;
	t_hit		hit;

	hit.distance = -1;
	hit.side = -1;
	hitbox = next_vhitbox(angle);
	point = cast(hitbox);
	if (point.x == -1 && point.y == -1)
		return (hit);
	hit.distance = abs(PLAYER_X - point.x) / cos(radians(angle));
	hit.side = h_side(point.x);
	hit.x = point.x;
	hit.y = point.y;
	int b_angle = angle - player_pov; // Turn into function
	hit.distance = hit.distance * cos(radians(b_angle));
	hit.distance = abs(hit.distance);
	return (hit);
}

t_hit	raycast(int direction, int player_pov)
{
	t_hit	horizontal;
	t_hit	vertical;

	if (direction < 0)
		direction = 360 + direction;
	if (player_pov < 0)
		player_pov = 360 + player_pov;
	horizontal.distance = -1;
	vertical.distance = -1;
	horizontal = h_ray(direction, player_pov);
	vertical = v_ray(direction, player_pov);
	if (horizontal.distance == OUT_OF_LIMITS)
		return (vertical);
	if (vertical.distance == OUT_OF_LIMITS)
		return (horizontal);
	if (vertical.distance < horizontal.distance)
		return (vertical);
	return (horizontal);
}

int	rgb_encode(short int red, short int green, short int blue)
{
	t_rgb	color;

	if (red < 0 || red > 255)
		red = 255;
	if (green < 0 || green > 255)
		green = 255;
	if (blue < 0 || blue > 255)
		blue = 255;
	color.rgb[3] = 0;
	color.rgb[2] = red;
	color.rgb[1] = green;
	color.rgb[0] = blue;
	return (color.color);
}

void	wall_pixel_put(t_screen *screen, t_point px, t_hit *hit, int height, int wall_y)
{
	int		color;
	t_img	*wall;
	double	texture_x;

	if (hit->side == W_NORTH || hit->side == W_SOUTH)
		texture_x = hit->x % 64;
	else if (hit->side == W_WEST || hit->side == W_EAST)
		texture_x = hit->y % 64;
	wall = &screen->walls[hit->side];
	color = pixel_get(
			wall,
			texture_x,
			floor(((double)wall->height / height) * wall_y));
	pixel_put(&screen->img, px.x, px.y, color);
}

int	what_im_doing(t_screen *screen, int player_pov) // Also known as raycast
{
	float direction;
	t_hit	hit;

	float angle_rays = 60 / 1280.0;
	//printf("Angle: %f", angle_rays); // Need fix that messy
	direction = player_pov + 30;
	int projection_distance = 720.0 / tan(radians(30));
	int i = 0;
	while (direction > (player_pov - 30.0))
	{
		//printf("\033c");
		hit = raycast(direction, player_pov);
		int height = ceil(64 / (float)hit.distance * projection_distance); // Round up
		int j = 360 - (height / 2);
		int x = 0;
		while (x <= height)
		{
			wall_pixel_put(screen, (t_point){i, j}, &hit, height, x);
			j++;
			x++;
		}
		/*
		printf("Ray: %d\n", i);
		printf("Height: %d\n", height);
		printf("Pov: %d\n", player_pov);
		printf("Direction: %f\n", direction);
		printf("Distance: %d\n", hit.distance);
		printf("Side: %c\n", hit.side);
		printf("Y: %d - X: %d\n", hit.y, hit.x);
		printf("=====================================\n");
		*/
		direction -= angle_rays;
		i++;
		//usleep(500000);
	}
	return (0);
}

int	render(t_screen *screen)
{
	volatile static int player_pov;
	usleep(20000);
	background(&screen->img, rgb_encode(0, 0, 0));
	what_im_doing(screen, player_pov);
	mlx_put_image_to_window(
		screen->mlx,
		screen->window,
		screen->img.mlx_img, 0, 0);
	player_pov++;
	if (player_pov == 360 || player_pov == -360)
		player_pov = 0;
	return (0);
}
