/*
 * SDLRoute.h
 *
 *  Created on: Nov 11, 2014
 *      Author: songjinxin
 */

#ifndef SDLROUTE_H_
#define SDLROUTE_H_
#include <list>
#include <SDL2/SDL.h>
class SDL_Line;
class SDL_Route {

public:
//	const char* origin;
//	std::list<SDL_Line*> lines;
//	SDL_Color rcolor;
//	SDL_Route(const char* n):
//		origin(n),
//		lines(),
//		rcolor(){}
	virtual ~SDL_Route();
};

#endif /* SDLROUTE_H_ */
