/*
 * DatapathGElement.h
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#ifndef DATAPATHGELEMENT_H_
#define DATAPATHGELEMENT_H_
#include <string>
#include <list>
#include <map>
#include <set>
#include "xml2graphdef.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
class DatapathGElementInput;
class DatapathGElementOutput;
class DatapathGElement {
	const char*  name;
	const char*  function;
	char*  text;
	std::set<DatapathGElementInput*> inputs;
	std::map<char,DatapathGElementOutput*> 	outputs;
	int level;
	SDL_Rect tx_pos;
	SDL_Rect rect;
private:
	void addInPort(const char*);
	void registerDestPort(DatapathGElementInput*i,const char*);
public:
	DatapathGElement(const DatapathGElement&);
	DatapathGElement& operator=(const DatapathGElement&);
	DatapathGElement(const char* n,const char* f);
	virtual ~DatapathGElement();
	void connect(const char* i,DatapathGElement* s,const char*o );
	void levelize(int,std::map<int, std::list<DatapathGElement*> >&);
	void compute(const int x , const int y, const int w, const int h,TTF_Font *font);
	inline const SDL_Rect& getOutlineRect() const { return rect;}
	inline const SDL_Rect& getTextPosition() const { return tx_pos;}
	inline const char* getName() const {return name;}
	inline const char* getFuncName() const{return function;}
	inline const char* getText() const{return text;}
};

#endif /* DATAPATHGELEMENT_H_ */
