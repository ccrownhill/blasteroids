#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include "blasteroids.h"
#include "spaceship.h"
#include "asteroid.h"
#include "blast.h"


ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_FONT *font = NULL;
Spaceship *spaceship = NULL;
Asteroid *asteroid_list_start = NULL;
Blast *blast_list_start = NULL;
pthread_t movement_thread;
int score;


int main()
{
	printf("Starting blasteroids...\n");

  ALLEGRO_EVENT_QUEUE *queue = NULL;

	if (!al_init()) {
		fprintf(stderr, "Failed to init allegro.\n");
		return 1;
	}
	if (!al_install_keyboard()) {
		fprintf(stderr, "Failed to install keyboard.\n");
		return 1;
	}
	display = al_create_display(WIDTH, HEIGHT);
	if (!display) {
		fprintf(stderr, "Failed to create display.\n");
		return 1;
	}
	queue = al_create_event_queue();
	if (!queue) {
		fprintf(stderr, "Failed to create event queue.");
		return 1;
	}
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "Failed to create timer.\n");
		return 1;
	}
	font = al_create_builtin_font();
	if (!font) {
		fprintf(stderr, "Failed to create font.\n");
		return 1;
	}

	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_keyboard_event_source());


	bool redraw = true;
	ALLEGRO_EVENT event;

	al_start_timer(timer);

	while (1) {
    score = 0;

		spaceship = init_ship(WIDTH / 2, HEIGHT / 2, al_map_rgb(255, 255, 255));
		asteroid_list_start = create_asteroid_list(INIT_ASTEROIDS);
    blast_list_start = NULL;

		if (pthread_create(&movement_thread, NULL, movement_handler, NULL) == -1)
			error("Failed to create user input thread");


		// MAIN loop
		while (!spaceship->gone && asteroid_list_start != NULL) {
			al_wait_for_event(queue, &event);
			if (event.type == ALLEGRO_EVENT_TIMER)
				redraw = true;
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				break;

			if (redraw && al_is_event_queue_empty(queue))
			{
				al_clear_to_color(al_map_rgb(0, 0, 0));

				// draw asteroids
				draw_asteroid_list(asteroid_list_start);
				// draw blasts
        draw_blast_list(blast_list_start);
				// spaceship drawing
				draw_ship(spaceship);
				display_lives(spaceship);
        display_score(font);
				al_flip_display();
				redraw = false;
			}
		}

		al_stop_timer(timer);

		void* movement_input_event_queue;
		if (pthread_join(movement_thread, &movement_input_event_queue) == -1)
			error("Failed to join thread");
		al_destroy_event_queue((ALLEGRO_EVENT_QUEUE*)movement_input_event_queue);

    if (spaceship->gone)
      quit_or_again_screen("GAME OVER!");
    else if (asteroid_list_start == NULL)
      quit_or_again_screen("VICTORY!");
    else
      quit_or_again_screen("STOPPED!");

		while (1) {
			al_wait_for_event(queue, &event);
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				spaceship->gone = 1;
				break;
			} else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
				if (event.keyboard.keycode == ALLEGRO_KEY_Q) {
					spaceship->gone = 1;
					break;
				} else {
					spaceship->gone = 0;
					al_resume_timer(timer);
					break;
				}
			}
		}
		if (spaceship->gone)
			break;

    destroy_blasts(blast_list_start);
		free(spaceship);
		destroy_asteroids(asteroid_list_start);
	}
	
	al_destroy_display(display);
	al_destroy_event_queue(queue);
	al_destroy_timer(timer);
	al_destroy_font(font);
	return 0;
}


// thread function for handling the movement of the ship
// but not its drawing (done in the MAIN loop)
void* movement_handler(void* a)
{
  ALLEGRO_EVENT_QUEUE *queue = NULL;

  queue = al_create_event_queue();
	if (!queue) {
		fprintf(stderr, "Failed to create event queue.");
		pthread_exit(NULL);
	}
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_display_event_source(display));

  float turn_speed = 0.0;
  float acceleration = 0.0;
	ALLEGRO_EVENT event;
  while (!spaceship->gone && asteroid_list_start != NULL) {
    al_wait_for_event(queue, &event);

    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      break;
		else if (event.type == ALLEGRO_EVENT_TIMER) {
			// ship movement
      move_ship(spaceship, turn_speed, acceleration);
			check_for_ship_asteroid_collisions(spaceship, asteroid_list_start);

			// asteroid movement
			move_asteroid_list(asteroid_list_start);
			if (asteroid_list_start != NULL)
				clean_gone_asteroids(&asteroid_list_start);

			// blast movement
			move_blast_list(blast_list_start);
			if (blast_list_start != NULL) // don't do cleaning if list is empty
				clean_gone_blasts(&blast_list_start);
      check_for_blast_asteroid_collisions(blast_list_start, asteroid_list_start);
		} else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
      switch(event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
          acceleration = SPACESHIP_ACCELERATION;
          break;
        case ALLEGRO_KEY_DOWN:
          acceleration = -SPACESHIP_ACCELERATION * SPACESHIP_BACK_TO_FRONT_ACCELERATION_RATIO; // backward acceleration smaller than forward
          break;
        case ALLEGRO_KEY_LEFT:
          turn_speed = -SPACESHIP_TURNSPEED;
          break;
        case ALLEGRO_KEY_RIGHT:
          turn_speed = SPACESHIP_TURNSPEED;
          break;
				case ALLEGRO_KEY_SPACE:
					shoot_blast_from_spaceship(spaceship, &blast_list_start);
					break;
      }
    } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
      switch(event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
          if (acceleration > 0.0)
            acceleration = 0.0;
          break;
        case ALLEGRO_KEY_DOWN:
          if (acceleration < 0.0)
            acceleration = 0.0;
          break;
        case ALLEGRO_KEY_LEFT:
          if (turn_speed < 0.0)
            turn_speed = 0.0;
          break;
        case ALLEGRO_KEY_RIGHT:
          if (turn_speed > 0.0)
            turn_speed = 0.0;
          break;
      }
    }
  }
  return queue;
}

void display_score(ALLEGRO_FONT* font)
{
  ALLEGRO_TRANSFORM transform;
  al_identity_transform(&transform);
  al_scale_transform(&transform, SCORE_SCALE, SCORE_SCALE);
  al_translate_transform(&transform, SCORE_X - SPACESHIP_RADIUS, SCORE_Y);
  al_use_transform(&transform);
  al_draw_textf(font, al_map_rgb(SCORE_RED, SCORE_GREEN, SCORE_BLUE), 0, 0, 0, "%i", score);
}

void display_text_centered(char *txt, float x, float y, float scale, ALLEGRO_COLOR color)
{
  ALLEGRO_TRANSFORM transform;
  al_identity_transform(&transform);
  al_scale_transform(&transform, scale, scale);
  al_translate_transform(&transform, x, y);
  al_use_transform(&transform);

  int text_width = al_get_text_width(font, txt);
  int text_height = al_get_font_line_height(font);
  al_draw_text(font, color, -text_width/2, -text_height/2, 0, txt);
}

void quit_or_again_screen(char *title)
{
  al_clear_to_color(al_map_rgb(0, 0, 0));
  display_text_centered(title, WIDTH/2, HEIGHT/2, 10, al_map_rgb(255, 255, 255));
  display_text_centered("PRESS Q TO EXIT OR ANY OTHER KEY TO PLAY AGAIN", WIDTH/2, HEIGHT/2+HEIGHT/4, 2, al_map_rgb(255, 255, 255));
  display_score(font);
  al_flip_display();
}

void error(char *msg)
{
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(1);
}
