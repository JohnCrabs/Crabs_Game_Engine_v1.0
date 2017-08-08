/* This is the main part of the program.
 * This class contains all main variables and functions the game needs to be created.
 * For better usage and easy debugging the functions are categorized in sections.
 * 
 * 1) Constructor - Destructor
 * 2) InitSDL2.0
 * 3) Level
 * 4) Run
 * 5) Graphics
 */

#include "GameCore.h"

/****************************/
/*1)Constructor - Destructor*/
/****************************/

GameCore::GameCore()
{
	level_index = 0;
	
	//Initialize SDL2.0
	init(GAMENAME);
	
	//Create Levels
	level = (Level *)malloc(LEVEL_NUM*sizeof(Level));
	load_img_snd();
	initLevels();
	
	//Game while loop
	int frame_limit = SDL_GetTicks() + DELAY;
	game.quit = OFF;
	while(game.quit == OFF) {
		input_sdl();
		runGame();
		delay(frame_limit, DELAY);
		frame_limit = SDL_GetTicks() + DELAY;
	}
	
	//Clear memory
	clear_sdl();
}

GameCore::~GameCore()
{
	
	
}

/**************/
/*2)initSDL2.0*/
/**************/

//This function initialize SDL2.0 library and creates the main characteristics of the game graphics
void GameCore::init(const char *title)
{
	/*open sdl*/
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("ERROR:SDL CANNOT OPEN::%s", SDL_GetError());
		exit(ERROR);
	}
	/*create window*/
	game.screen =  SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 
									SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (game.screen == NULL) {
		printf("ERROR:CAN'T CREATING WINDOW (%d x %d)::%s", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
		exit(ERROR);
	}
	SDL_SetWindowMinimumSize(game.screen, SCREEN_WIDTH, SCREEN_HEIGHT); //set window size
	game.renderer = SDL_CreateRenderer(game.screen, -1, SDL_RENDERER_ACCELERATED); //create renderer
	game.window = SDL_GetWindowSurface(game.screen);
	game.mode = ON;
}

//Delay
void GameCore::delay(unsigned int frame_limit, unsigned int delay_num)
{
	unsigned int ticks = SDL_GetTicks();

	if (frame_limit < ticks)
		return;
	
	if (frame_limit > ticks + delay_num)
		SDL_Delay(delay_num);
	
	else
		SDL_Delay(frame_limit - ticks);
}

//This function clear the memory from any open sdl utillity
void GameCore::clear_sdl()
{
	clear_memory();
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.screen);
	SDL_FreeSurface(game.window);
	SDL_Quit();
	exit(EXIT);
}

//Take events
void GameCore::input_sdl()
{
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT: {
				game.quit = ON;
				break;
			}
			case SDL_WINDOWEVENT: {
				switch(event.window.event) {
					case SDL_WINDOWEVENT_RESIZED: {
						game.mode = ON;
						level[level_index].mode = ON;
						break;
					}
				}
				break;
			}
			case SDL_KEYDOWN: {
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE: {
						game.quit = ON;
						break;
					}
					case SDLK_UP:
					case SDLK_w: {
						input.up = 1;
						level[level_index].mode = ON;
						break;
					}
					case SDLK_DOWN:
					case SDLK_s: {
						input.down = 1;
						level[level_index].mode = ON;
						break;
					}
					case SDLK_RIGHT:
					case SDLK_d: {
						input.right = 1;
						player[0].mode = ON;
						level[level_index].mode = ON;
						break;
					}
					case SDLK_LEFT:
					case SDLK_a: {
						input.left = 1;
						player[0].mode = OFF;
						level[level_index].mode = ON;
						break;
					}
					case SDLK_SPACE: {
						input.action = 1;
						level[level_index].mode = ON;
						break;
					}
				}
				break;
			}
			case SDL_KEYUP: {
				switch (event.key.keysym.sym) {
					case SDLK_UP:
					case SDLK_w: {
						input.up = 0;
						break;
					}
					case SDLK_DOWN:
					case SDLK_s: {
						input.down = 0;
						break;
					}
					case SDLK_RIGHT:
					case SDLK_d: {
						input.right = 0;
						player[0].mode = ON;
						break;
					}
					case SDLK_LEFT:
					case SDLK_a: {
						input.left = 0;
						player[0].mode = OFF;
						break;
					}
					case SDLK_SPACE: {
						input.action = 0;
						break;
					}
				}
				break;
			}
		}
		break;
	}
}

