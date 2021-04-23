#ifndef BLAST_H
#define BLAST_H

#include "spaceship.h"

#define BLAST_COLOR al_map_rgb(255, 0, 0) // red
#define BLAST_LENGTH 15 // in pixels
#define BLAST_SPEED SPACESHIP_MAX_SPEED*2.25 // in pixels/s
#define BLAST_THICKNESS 2.0f

typedef struct Blast {
  float sx;
  float sy;
  float heading;
  float speed;
  unsigned int gone:1;
  ALLEGRO_COLOR color;
  struct Blast* next;
} Blast;

Blast* init_blast(float init_x, float init_y, float heading);
void shoot_blast_from_spaceship(Spaceship* s, Blast** list_start);
void move_blast(Blast* blast);
void move_blast_list(Blast* list_start);
void check_for_blast_asteroid_collisions(Blast* blast_list_start, Asteroid* asteroid_list_start);
void draw_blast(Blast* blast);
void draw_blast_list(Blast* list_start);
void clean_gone_blasts(Blast** list_start);
void destroy_blasts(Blast* list_start);

#endif
