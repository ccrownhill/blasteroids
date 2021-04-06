#include <stdio.h> // for debugging
#include <math.h>
#include <allegro5/allegro_primitives.h>

#include "blasteroids.h"
#include "asteroid.h"
#include "spaceship.h"

Spaceship* init_ship(float init_x, float init_y, ALLEGRO_COLOR color)
{
	Spaceship* spaceship = malloc(sizeof(Spaceship));
	spaceship->sx = init_x;
	spaceship->sy = init_y;
	spaceship->heading = 0.0;
	spaceship->speed = 0.0;
	spaceship->color = color;
	spaceship->lives = LIVES;
	spaceship->is_invincible = 1;
	spaceship->gone = 0;
	return spaceship;
}

// TODO
// Add x and y speed variable to struct
// so that only through acceleration the ship can be turned
// that way the cos(heading)*acceleration would be added to the x speed
// and the sin(heading)*acceleration to the y speed
void move_ship(Spaceship* s, float turn_speed, float acceleration)
{
	// add cosine of heading angle * speed to sx
	s->sx += cosf(s->heading+PI/2) * (-s->speed) * (1.0/FPS);
	// add sine of heading angle * speed to sy
	s->sy += sinf(s->heading+PI/2) * (-s->speed) * (1.0/FPS);

	// make spaceship appear on the
	// opposite screen edge again
	if (s->sx > WIDTH)
		s->sx = 0.0;
	else if (s->sx < 0.0)
		s->sx = WIDTH;
	if (s->sy > HEIGHT)
		s->sy = 0.0;
	else if (s->sy < 0.0)
		s->sy = HEIGHT;

  // don't turn when standing still
  if (s->speed > MINIMUM_TURN_SPEED)
    s->heading += turn_speed * (1.0/FPS);
	s->speed += acceleration * (1.0/FPS); // multiply with frame time
	if (s->speed < 0.0)
		s->speed = 0.0;
  else if (s->speed > MAX_SPEED)
    s->speed = MAX_SPEED;
}

void check_for_collisions(Spaceship* s, Asteroid* first_asteroid)
{
	static float invincibility_time_counter = 0;
	if (!s->is_invincible) {
		Asteroid* i = first_asteroid;
		float distance;
		for(; i != NULL; i = i->next_asteroid) {
			distance = sqrtf((s->sx - i->sx)*(s->sx - i->sx) + (s->sy - i->sy)*(s->sy - i->sy));
			if (distance < ASTEROID_RADIUS*i->scale + SPACESHIP_RADIUS) {
				if (--(s->lives) <= 0) {
					s->gone = 1;
				} else {
					s->sx = WIDTH/2;
					s->sy = HEIGHT/2;
					s->speed = 0.0;
					s->heading = 0.0;
					s->is_invincible = 1;
				}
				puts("Collision!!!");
			}
		}
	} else { // wait for INVINCIBILITY_TIME seconds
		invincibility_time_counter += (1.0/FPS);
		if (invincibility_time_counter > INVINCIBILITY_TIME) {
			s->is_invincible = 0;
			invincibility_time_counter = 0;
		}
	}
}

void draw_ship(Spaceship* s)
{
	static float blink_time_counter = 0.0;
	if (s->is_invincible) {
		blink_time_counter += 1.0/FPS;
		if (blink_time_counter > BLINK_TIME*2)
			blink_time_counter = 0.0;
		else if (blink_time_counter > BLINK_TIME) {
			return;
		}
	}
	ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
	al_rotate_transform(&transform, s->heading);
	al_translate_transform(&transform, s->sx, s->sy);
	al_use_transform(&transform);
	al_draw_line(-8, 9, 0, -11, s->color, 3.0f);
	al_draw_line(0, -11, 8, 9, s->color, 3.0f);
	al_draw_line(-6, 4, -1, 4, s->color, 3.0f);
	al_draw_line(6, 4, 1, 4, s->color, 3.0f);
}

void display_lives(Spaceship* s)
{
	Spaceship* lives_spaceship = malloc(sizeof(Spaceship));
	memset(lives_spaceship, 0, sizeof(Spaceship));
	lives_spaceship->sx = LIVES_X;
	lives_spaceship->sy = LIVES_Y;
	lives_spaceship->color = al_map_rgb(LIVES_RED, LIVES_GREEN, LIVES_BLUE);
	ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
	al_translate_transform(&transform, LIVES_X, LIVES_Y);
	int i;
	for (i = 0; i < s->lives; i++) {
		draw_ship(lives_spaceship);
		lives_spaceship->sx += LIVES_X;
	}
	free(lives_spaceship);
}

void destroy_ship(Spaceship* s)
{
	free(s);
}
