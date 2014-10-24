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

SDLInstance::SDLInstance()
:gWindow(),
 gRenderer(),
 gFont(),
 gTextTexture(),
 gArrowTexture()
{
	// TODO Auto-generated constructor stub

}

SDLInstance::~SDLInstance() {
	// TODO Auto-generated destructor stub
}

bool SDLInstance::init() {
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
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

	gTextTexture 	= new LTexture(gRenderer,gFont);
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

bool SDLInstance::draw(){
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//Set text color as black
	SDL_Color textColor = { 0, 0, 0, 0xFF };

	//The current input text.
	std::string inputText = "SideWorks Functional Unit";
	gTextTexture->loadFromRenderedText( inputText.c_str(), textColor );

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

		//Render red filled quad
		//				SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
		//				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
		//				SDL_RenderFillRect( gRenderer, &fillRect );
		gArrowTexture->render( 512, 512, NULL,90 );
		gArrowTexture->render( 500, 512, NULL,180);
		gArrowTexture->render( 480, 512, NULL,270);
		gArrowTexture->render( 460, 512, NULL,360);

		//Render green outlined quad
		SDL_Rect outlineRect = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 30, SCREEN_HEIGHT/ 30 };
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
		SDL_RenderDrawRect( gRenderer, &outlineRect );

		outlineRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, SCREEN_WIDTH*6 / 30, SCREEN_HEIGHT/ 30 };
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
		SDL_RenderDrawRect( gRenderer, &outlineRect );
		gArrowTexture->render( 0, 512 );

		//				//Draw blue horizontal line
		//				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
		//				SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 );
		//
		//				//Draw vertical line of yellow dots
		//				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
		//				for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
		//				{
		//					SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
		//				}

		//Render text textures
		gTextTexture->render(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2);

		//Update screen
		SDL_RenderPresent( gRenderer );
	}
	return quit;
}


void SDLInstance::close()
{
	//Free loaded images
	delete gTextTexture;
	delete gArrowTexture;
	gTextTexture  = NULL;
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
