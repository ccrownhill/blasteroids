#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "blasteroids.h"
#include "spaceship.h"

Spaceship* init_ship(float init_x, float init_y, ALLEGRO_COLOR color)
{
	Spaceship* spaceship = malloc(sizeof(Spaceship));
	spaceship->sx = init_x;
	spaceship->sy = init_y;
	spaceship->heading = 0;
	spaceship->speed = 0;
	spaceship->color = color;
	return spaceship;
}

// maybe add accelaration as a parameter
void move_ship(Spaceship* s, float turn_speed, float acceleration)
{
	// add cosine of heading angle * speed to sx
	s->sx += cosf(s->heading+PI/2) * (-s->speed);
	// add sine of heading angle * speed to sy
	s->sy += sinf(s->heading+PI/2) * (-s->speed);

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

void draw_ship(Spaceship* s)
{
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

void destroy_ship(Spaceship* s)
{
	free(s);
}
