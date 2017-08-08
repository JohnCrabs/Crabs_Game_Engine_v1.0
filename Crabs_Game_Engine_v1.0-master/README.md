## Synopsis

My first try to create my personal Game Engine.

## Code Example
//Do the action
void GameCore::take_action(Level *map, int pos, int line, int *change_line)
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
		default:{
			*change_line = line;
			break;
		}
	}
}

## Motivation

Just a simple game engine to create simple 2D games. SoapGamesDawn_v1.0 is a simple example. 

## Installation

1)Open a terminal in the folder with the makefile.
2)Run the command make.

## Contributors

Currently only me.

## License
Copyright (C) Dec 2016 John Crabs <kavouras.ioannis@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.




