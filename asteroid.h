#include <allegro5/allegro_primitives.h>

#define INIT_ASTEROIDS 5
#define ASTEROID_MIN_SPEED 3 // in pixels/s
#define ASTEROID_MAX_SPEED 10 // in pixels/s
#define ASTEROID_MIN_ROT_SPEED 0.2 // in radians/s
#define ASTEROID_MAX_ROT_SPEED 2 // in radians/s

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
} Asteroid;

Asteroid* init_asteroid(float init_x, float init_y, ALLEGRO_COLOR color);
void move_asteroid(Asteroid* a);
void draw_asteroid(Asteroid* a);
void destroy_asteroid(Asteroid* a);
