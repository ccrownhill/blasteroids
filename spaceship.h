#include <allegro5/allegro_primitives.h>

#define TURNSPEED 1.5
#define MINIMUM_TURN_SPEED 15
#define ACCELERATIONPLUS 3000.0
#define MAX_SPEED 15

typedef struct Spaceship {
	float sx;
	float sy;
	float heading;
	float speed;
	int gone;
	ALLEGRO_COLOR color;
} Spaceship;

Spaceship* init_ship(float init_x, float init_y, ALLEGRO_COLOR color);
void move_ship(Spaceship* s, float turn_speed, float acceleration);
void draw_ship(Spaceship* s);
void destroy_ship(Spaceship* s);
