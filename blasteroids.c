#include <stdio.h>
#include <unistd.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <spaceship.h>

#define WIDTH 640
#define HEIGHT 480
#define FPS 30

int main()
{
	printf("Starting blasteroids...\n");

	Spaceship *spaceship = init_ship(WIDTH / 2, HEIGHT / 2, al_map_rgb(255, 255, 255));
	spaceship->speed = 1.0;

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT* font = NULL;

	if (!al_init()) {
		fprintf(stderr, "Failed to init allegro.\n");
		return 1;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "Failed to install keyboard.\n");
		return 1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "Failed to create timer.\n");
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

	font = al_create_builtin_font();
	if (!font) {
		fprintf(stderr, "Failed to create font.\n");
		return 1;
	}

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));


	bool redraw = true;
	ALLEGRO_EVENT event;

	al_start_timer(timer);
	while (1) {
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
		} else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch(event.keyboard.keycode) {
				case ALLEGRO_KEY_UP:
					spaceship->speed += 1.0;
					break;
				case ALLEGRO_KEY_DOWN:
					spaceship->speed -= 1.0;
					break;
				case ALLEGRO_KEY_LEFT:
					spaceship->heading -= 0.1;
					break;
				case ALLEGRO_KEY_RIGHT:
					spaceship->heading += 0.1;
					break;
			}
		} else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		move_ship(spaceship);

		if (redraw && al_is_event_queue_empty(queue))
		{
			al_clear_to_color(al_map_rgb(0, 0, 0));
			draw_ship(spaceship);
			al_flip_display();
			redraw = false;
		}
	}

	
	destroy_ship(spaceship);
	al_destroy_display(display);
	al_destroy_event_queue(queue);
	al_destroy_timer(timer);
	al_destroy_font(font);
	return 0;
}
