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

// displaying score
#define SCORE_X SPACESHIP_LIVES_X
#define SCORE_Y 20
#define SCORE_SCALE 7
// score color
#define SCORE_RED 255
#define SCORE_GREEN 255
#define SCORE_BLUE 255

#ifndef PI
# define PI 3.1416 // for translation calc with angles
#endif

void* ship_movement_handler(void* a);
void display_text(char *txt, float x, float y, float scale, ALLEGRO_COLOR color);
void game_over_screen();
void error(char *msg);
