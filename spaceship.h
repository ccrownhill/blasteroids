#include <allegro5/allegro_primitives.h>
#include "asteroid.h"

#define LIVES 3
#define TURNSPEED 1.5 // radians/s
#define MINIMUM_TURN_SPEED 15 // pixels/s
#define ACCELERATION 90.0 // pixels/(s^2)
#define MAX_SPEED 450
#define SPACESHIP_RADIUS 5
#define INVINCIBILITY_TIME 3 // in seconds
#define BLINK_TIME 0.2 // number of seconds to stay with one color

typedef struct Spaceship {
	float sx;
	float sy;
	float heading;
	float speed;
	unsigned int lives:3;
	unsigned int is_invincible:1;
	unsigned int gone:1;
	ALLEGRO_COLOR color;
} Spaceship;

Spaceship* init_ship(float init_x, float init_y, ALLEGRO_COLOR color);
void move_ship(Spaceship* s, float turn_speed, float acceleration);
void check_for_collisions(Spaceship* s, Asteroid* first_asteroid);
void draw_ship(Spaceship* s);
void display_lives(Spaceship* s);
void destroy_ship(Spaceship* s);
