#include <spaceship.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

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
void move_ship(Spaceship* s)
{
	// add cosine of heading angle * speed to sx
	s->sx += cosf(s->heading+PI/2) * (-s->speed);
	// add sine of heading angle * speed to sy
	s->sy += sinf(s->heading+PI/2) * (-s->speed);
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
