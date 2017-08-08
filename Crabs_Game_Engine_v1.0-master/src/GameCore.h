#include "structs.h"

class GameCore
{
private:
	/***************/
	/***Variables***/
	/***************/
	
	//Game
	Game game;
	//Level
	Level *level;
	int level_index;
	//Controls
	Controls input;
	//Image
	Image logo;
	Image block[BLOCK_NUM];
	Image player[2];
	Image object['z'-'a'];
	
	/***************/
	/***Functions***/
	/***************/
	
	//initSDL2.0
	void init(const char *title);
	void delay(unsigned int frame_limit, unsigned int delay_num);
	void clear_sdl();
	void input_sdl();
	
	//Level
	void initLevels();
	void load_level(Level *map, const char *path);
	void draw_level(Level *map);
	void take_action(Level *map, int pos, int line, int row, int *change_pos);
	void level_size(const char *path, int *x, int *y);
	void create_level(const char *path, char *map);
	
	//Run
	void runGame();
	void logo_print(Image *im);
	
	//Graphics
	Image load_img(const char *path);
	Sprite load_sprt(const char *path);
	void draw_image(Image display_image, int x, int y, SDL_Renderer *renderer);
	void draw_sprite(Sprite display_image, int x, int y, Game game);
	void free_graphic(SDL_Surface *image);
	void load_img_snd(void);
	void clear_memory(void);
	
public:
	GameCore();
	~GameCore();
};