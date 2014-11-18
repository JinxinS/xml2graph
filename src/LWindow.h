/*
 * LWindow.h
 *
 *  Created on: Nov 18, 2014
 *      Author: songjinxin
 */

#ifndef LWINDOW_H_
#define LWINDOW_H_
#include "SDL2/SDL.h"
class LWindow {
private:
	//Window data
	SDL_Window* mWindow;
	SDL_Renderer* gRenderer;
	//Window dimensions
	int mWidth;
	int mHeight;

	//Window focus
//	bool mMouseFocus;
//	bool mKeyboardFocus;
//	bool mFullScreen;
//	bool mMinimized;

public:
	LWindow(const LWindow&);
	LWindow& operator=(const LWindow&);
	LWindow();
	virtual ~LWindow();
	bool init(const char* title);
	SDL_Renderer* createRenderer();
	void handleEvent( SDL_Event& e );
	void free();
	inline int getWidth()const{ return mWidth;}
	inline int getHeight()const { return mHeight;}
};

#endif /* LWINDOW_H_ */