/*********/
/*3)Level*/
/*********/

//Initialize Levels
void GameCore::initLevels()
{
	load_level(&level[0], LEVEL1);
	load_level(&level[1], LEVEL2);
	load_level(&level[2], LEVEL3);
	load_level(&level[3], LEVEL4);
	load_level(&level[4], LEVEL5);
}

//This function Loads a Level map
void GameCore::load_level(Level *map, const char *path)
{
	map->mode = ON;
	level_size(path, &map->x, &map->y);
	map->level = (char*)malloc((map->x)*(map->y)*sizeof(char));
	create_level(path, map->level);
}

//This function Draws the Level
void GameCore::draw_level(Level *map)
{
	int index, face, line_change;
	int width  = game.w/2 - SCREEN_WIDTH/2;
	int height = game.h/2 - SCREEN_HEIGHT/2;
	register int i, j;
	SDL_RenderClear(game.renderer);
	for(i = 0; i < map->y; i++)
		for(j = 0; j < map->x; j++) {
			if(isdigit(map->level[i*map->x + j]) && (index = (map->level[i*map->x + j] - '0')) > 0) {
				block[index].x = width + j*block[index].w;
				block[index].y = height + i*block[index].h;
				draw_image(block[index], block[index].x, block[index].y, game.renderer);
			}
			else if(isalpha(map->level[i*map->x + j]))
				switch (map->level[i*map->x + j]) {
					case PLAYER: {
						if(input.up == 1) {
							face = ((player[0].mode == ON) ? 1 : -1);
							if((map->level[(i-1)*map->x + j + face] - '0') < 1 && (map->level[(i-1)*map->x + j] - '0') < 1 && 
							(map->level[i*map->x + j + face] - '0') > 0) {
								   
								map->level[i*map->x + j] = '0';
								map->level[(i-1)*map->x + j + face] = 'P';
								player[0].x = width + (j+face)*player[0].w;
								player[0].y = height + (i-1)*player[0].h;
								
							}
						}
						else if(input.down == 1) {
							face = ((player[0].mode == ON) ? 1 : -1);
							if((map->level[(i+1)*map->x + j + face] - '0') < 1 && (map->level[(i+1)*map->x + j + face] - '0') < 1 && 
							(map->level[i*map->x + j + face] - '0') < 1) {
								   
								map->level[i*map->x + j] = '0';
								map->level[(i+1)*map->x + j + face] = 'P';
								player[0].x = width + (j+face)*player[0].w;
								player[0].y = height + face*(i+1)*player[0].h;
								input.down = 0;
							}
						}
						else if(input.right == 1 && (map->level[i*map->x + j + 1] - '0') < 1 && (map->level[(i+1)*map->x + j + 1] - '0') > 0) {
							map->level[i*map->x + j] = '0';
							map->level[i*map->x + j + 1] = 'P';
							player[0].x = width + (j+1)*player[0].w;
							player[0].y = height + i*player[0].h;
							//draw_image(player[0], player[0].x, player[0].y, game.renderer);
							j++;
						}
						else if(input.left == 1 && (map->level[i*map->x + j - 1] - '0') < 1 && (map->level[(i+1)*map->x + j - 1] - '0') > 0) {
							map->level[i*map->x + j] = '0';
							map->level[i*map->x + j - 1] = 'P';
							player[0].x = width + (j-1)*player[0].w;
							player[0].y = height + i*player[0].h;
							//draw_image(player[1], player[0].x, player[0].y, game.renderer);
						}
						else if(input.action == 1) {
							take_action(map, i*map->x + j, i, j, &line_change);
							i = line_change;
							j = -1;
							input.action = 0;
						}
						else {
							player[0].x = width + j*player[0].w;
							player[0].y = height + i*player[0].h;
							//draw_image((player[0].mode == ON) ? player[0] : player[1], player[0].x, player[0].y, game.renderer);
						}
						break;
						
					}
					case CARPET: {
						object[CARPET - 'a'].x = width + j*object[CARPET - 'a'].w;
						object[CARPET - 'a'].y = height + i*object[CARPET - 'a'].h;
						draw_image(object[CARPET - 'a'], object[CARPET - 'a'].x, object[CARPET - 'a'].y, game.renderer);
						break;
					}
					case DOOR: {
						object[DOOR - 'a'].x = width + j*object[DOOR - 'a'].w;
						object[DOOR - 'a'].y = height + i*object[DOOR - 'a'].h;
						draw_image(object[DOOR - 'a'], object[DOOR - 'a'].x, object[DOOR - 'a'].y, game.renderer);
						break;
					}
					case FLOWER: {
						object[FLOWER - 'a'].x = width + j*object[FLOWER - 'a'].w;
						object[FLOWER - 'a'].y = height + i*object[FLOWER - 'a'].h;
						draw_image(object[FLOWER - 'a'], object[FLOWER - 'a'].x, object[FLOWER - 'a'].y, game.renderer);
						break;
					}
					case ROOT: {
						object[ROOT - 'a'].x = width + j*object[ROOT - 'a'].w;
						object[ROOT - 'a'].y = height + i*object[ROOT - 'a'].h;
						draw_image(object[ROOT - 'a'], object[ROOT - 'a'].x, object[ROOT - 'a'].y, game.renderer);
						break;
					}
					case WATER: {
						object[WATER - 'a'].x = width + j*object[WATER - 'a'].w;
						object[WATER - 'a'].y = height + i*object[WATER - 'a'].h;
						draw_image(object[WATER - 'a'], object[WATER - 'a'].x, object[WATER - 'a'].y, game.renderer);
						break;
					}
					case LADDER: {
						object[LADDER - 'a'].x = width + j*object[LADDER - 'a'].w;
						object[LADDER - 'a'].y = height + i*object[LADDER - 'a'].h;
						draw_image(object[LADDER - 'a'], object[LADDER - 'a'].x, object[LADDER - 'a'].y, game.renderer);
						break;
					}
				}
			}
	if(object[DOOR-'a'].mode == ON) {
		draw_image((player[0].mode == ON) ? player[0] : player[1], player[0].x, player[0].y, game.renderer);
		SDL_RenderPresent(game.renderer);
		map->mode = OFF;
	}
	object[DOOR-'a'].mode = ON;
}

