#define WIDTH 1000 // 640
#define HEIGHT 720 // 480
#define FPS 60

// displaying lives
#define LIVES_X 30
#define LIVES_Y 60
// color of lives
#define LIVES_RED 255
#define LIVES_GREEN 0
#define LIVES_BLUE 0

#ifndef PI
# define PI 3.1416 // for translation calc with angles
#endif

void error(char *msg);
void* ship_movement_handler(void* a);
