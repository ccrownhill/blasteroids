#include <math.h>
#include <allegro5/allegro_primitives.h>

#include "blasteroids.h"
#include "asteroid.h"

Asteroid* init_asteroid(float init_x, float init_y, ALLEGRO_COLOR color)
{
	Asteroid* asteroid = malloc(sizeof(Asteroid));
	asteroid->sx = init_x;
	asteroid->sy = init_y;
	asteroid->heading = 0.0;
	asteroid->twist = 0.0;
	asteroid->speed = ASTEROID_MIN_SPEED + (ASTEROID_MAX_SPEED - ASTEROID_MIN_SPEED)/2;
	asteroid->rot_velocity = ASTEROID_MIN_ROT_SPEED + (ASTEROID_MAX_ROT_SPEED - ASTEROID_MIN_ROT_SPEED)/2;
	asteroid->scale = 1.0;
	asteroid->gone = 0;
	asteroid->color = color;
	return asteroid;
}

void move_asteroid(Asteroid* a)
{
	a->sx += cosf(a->heading + PI/2) * (-a->speed) * (1.0/FPS); // negative speed because with allegro the upwards direction is negative
	a->sy += sinf(a->heading + PI/2) * (-a->speed) * (1.0/FPS);

	if (a->sx > WIDTH)
		a->sx = 0.0;
	else if (a->sx < 0.0)
		a->sx = WIDTH;
	if (a->sy > HEIGHT)
		a->sy = 0.0;
	else if (a->sy < 0.0)
		a->sy = HEIGHT;

	a->twist += a->rot_velocity * (1.0/FPS);	
}

void draw_asteroid(Asteroid* a)
{
	ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
	al_scale_transform(&transform, a->scale, a->scale);
	al_rotate_transform(&transform, a->twist);
	al_translate_transform(&transform, a->sx, a->sy);
	al_use_transform(&transform);
	al_draw_line(-20, 20, -25, 5, a->color, 2.0f);
	al_draw_line(-25, 5, -25, -10, a->color, 2.0f);
	al_draw_line(-25, -10, -5, -10, a->color, 2.0f);
	al_draw_line(-5, -10, -10, -20, a->color, 2.0f);
	al_draw_line(-10, -20, 5, -20, a->color, 2.0f);
	al_draw_line(5, -20, 20, -10, a->color, 2.0f);
	al_draw_line(20, -10, 20, -5, a->color, 2.0f);
	al_draw_line(20, -5, 0, 0, a->color, 2.0f);
	al_draw_line(0, 0, 20, 10, a->color, 2.0f);
	al_draw_line(20, 10, 10, 20, a->color, 2.0f);
	al_draw_line(10, 20, 0, 15, a->color, 2.0f);
	al_draw_line(0, 15, -20, 20, a->color, 2.0f);
}

void destroy_asteroid(Asteroid* a)
{
	free(a);
}
