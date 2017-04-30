#include "includes.h"

//Game
typedef struct Game {
	SDL_Window *screen;
	SDL_Renderer *renderer;
	SDL_Surface *window;
	int w;
	int h;
	int quit;
	int mode;
}Game;

//Level
typedef struct Level {
	char *level;
	int mode;
	int x;
	int y;
	
	void level_size(char *path, int *x, int *y);
	void create_level(char *path, char *map);
}Level;

//Image
typedef struct Image {
	SDL_Texture *img;
	int mode;
	int x;
	int y;
	int w;
	int h;
	int speed_x;
	int speed_y;
}Image;

//Sprite
typedef struct Sprite {
	SDL_Surface *img;
	int mode;
	int x;
	int y;
	int speed_x;
	int speed_y;
}Sprite;

//Controls
typedef struct Controls {
	int up;
	int down;
	int left;
	int right;
	int action;
}Controls;




