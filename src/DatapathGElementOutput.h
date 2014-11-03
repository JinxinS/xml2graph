/*
 * DatapathGElementOutput.h
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#ifndef DATAPATHGELEMENTOUTPUT_H_
#define DATAPATHGELEMENTOUTPUT_H_
#include <set>
#include <list>
#include <map>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include "xml2graphdef.h"
class DatapathGElementInput;
class DatapathGElement;
class DatapathGElementOutput {
	const char* name;
	std::set<DatapathGElementInput*> destationInputs;
	DatapathGElement* parent;
	SDL_Output pos;
public:
	DatapathGElementOutput(const char* n,DatapathGElement* p);
	virtual ~DatapathGElementOutput();
	void registerInput(DatapathGElementInput*);
	void levelize(int lvl,std::map<int, std::list<DatapathGElement*> >&);
	DatapathGElementOutput(const DatapathGElementOutput&);
	DatapathGElementOutput& operator=(const DatapathGElementOutput&);
	void compute(const int x, const int y,TTF_Font *font);
	inline const SDL_Output& getOutputPosition() const {return pos;}
	inline const char * getText() const {return name;}
	inline const DatapathGElement * getParent() const {return parent;}
};

#endif /* DATAPATHGELEMENTOUTPUT_H_ */
