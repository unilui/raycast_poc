#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
	int	y;
	int	x;
} t_point;

typedef struct s_hit
{
	int		distance;
	char	side;
	int		x;
	int		y;
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
	hitbox.y = floor(PLAYER_Y / BOX_SIZE) * BOX_SIZE + correction;
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
	hitbox.x = floor(PLAYER_X / BOX_SIZE) * (BOX_SIZE) + correction;
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

char	h_side(int x_coordinate)
{
	if ((x_coordinate % BOX_SIZE) > (BOX_SIZE / 2))
		return ('E');
	return ('W');
}

char	v_side(int y_coordinate)
{
	if ((y_coordinate % BOX_SIZE) > (BOX_SIZE / 2))
		return ('S');
	return ('N');
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
	hit.x = point.x / 64;
	hit.y = point.y / 64;
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
	hit.x = point.x / 64;
	hit.y = point.y / 64;
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

int main (void)
{
	int pov;
	float direction;
	t_hit	hit;

	float angle_rays = 60 / 320.0;
	printf("Angle: %f", angle_rays);
	pov = 30;
	direction = 0;
	int i = 0;
	while (direction <= 60)
	{
		printf("\033c");
		hit = raycast(direction, pov);
		int height = 64 / (float)hit.distance * 277; // Round up
		printf("Ray: %d\n", i);
		printf("Height: %d\n", height);
		printf("Pov: %d\n", pov);
		printf("Direction: %f\n", direction);
		printf("Distance: %d\n", hit.distance);
		printf("Side: %c\n", hit.side);
		printf("Y: %d - X: %d\n", hit.y, hit.x);
		printf("=====================================\n");
		direction += angle_rays;
		i++;
		usleep(500000);
	}
}
