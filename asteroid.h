#include <allegro5/allegro_primitives.h>

// used for initializing properties randomly
#define RANDOM_BETWEEN_ZERO_AND_ONE ((random()%1001)/1000.0)

#define INIT_ASTEROIDS 5
#define ASTEROID_INIT_SCALE 2.0
#define MAX_SPLITS 2
#define ASTEROID_MIN_SPEED 50 // in pixels/s
#define ASTEROID_MAX_SPEED 300 // in pixels/s
#define ASTEROID_MIN_ROT_SPEED 0.7 // in radians/s
#define ASTEROID_MAX_ROT_SPEED 6 // in radians/s
#define ASTEROID_RADIUS 25

// splitting
#define SPLIT_ASTEROID_MAX_ANGLE_DIFF 1.5 // ~90 degrees
#define SPLIT_ASTEROID_MIN_ANGLE_DIFF 0.2 // in radians

// asteroid color
#define ASTEROID_RED 155
#define ASTEROID_GREEN 155
#define ASTEROID_BLUE 155

#ifndef ASTEROID_DEFINED
typedef struct Asteroid {
	float sx;
	float sy;
	float heading; // angle of movement trajectory
	float twist; // current rotation angle in radians
	float speed;
	float rot_velocity;
	float scale;
	unsigned int gone:1; // has it been destroyed
	ALLEGRO_COLOR color;
	struct Asteroid* next;
} Asteroid;
#define ASTEROID_DEFINED
#endif

Asteroid* init_asteroid(float init_x, float init_y, float heading, float scale);
Asteroid* create_asteroid_list(unsigned int num_asteroids);
void split_asteroid(Asteroid** a);
void move_asteroid(Asteroid* a);
void move_asteroid_list(Asteroid* list_start);
void draw_asteroid(Asteroid* a);
void draw_asteroid_list(Asteroid* list_start);
void clean_gone_asteroids(Asteroid** list_start);
void destroy_asteroids(Asteroid* list_start);
