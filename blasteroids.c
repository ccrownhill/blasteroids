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


ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_FONT *font = NULL;
Spaceship *spaceship;
Asteroid *first_asteroid;


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

		spaceship = init_ship(WIDTH / 2, HEIGHT / 2, al_map_rgb(255, 255, 255));
		first_asteroid = create_asteroid_list(INIT_ASTEROIDS);

		pthread_t ship_movement_thread;
		if (pthread_create(&ship_movement_thread, NULL, ship_movement_handler, NULL) == -1)
			error("Failed to create user input thread");


		// MAIN loop
		while (!spaceship->gone) {
			al_wait_for_event(queue, &event);
			if (event.type == ALLEGRO_EVENT_TIMER)
				redraw = true;
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				break;

			if (redraw && al_is_event_queue_empty(queue))
			{
				al_clear_to_color(al_map_rgb(0, 0, 0));

				move_asteroid_list(first_asteroid);
				draw_asteroid_list(first_asteroid);

				draw_ship(spaceship);
				display_lives(spaceship);
				al_flip_display();
				redraw = false;
			}
		}

		al_stop_timer(timer);

		void* movement_input_event_queue;
		if (pthread_join(ship_movement_thread, &movement_input_event_queue) == -1)
			error("Failed to join thread");
		al_destroy_event_queue((ALLEGRO_EVENT_QUEUE*)movement_input_event_queue);

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

		destroy_ship(spaceship);
		destroy_asteroids(first_asteroid);
	}
	
	al_destroy_display(display);
	al_destroy_event_queue(queue);
	al_destroy_timer(timer);
	al_destroy_font(font);
	return 0;
}


// thread function for handling the movement of the ship
// but not its drawing (done in the MAIN loop
void* ship_movement_handler(void* a)
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
  while (!spaceship->gone) {
    al_wait_for_event(queue, &event);

    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      break;
		else if (event.type == ALLEGRO_EVENT_TIMER) {
      move_ship(spaceship, turn_speed, acceleration);
			check_for_collisions(spaceship, first_asteroid);
		} else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
      switch(event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
          acceleration = ACCELERATION;
          break;
        case ALLEGRO_KEY_DOWN:
          acceleration = -ACCELERATION/2; // backward acceleration smaller than forward
          break;
        case ALLEGRO_KEY_LEFT:
          turn_speed = -TURNSPEED;
          break;
        case ALLEGRO_KEY_RIGHT:
          turn_speed = TURNSPEED;
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

void display_text();

void error(char *msg)
{
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(1);
}
