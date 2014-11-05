/*
 * DatapathGElementOutput.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#include "DatapathGElementOutput.h"
#include "DatapathGElementInput.h"
#include "DatapathGElement.h"
DatapathGElementOutput::DatapathGElementOutput(const char* n,DatapathGElement* p):
name(n),
destationInputs(),
parent(p),
pos()
{
	// TODO Auto-generated constructor stub

}

DatapathGElementOutput::~DatapathGElementOutput() {
	// TODO Auto-generated destructor stub
}

void DatapathGElementOutput::registerInput(DatapathGElementInput* in){
	destationInputs.insert(in);
}

void DatapathGElementOutput::levelize(int row,int col,std::map<int, std::list<DatapathGElement*> >& linfo){
	for(std::set<DatapathGElementInput*>::const_iterator it = destationInputs.begin(); it!=destationInputs.end();++it){
		(*it)->levelize(row,col,linfo);
	}
}

void DatapathGElementOutput::compute(const int x, const int y,TTF_Font *font){
	int text_w,text_h;
	TTF_SizeText(font,name,&text_w, &text_h);
	pos.p.x = x;
	pos.p.y = y;
	pos.textp.x = x - (text_w/2);
	pos.textp.y=  y - (text_h);
}

void DatapathGElementOutput::route(SDL_LNode* origin, SDL_LNode* destination){
	origin->next = destination;
}

void DatapathGElementOutput::computeConnections(std::set<SDL_LNode*>& cset){

	if ((this->pos.p.x==0)&&(this->pos.p.y == 0)) return;
	for(std::set<DatapathGElementInput*>::const_iterator it = destationInputs.begin(); it!=destationInputs.end();++it){
		SDL_LNode* route_start =  new SDL_LNode(pos.p);
		SDL_LNode* route_end   =  new SDL_LNode((*it)->getArrowPosition().cp);
		printf("%s.%s x(%d) y(%d), %s.%s x(%d) y(%d) \n",this->getParent()->getName(),this->getText(),
				route_start->p.x, route_start->p.y,
				(*it)->getParent()->getName(),(*it)->getText(),route_end->p.x,route_end->p.y);
		cset.insert(route_start);
		route(route_start,route_end);
	}
}
