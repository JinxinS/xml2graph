/*
 * xml2graphdef.h
 *
 *  Created on: Oct 24, 2014
 *      Author: songjinxin
 */

#ifndef XML2GRAPHDEF_H_
#define XML2GRAPHDEF_H_
#include <SDL2/SDL.h>
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;
//const float swratio = 0.5;												// space-width ratio
//const float shratio = 0.5;												// space-height ratio
const int wsratio = 2;												// space-width ratio
const int hsratio = 1;												// space-height ratio


//struct position {
//	int  x;
//	int y;
//} ;

struct size{
	int width;
	int height;
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


//struct SDL_Line{
//	SDL_Point start;
//	SDL_Point end;
//	int idx;
//	SDL_Line* prev;
//	SDL_Line* next;
//	SDL_Line(SDL_Point p1,SDL_Point p2,int i,SDL_Line* prv):
//	start(p1),
//	end(p2),
//	idx(i),
//	prev(prv),
//	next(){}
//};
//
//struct SDL_Route{
//	const char* origin;
//	std::list<SDL_Line*> lines;
//	SDL_Color rcolor;
//	SDL_Route(const char* n):
//		origin(n),
//		lines(),
//		rcolor(){}
//};

struct dis_position{
	float x;
	float y;
} ;

#endif /* XML2GRAPHDEF_H_ */
