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
class SDL_Line;
class DatapathGElementOutput {
	const char* name;
	std::set<DatapathGElementInput*> destationInputs;
	DatapathGElement* parent;
	SDL_Output pos;
	int xoffset;
	int yoffset;
	char* origin;
	std::set<SDL_Line*> routes;
private:
	int rand_number(int from, int to);
	int adjustL1(SDL_Line* l1,SDL_Line*l2,std::set<int>& hvals);
	int adjustL2(SDL_Line* l2,SDL_Line*l, std::set<int>& hvals);
	int adjustL3(SDL_Line* l3,SDL_Line*l, std::set<int>& vvals,bool lr);
	void adjustRoutes(int offset,int idx);
	bool isOverLap(SDL_Line* l1,SDL_Line*l2);
	void route(const SDL_Rect&,const SDL_Output& origin,const SDL_Rect&, const SDL_Arrow& destination,std::set<SDL_Line*>& cset);
public:
	DatapathGElementOutput(const char* n,DatapathGElement* p);
	virtual ~DatapathGElementOutput();
	void registerInput(DatapathGElementInput*);
	void levelize(int row,int col, std::map<int, std::list<DatapathGElement*> >&);
	DatapathGElementOutput(const DatapathGElementOutput&);
	DatapathGElementOutput& operator=(const DatapathGElementOutput&);
	void compute(const int x, const int y,const int xoffset_, const int yoffset_,TTF_Font *font);
	void computeConnections(std::set<SDL_Line*>& cset);
	inline const SDL_Output& getOutputPosition() const {return pos;}
	inline const char * getText() const {return name;}
	inline const DatapathGElement * getParent() const {return parent;}
};

#endif /* DATAPATHGELEMENTOUTPUT_H_ */