//Do the action
void GameCore::take_action(Level *map, int pos, int line, int row, int *change_line)
{
	register int i, j;
	int face = (player[0].mode == ON) ? 1 : -1;
	int object_type;
	switch (map->level[pos+face]){
		case WATER: {
			map->level[pos+face] = '0';
			for(i = 0; i < map->y; i++)
				for(j = 0; j < map->x; j++)
					if (map->level[i*map->x + j] == ROOT && i > 1) {
						object_type = map->level[(i-1)*map->x + j];
						map->level[(i-1)*map->x + j] = 'f';
						map->level[(i-2)*map->x + j] = object_type;
						SDL_RenderClear(game.renderer);
						*change_line = 0;
					}
			break;
		}
		case DOOR: {
			free(level[level_index].level);
			object[DOOR - 'a'].mode = OFF;
			level_index++;
			level[level_index].mode = ON;
			*change_line = map->y;
			break;
		}
		case CARPET: {
			map->level[pos+face] = '0';
			SDL_RenderClear(game.renderer);
			*change_line = 0;
			break;
		}
		case LADDER: {
			i = line;
			j = (row+face);
			if(map->level[(i-1)*map->x+j] == 'l') {
				i--;
				while(map->level[(i-1)*map->x+j] == 'l')
					i--;
			}
			else if(map->level[(i+1)*map->x+j] == 'l'){
				i++;
				while(map->level[(i+1)*map->x+j] == 'l')
					i++;
			}
			map->level[pos] = '0';
			map->level[i*map->x+j+face] = 'P';
			SDL_RenderClear(game.renderer);
			*change_line = 0;
			break;
		}
		default:{
			*change_line = line;
			break;
		}
	}
}

void GameCore::level_size(const char *path, int *x, int *y)
{
	char c;
	FILE *openfile;
	openfile = fopen(path, "r");
	if (openfile == NULL) {
		printf("ERROR:CANNOT OPEN LEVEL FILE::%s", path);
		return ;
	}
	*x = *y = 0;
	while((c = fgetc(openfile)) != EOF) {
		if(isalnum(c) && *y == 0)
			++(*x);
		if(c == '\n')
			++(*y);
	}
	fclose(openfile);
}

