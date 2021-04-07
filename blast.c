#include <stdio.h> // debugging
#include <math.h>
#include <allegro5/allegro_primitives.h>

#include "blasteroids.h"
#include "blast.h"

Blast* init_blast(float init_x, float init_y, float heading)
{
  Blast* blast = malloc(sizeof(Blast));
  blast->sx = init_x;
  blast->sy = init_y;
  blast->heading = heading;
  blast->speed = BLAST_SPEED;
  blast->gone = 0;
  blast->color = BLAST_COLOR;
  blast->next = NULL;
  return blast;
}

void move_blast(Blast* blast)
{
  blast->sx += cosf(blast->heading) * (-blast->speed) * (1.0/FPS);
  blast->sy += sinf(blast->heading) * (-blast->speed) * (1.0/FPS);

  // destroy the blast if it is off edge
  if (blast->sx < 0.0 || blast->sx > WIDTH ||
      blast->sy < 0.0 || blast->sy > HEIGHT) {
    blast->gone = 1;
  }
}

void move_blast_list(Blast* list_start)
{
  Blast* i = list_start;
  for (; i != NULL; i = i->next) {
    move_blast(i);
  }
}

void draw_blast(Blast* blast)
{
  // coordinates of other end of line
  float x_endpoint_diff = cosf(blast->heading)*BLAST_LENGTH;
  float y_endpoint_diff = sinf(blast->heading)*BLAST_LENGTH;
  ALLEGRO_TRANSFORM transform;
  al_identity_transform(&transform);
  al_translate_transform(&transform, blast->sx, blast->sy);
  al_use_transform(&transform);
  al_draw_line(0, 0, x_endpoint_diff, y_endpoint_diff, blast->color, BLAST_THICKNESS);
}

void draw_blast_list(Blast* list_start)
{
  Blast* i = list_start;
  for (; i != NULL; i = i->next) {
    draw_blast(i);
  }
}

void clean_gone_blasts(Blast* list_start)
{
  Blast* prev;
  Blast* i;
  Blast* next;
  if (list_start != NULL) {
    prev = list_start;
    i = list_start->next;
    next = NULL;
    if (list_start->gone)
      free(list_start);
    for (; i != NULL; i = next) {
      next = i->next;
      if (i->gone) {
        prev->next = next;
        free(i);
      }
      prev = i;
    }
  }
}

void destroy_blasts(Blast* list_start)
{
  Blast* i = list_start;
  Blast* next = NULL;
  for (; i != NULL; i = next) {
    next = i->next;
    free(i);
  }
}
