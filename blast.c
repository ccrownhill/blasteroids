#include <stdio.h> // debugging
#include <math.h>
#include <pthread.h>
#include <allegro5/allegro_primitives.h>

#include "blasteroids.h"
#include "blast.h"

Blast* init_blast(float init_x, float init_y, float heading)
{
  Blast* blast = (Blast*)malloc(sizeof(Blast));
  blast->sx = init_x;
  blast->sy = init_y;
  blast->heading = heading;
  blast->speed = BLAST_SPEED;
  blast->gone = 0;
  blast->color = BLAST_COLOR;
  blast->next = NULL;
  return blast;
}

// insert new blast at beginning of list
void shoot_blast_from_spaceship(Spaceship* s, Blast** list_start)
{
	if (!s->is_invincible) { // spaceship can only shoot if it is not invincible
		// spawn blast at peak of spaceship
		float x_offset = cosf(s->heading)*SPACESHIP_RADIUS;
		float y_offset = sinf(s->heading)*SPACESHIP_RADIUS;
		Blast* new_blast = init_blast(s->sx + x_offset, s->sy + y_offset, s->heading);
		new_blast->next = *list_start;
		*list_start = new_blast;
	}
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
	if (!blast->gone) {
		// coordinates of other end of line
		float x_endpoint_diff = (-cosf(blast->heading))*BLAST_LENGTH; // use negative angle to account for reversed coordinate system
		float y_endpoint_diff = (-sinf(blast->heading))*BLAST_LENGTH;
		ALLEGRO_TRANSFORM transform;
		al_identity_transform(&transform);
		al_translate_transform(&transform, blast->sx, blast->sy);
		al_use_transform(&transform);
		al_draw_line(0, 0, x_endpoint_diff, y_endpoint_diff, blast->color, BLAST_THICKNESS);
	}
}

void draw_blast_list(Blast* list_start)
{
  Blast* i = list_start;
  for (; i != NULL; i = i->next) {
		if (!i->gone)
			draw_blast(i);
  }
}

void clean_gone_blasts(Blast** list_start)
{
	Blast* prev = NULL;
	Blast* current = *list_start;
	Blast* next = NULL;
	for (; current != NULL; current = next) {
		next = current->next;
		if (current->gone) {
			if (prev != NULL)
				prev->next = next;
			else // if it is first element in the list
				*list_start = next;
			free(current);
		} else { // if the current element is removed prev should stay the same
			prev = current;
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
