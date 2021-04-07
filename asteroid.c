#include <math.h>
#include <allegro5/allegro_primitives.h>

// includes for random initial movement
#include <stdlib.h>
#include <time.h>

#include "blasteroids.h"
#include "asteroid.h"

// create an asteroid in a random position
// with a random speed
// and a random direction of movement
Asteroid* init_asteroid()
{
	struct timespec current_time;
	clock_gettime(CLOCK_REALTIME, &current_time);
	srandom(current_time.tv_nsec); // seed with current time in nanoseconds
	Asteroid* asteroid = (Asteroid*)malloc(sizeof(Asteroid));
	asteroid->sx = RANDOM_BETWEEN_ZERO_AND_ONE * WIDTH;
	asteroid->sy = RANDOM_BETWEEN_ZERO_AND_ONE * HEIGHT;
	asteroid->heading = RANDOM_BETWEEN_ZERO_AND_ONE * 2*PI;
	asteroid->twist = 0.0;
	asteroid->speed = ASTEROID_MIN_SPEED + RANDOM_BETWEEN_ZERO_AND_ONE*(ASTEROID_MAX_SPEED - ASTEROID_MIN_SPEED);
	asteroid->rot_velocity = ASTEROID_MIN_ROT_SPEED + RANDOM_BETWEEN_ZERO_AND_ONE*(ASTEROID_MAX_ROT_SPEED - ASTEROID_MIN_ROT_SPEED)/2;
	asteroid->scale = INIT_SCALE;
	asteroid->gone = 0;
	asteroid->color = al_map_rgb(ASTEROID_RED, ASTEROID_GREEN, ASTEROID_BLUE);
	asteroid->next = NULL;
	return asteroid;
}

Asteroid* create_asteroid_list(unsigned int num_asteroids)
{
	Asteroid* start = init_asteroid();
	Asteroid* i = start;
	Asteroid* next = NULL;

	int asteroids_created;
	for (asteroids_created = 0; asteroids_created < num_asteroids - 1; asteroids_created++, i = next) {
		next = init_asteroid();
		i->next = next;
	}

	return start;
}

void split_asteroid(Asteroid* a)
{
	// use SPLIT_ASTEROID_MAX_ANGLE_DIFF and SPLIT_ASTEROID_MIN_ANGLE_DIFF
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

void move_asteroid_list(Asteroid* list_start)
{
	Asteroid* i = list_start;
	for(; i != NULL; i = i->next) {
		move_asteroid(i);
	}
}

void draw_asteroid(Asteroid* a)
{
	if (!a->gone) {
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
}

void draw_asteroid_list(Asteroid* list_start)
{
	Asteroid* i = list_start;
	for(; i != NULL; i = i->next) {
		draw_asteroid(i);
	}
}

void clean_gone_asteroids(Asteroid** list_start)
{
	Asteroid* prev = NULL;
	Asteroid* current = *list_start;
	Asteroid* next = NULL;
	for (; current != NULL; current = next) {
		next = current->next;
		if (current->gone) {
			if (prev != NULL)
				prev->next = next;
			else
				*list_start = next;
			free(current);
		} else {
			prev = current;
		}
	}
}

void destroy_asteroids(Asteroid* list_start)
{
	Asteroid* i = list_start;
	Asteroid* next = NULL;
	for (; i != NULL; i = next) {
		next = i->next;
		free(i);
	}
}
