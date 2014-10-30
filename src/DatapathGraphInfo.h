/*
 * DatapathGraphInfo.h
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#ifndef DATAPATHGRAPHINFO_H_
#define DATAPATHGRAPHINFO_H_
#include <string>
#include <list>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "xml2graphdef.h"
class LTexture;
class DatapathGElement;
class DatapathGraphInfo {
	std::string name;
	size element_size;
	dis_position disp;
	std::map<std::string, DatapathGElement*> graphelements;
	std::map<int, std::list<DatapathGElement*> > levelinfo;
private:

public:
	DatapathGraphInfo(const std::string&);
	virtual ~DatapathGraphInfo();
	void addElement(const char* fname,DatapathGElement* element);
	void addPortToElement(const char* fname, const char* iname,const char* oname);
	void levelize();
	void compute(TTF_Font *gFont);
	void printlevel();
    int getOutlineRects(SDL_Rect**);
    int getTextTexture(LTexture* gTextTexture[],SDL_Rect* gTextPosition,const SDL_Color& color);
    inline int getNumberOfElements() const { return graphelements.size(); }
	inline const char* getName(){	return name.c_str();}

};

#endif /* DATAPATHGRAPHINFO_H_ */