void GameCore::create_level(const char *path, char *map)
{
	char c;
	FILE *openfile;
	openfile = fopen(path, "r");
	if (openfile == NULL) {
		printf("ERROR:CANNOT OPEN LEVEL FILE::%s", path);
		return ;
	}
	while((c = fgetc(openfile)) != EOF)
		if(isalnum(c)){
			*map = c;
			map++;
		}
	fclose(openfile);
}

/*******/
/*4)Run*/
/*******/

//This function create the rendering textures 
void GameCore::runGame()
{
	SDL_GetWindowSize(game.screen, &game.w, &game.h); 
	SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
	SDL_RenderClear(game.renderer);
	if(game.mode == ON) {
		SDL_RenderPresent(game.renderer);
		game.mode = OFF;
	}
	
	if(logo.mode == ON) {
		logo_print(&logo);
		SDL_DestroyTexture(logo.img);
	}
	if(logo.mode == OFF) {
		if (level[level_index].mode == ON && game.mode == OFF)
			draw_level(&level[level_index]);
	}
}

//This function prints the Logo
void GameCore::logo_print(Image *im)
{
	int del = 3000;
	int frame_limit = SDL_GetTicks() + del;
	im->y = game.h/2 - im->h/2;
	im->x = game.w/2 - im->w/2;
	draw_image(*im, im->x, im->y, game.renderer);
	im->mode = OFF;
	SDL_RenderPresent(game.renderer);
	delay(frame_limit, del);
	game.mode = ON;
}

/************/
/*5)Graphics*/
/************/

Image GameCore::load_img(const char *path)
{
	Image temp;
	temp.img = IMG_LoadTexture(game.renderer, path);
	if (temp.img == NULL) {
		printf("\nERROR:CANNOT OPEN IMAGE::%s\n", path);
		exit(ERROR);
	}
	SDL_QueryTexture(temp.img, NULL, NULL, &temp.w, &temp.h);
	temp.mode = ON;
	return temp;
}

Sprite GameCore::load_sprt(const char *path)
{
	Sprite temp;
	temp.img = IMG_Load(path);
	if (temp.img == NULL) {
		printf("\nERROR:CANNOT OPEN IMAGE::%s\n", path);
		exit(ERROR);
	}
	return temp;
}

void GameCore::draw_image(Image display_image, int x, int y, SDL_Renderer *renderer)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = display_image.w;
	dest.h = display_image.h;
	SDL_RenderCopy(renderer, display_image.img, NULL, &dest);
}

void GameCore::draw_sprite(Sprite display_image, int x, int y, Game game)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = display_image.img->w;
	dest.h = display_image.img->h;
	SDL_BlitSurface(display_image.img, NULL, game.window, &dest);
	SDL_UpdateWindowSurface(game.screen);
	SDL_RenderPresent(game.renderer);
}

void GameCore::free_graphic(SDL_Surface *image)
{
	SDL_FreeSurface(image);
}

void GameCore::load_img_snd() 
{
	//backgrounds
	logo = load_img(LOGO_PATH);
	
	//blocks
	block[1] = load_img(BLOCK);
	block[2] = load_img(GRASS);
	
	//characters
	player[0] = load_img(PLAYER_R);
	player[1] = load_img(PLAYER_L);
	player[0].speed_x = PLAYER_SPEED;
	player[0].speed_y = PLAYER_SPEED;
	
	//objects
	object['c'-'a'] = load_img(CARPET_PATH);
	object['d'-'a'] = load_img(DOOR_PATH);
	object['f'-'a'] = load_img(FLOWER_PATH);
	object['r'-'a'] = load_img(ROOT_PATH);
	object['w'-'a'] = load_img(WATER_PATH);
	object['l'-'a'] = load_img(LADDER_PATH);
}

void GameCore::clear_memory()
{
	register int i;
	SDL_DestroyTexture(logo.img);
	SDL_DestroyTexture(player[1].img);
	SDL_DestroyTexture(player[2].img);
	for (i = 0; i < BLOCK_NUM; i++)
		SDL_DestroyTexture(block[i].img);
	for (i = 0; i < 'z' - 'a'; i++)
		SDL_DestroyTexture(object[i].img);
	for (int i = 0; i <= level_index; i++)
		free(level[level_index].level);
}


