#include <allegro5/allegro_primitives.h>

#ifndef PI
#	define PI 3.1416
#endif

typedef struct Spaceship {
	float sx;
	float sy;
	float heading;
	float speed;
	int gone;
	ALLEGRO_COLOR color;
} Spaceship;

Spaceship* init_ship(float, float, ALLEGRO_COLOR);
void move_ship(Spaceship *s);
void draw_ship(Spaceship*);
void destroy_ship(Spaceship*);
