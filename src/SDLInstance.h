/*
 * SDLInstance.h
 *
 *  Created on: Oct 24, 2014
 *      Author: songjinxin
 */

#ifndef SDLINSTANCE_H_
#define SDLINSTANCE_H_
#include <SDL2/SDL_ttf.h>
#include <string>
class LWindow;
class LTexture;
class SDL_Window;
class SDL_Renderer;
class SDL_Texture;
class DatapathGraphInfo;
class SDLInstance {
	//The window we'll be rendering to
	LWindow* gWindow;

	//The window renderer
	SDL_Renderer* gRenderer;

	//Globally used font
	TTF_Font *gFont;

	//Input used font
	TTF_Font *gInputFont;

	//Scene textures
	//LTexture* gTextTexture;
	LTexture* gArrowTexture;
public:
	SDLInstance(const SDLInstance&);
	SDLInstance& operator=(const SDLInstance&);
	SDLInstance();
	virtual ~SDLInstance();
	bool init(const char* title);
	bool loadMedia();
	SDL_Texture* loadTexture( std::string path );
	bool draw(DatapathGraphInfo* g);
	void close();
	inline TTF_Font * getFont() const{ return gFont;}
	inline TTF_Font * getInputFont() const{ return gInputFont;}
	inline LTexture * getArrowTexture() const{ return gArrowTexture;}
};

#endif /* SDLINSTANCE_H_ */
