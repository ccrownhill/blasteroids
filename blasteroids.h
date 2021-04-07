#include <allegro5/allegro_font.h>


#define WIDTH 1000 // 640
#define HEIGHT 720 // 480
#define FPS 60

// displaying lives
#define SPACESHIP_LIVES_X 30
#define SPACESHIP_LIVES_Y 100
// color of lives
#define SPACESHIP_LIVES_RED 255
#define SPACESHIP_LIVES_GREEN 0
#define SPACESHIP_LIVES_BLUE 0

// score
#define ASTEROID_HIT_SCORE_PLUS 100
#define SCORE_X SPACESHIP_LIVES_X
#define SCORE_Y 20
#define SCORE_SCALE 7
// score color
#define SCORE_RED 255
#define SCORE_GREEN 255
#define SCORE_BLUE 255

// some math macros
#ifndef PI
# define PI 3.1416 // for translation calc with angles
#endif
#define SQUARE(x) (x)*(x)

void* movement_handler(void* a);
void display_score(ALLEGRO_FONT* font);
void display_text_centered(char *txt, float x, float y, float scale, ALLEGRO_COLOR color);
void quit_or_again_screen(char *title);
void error(char *msg);

extern int score;
