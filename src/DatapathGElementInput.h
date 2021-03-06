/*
 * DatapathGElementInput.h
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#ifndef DATAPATHGELEMENTINPUT_H_
#define DATAPATHGELEMENTINPUT_H_
#include <string>
#include <map>
#include <list>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include "xml2graphdef.h"
class DatapathGElement;
class DatapathGElementOutput;
class DatapathGElementInput {
	const char* name;
	DatapathGElementOutput * sourceOutput;
	DatapathGElement* parent;
	SDL_Arrow pos;
public:
	DatapathGElementInput(const char* n, DatapathGElement* p);
	virtual ~DatapathGElementInput();
	void registerOutput(DatapathGElementOutput*);
	void levelize(int row,int col,std::map<int, std::list<DatapathGElement*> >& linfo);
	DatapathGElementInput(const DatapathGElementInput&);
	DatapathGElementInput& operator=(const DatapathGElementInput&);
	void compute(const int x, const int y,const int w,const int h,TTF_Font *font);
	inline const SDL_Arrow& getArrowPosition() const {return pos;}
	inline const char * getText() const {return name;}
	inline const DatapathGElement* getParent() const {return parent; }
};

#endif /* DATAPATHGELEMENTINPUT_H_ */
