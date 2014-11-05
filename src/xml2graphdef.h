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
const int SCREEN_WIDTH = 1280;
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
	SDL_Rect  textp;					//input name position
	SDL_Point p;						//arrow position
	SDL_Point cp;						//Line connection position
	double angle;						//direction of the angle
} ;


struct SDL_Output{
	SDL_Rect  textp;					//output name position
	SDL_Point p;						//output node position
};

struct SDL_LNode{
	SDL_Point p;
	SDL_LNode* next;
	SDL_LNode(SDL_Point p)
	:p(p),
	 next(NULL){}
};

struct dis_position{
	float x;
	float y;
} ;

#endif /* XML2GRAPHDEF_H_ */
