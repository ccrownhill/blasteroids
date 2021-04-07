#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "asteroid.h"

#ifndef SPACESHIP

#define SPACESHIP_LIVES 3
#define SPACESHIP_TURNSPEED 1.5 // radians/s
#define SPACESHIP_MIN_TURNSPEED 15 // pixels/s
#define SPACESHIP_ACCELERATION 90.0 // pixels/(s^2)
#define SPACESHIP_BACK_TO_FRONT_ACCELERATION_RATIO 1/1.5
#define SPACESHIP_MAX_SPEED 400
#define SPACESHIP_RADIUS 7
#define SPACESHIP_INVINCIBILITY_TIME 3 // in seconds
#define SPACESHIP_BLINK_TIME 0.2 // number of seconds to stay with one color

typedef struct Spaceship {
	float sx;
	float sy;
	float heading;
	float speed;
	unsigned int lives:3;
	unsigned int is_invincible:1;
	unsigned int gone:1;
  unsigned int score;
	ALLEGRO_COLOR color;
} Spaceship;

Spaceship* init_ship(float init_x, float init_y, ALLEGRO_COLOR color);
void move_ship(Spaceship* s, float turn_speed, float acceleration);
void check_for_ship_asteroid_collisions(Spaceship* s, Asteroid* first_asteroid);
void draw_ship(Spaceship* s);
void display_lives(Spaceship* s);
void display_score(ALLEGRO_FONT* font, Spaceship* s);

#endif
#define SPACESHIP
