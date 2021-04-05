#include <allegro5/allegro_primitives.h>

#ifndef PI
#	define PI 3.1416
#endif
#define TURNSPEED 0.05
#define ACCELERATIONPLUS 0.1

typedef struct Spaceship {
	float sx;
	float sy;
	float heading;
	float speed;
	int gone;
	ALLEGRO_COLOR color;
} Spaceship;

Spaceship* init_ship(float, float, ALLEGRO_COLOR);
void move_ship(Spaceship *s, float turn_speed, float acceleration);
void draw_ship(Spaceship*);
void destroy_ship(Spaceship*);
