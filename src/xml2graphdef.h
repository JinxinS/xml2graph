/*
 * xml2graphdef.h
 *
 *  Created on: Oct 24, 2014
 *      Author: songjinxin
 */

#ifndef XML2GRAPHDEF_H_
#define XML2GRAPHDEF_H_
#include <SDL2/SDL.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;


struct position {
	float x;
	float y;
} ;

struct size{
	float width;
	float height;
} ;

struct SDL_Arrow{
	SDL_Point p;
	double angle;
} ;

struct dis_position{
	float x;
	float y;
} ;

#endif /* XML2GRAPHDEF_H_ */
