/*
 * SDLInstance.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: songjinxin
 */

#include "SDLInstance.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <cmath>
#include "LTexture.h"
#include "xml2graphdef.h"
#include "DatapathGraphInfo.h"
SDLInstance::SDLInstance()
:gWindow(),
 gRenderer(),
 gFont(),
 //gTextTexture(),
 gArrowTexture()
{
	// TODO Auto-generated constructor stub

}

SDLInstance::~SDLInstance() {
	// TODO Auto-generated destructor stub
}

bool SDLInstance::init(const char* title) {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}

				//Open the font
				gFont = TTF_OpenFont( "/usr/share/fonts/truetype/droid/DroidSansMono.ttf", 12 );
				if( gFont == NULL )
				{
					printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	//gTextTexture 	= new LTexture(gRenderer,gFont);
	gArrowTexture   = new LTexture(gRenderer,gFont);

	return success;
}

bool SDLInstance::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load Arrow' texture
	if( !gArrowTexture->loadFromFile( "/coreworks/sideconf/xml2graph/res/arrow.png" ) )
	{
		printf( "Failed to load Foo' texture image!\n" );
		success = false;
	}

	//Nothing to load
	return success;
}

SDL_Texture* SDLInstance::loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

bool SDLInstance::draw(DatapathGraphInfo* g){
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//Set text color as black
	SDL_Color textColor = { 0, 0, 0, 0xFF };

	//Get number of elements
	int n_rects = g->getNumberOfElements();

	//Create Shapes
	SDL_Rect* outlineRects = new SDL_Rect[n_rects];
	g->getOutlineRects(outlineRects);

	//Create Component Name Text
	LTexture* gTextTexture[n_rects];
	SDL_Rect gTextPosition[n_rects];
	for(int i = 0 ; i< n_rects; ++i) gTextTexture[i]  =  new  LTexture(gRenderer,gFont);
	g->getTextTexture(gTextTexture,gTextPosition,textColor);

	//Get number of Arrows and Create arrowTexture
	int n_arrows = g->getNumberOfInputs();
	SDL_Arrow* arrowRects  = new SDL_Arrow[n_arrows];
	g->getArrowTexture(arrowRects);


	//While application is running

	while( !quit )
	{
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer );

		//Render Arrow
		for(int i = 0 ; i < n_arrows; ++i)
				gArrowTexture->render( arrowRects[i].p.x, arrowRects[i].p.y, NULL,arrowRects[i].angle );

		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
		SDL_RenderDrawRects( gRenderer, outlineRects,n_rects );

		//Render text textures
		for(int i = 0 ; i< n_rects; ++i)
			gTextTexture[i]->render(gTextPosition[i].x, gTextPosition[i].y);

		//Update screen
		SDL_RenderPresent( gRenderer );
	}

	//free up
	for(int i = 0 ; i< n_rects; ++i) delete gTextTexture[i];
    delete []outlineRects;
    delete []arrowRects;

    return quit;
}


void SDLInstance::close()
{
	//Free loaded images
	//delete gTextTexture;
	delete gArrowTexture;
	//gTextTexture  = NULL;
	gArrowTexture = NULL;
	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


/*usage example*/

//Render red filled quad
//SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
//SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
//SDL_RenderFillRect( gRenderer, &fillRect );
////Draw blue horizontal line
//SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
//SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 );
//
////Draw vertical line of yellow dots
//SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
//for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
//{
//	SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
//}
////Render green outlined quad
//SDL_Rect outlineRect = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 30, SCREEN_HEIGHT/ 30 };
//SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
