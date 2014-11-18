/*
 * SDLLine.h
 *
 *  Created on: Nov 11, 2014
 *      Author: songjinxin
 */

#ifndef SDLLINE_H_
#define SDLLINE_H_
#include <SDL2/SDL.h>
class SDL_Route;
class DatapathGElementOutput;
class SDL_Line {


public:
	SDL_Point start;
	SDL_Point end;
	int idx;
	SDL_Color color;
	SDL_Line* prev;
	SDL_Line* next;
	DatapathGElementOutput* origin;

	SDL_Line():
		start(),
		end(),
		idx(),
		color(),
		prev(),
		next(),
		origin(){}

	SDL_Line(SDL_Point p1,SDL_Point p2,int i,SDL_Color c,SDL_Line* prv,DatapathGElementOutput* o):
		start(p1),
		end(p2),
		idx(i),
		color(c),
		prev(prv),
		next(),
		origin(o){}
	virtual ~SDL_Line();
	SDL_Line(const SDL_Line&);
	SDL_Line& operator=(const SDL_Line&);
};

#endif /* SDLLINE_H_ */
