//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <cmath>
#include <SDL2/SDL_ttf.h>
#include "LTexture.h"
#include "SDLInstance.h"
#include "XmlReader.h"
#include "DatapathGraphInfo.h"

char* parseArguments(int argc, const char* argv[] );
void printUsage(const char *progName);
int main( int argc,const char* args[] )
{

	const char* fname = parseArguments(argc, args);
	SDLInstance sdlInstance;
	XmlReader   xmlreader;
	//Read xml files
	DatapathGraphInfo* graphInfo = xmlreader.read(fname);
	//levelize all elements
	graphInfo->levelize();


	//Start up SDL and create window
	if( !sdlInstance.init(graphInfo->getName()) )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		/*compute position for all elements*/
		graphInfo->compute(sdlInstance.getFont());


		/*compute connection information  */
		//Load media
		if( !sdlInstance.loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			sdlInstance.draw(graphInfo);
		}
	}
	//Free resources and close SDL
	sdlInstance.close();

	return 0;
}

char* parseArguments(int argc, const char* argv[] ){

	char* fname= NULL;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			printUsage(argv[0]);
			exit (EXIT_SUCCESS);
		}
		else if (strcmp(argv[i], "-f") == 0) {
			if(argv[i + 1]!= NULL) fname = strdup(argv[i + 1]);
			else {
				printf("input xmlfile not specified! \n");
				exit (EXIT_FAILURE);
			}
			i++;
			continue;
		}
		else {
			printf("unknown argument %s\n",argv[i]);
			exit (EXIT_FAILURE);
		}
	}
	return fname;
}

void printUsage(const char *progName) {
	printf("Usage: %s -f <xmlfilename> [other options]\n"
			"Options:\n"
			"  -h                                  Show  usage.\n"
			"  -f                                  Input xmlFile. (mandatory)\n"
			,progName);
}
