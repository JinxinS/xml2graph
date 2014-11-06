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
class LTexture;
class DatapathGElementInput;
class DatapathGElementOutput;
class DatapathGElement {
	const char*  name;
	const char*  function;
	char*  text;
	std::set<DatapathGElementInput*> inputs;
	std::map<char,DatapathGElementOutput*> 	outputs;
	int rowidx;
	int colidx;
	SDL_Rect tx_pos;
	SDL_Rect rect;
	friend class DatapathGElementOutput;
private:
	void addInPort(DatapathGElementInput* i);
public:

	DatapathGElement(const DatapathGElement&);
	DatapathGElement& operator=(const DatapathGElement&);
	DatapathGElement(const char* n,const char* f);
	virtual ~DatapathGElement();
	DatapathGElementOutput* getElementOutput(const char*);
	void connect(DatapathGElementInput* i,DatapathGElementOutput* o);
	void levelize(int, int , std::map<int, std::list<DatapathGElement*> >&);
	void compute(const int x, const int y,const int w,const int h,TTF_Font *font,TTF_Font *gInFont,LTexture * arrowTexture);
	inline const SDL_Rect& getOutlineRect() const { return rect;}
	inline const SDL_Rect& getTextPosition() const { return tx_pos;}
	inline const char* getName() const {return name;}
	inline const char* getFuncName() const{return function;}
	inline const char* getText() const{return text;}
};

#endif /* DATAPATHGELEMENT_H_ */
