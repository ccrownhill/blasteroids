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
	spaceship->heading = PI/2;
	spaceship->speed = 0.0;
	spaceship->color = color;
	spaceship->lives = SPACESHIP_LIVES;
	spaceship->is_invincible = 1;
	spaceship->gone = 0;
  spaceship->score = 0;
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
	s->sx += cosf(s->heading) * (-s->speed) * (1.0/FPS);
	// add sine of heading angle * speed to sy
	s->sy += sinf(s->heading) * (-s->speed) * (1.0/FPS);

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
  if (s->speed > SPACESHIP_MIN_TURNSPEED)
    s->heading += turn_speed * (1.0/FPS);
	s->speed += acceleration * (1.0/FPS);
	if (s->speed < 0.0)
		s->speed = 0.0;
  else if (s->speed > SPACESHIP_MAX_SPEED)
    s->speed = SPACESHIP_MAX_SPEED;
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
					s->heading = PI/2;
					s->is_invincible = 1;
				}
			}
		}
	} else { // wait for SPACESHIP_INVINCIBILITY_TIME seconds
		invincibility_time_counter += (1.0/FPS);
		if (invincibility_time_counter > SPACESHIP_INVINCIBILITY_TIME) {
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
		if (blink_time_counter > SPACESHIP_BLINK_TIME*2)
			blink_time_counter = 0.0;
		else if (blink_time_counter > SPACESHIP_BLINK_TIME) {
			return;
		}
	}
	ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
	al_rotate_transform(&transform, s->heading-PI/2);
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
	lives_spaceship->sx = SPACESHIP_LIVES_X;
	lives_spaceship->sy = SPACESHIP_LIVES_Y;
  lives_spaceship->heading = PI/2; // make them point upwards
	lives_spaceship->color = al_map_rgb(SPACESHIP_LIVES_RED, SPACESHIP_LIVES_GREEN, SPACESHIP_LIVES_BLUE);
	ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
	al_translate_transform(&transform, SPACESHIP_LIVES_X, SPACESHIP_LIVES_Y);
  al_use_transform(&transform);
	int i;
	for (i = 0; i < s->lives; i++) {
		draw_ship(lives_spaceship);
		lives_spaceship->sx += SPACESHIP_LIVES_X;
	}
	free(lives_spaceship);
}

void display_score(ALLEGRO_FONT* font, Spaceship* s)
{
  ALLEGRO_TRANSFORM transform;
  al_identity_transform(&transform);
  al_scale_transform(&transform, SCORE_SCALE, SCORE_SCALE);
  al_translate_transform(&transform, SCORE_X - SPACESHIP_RADIUS, SCORE_Y);
  al_use_transform(&transform);
  al_draw_textf(font, al_map_rgb(SCORE_RED, SCORE_GREEN, SCORE_BLUE), 0, 0, 0, "%i", s->score);
}
