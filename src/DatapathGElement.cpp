/*
 * DatapathGElement.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#include "DatapathGElement.h"
#include "DatapathGElementOutput.h"
#include "DatapathGElementInput.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

DatapathGElement::DatapathGElement(const char* n,const char* f):
	name(n),
	function(f),
	text(),
	inputs(),
	outputs(),
	level(-1),
	tx_pos({0,0,0,0}),
	rect({0,0,0,0})
{
	text = (char*)malloc(strlen(name)+strlen(function) +3);
	strcpy(text,name);
	strcat(text,"[");
	strcat(text,function);
	strcat(text,"]");
}

DatapathGElement::~DatapathGElement() {
	free(text);
	//for(auto i = inputs.begin(); i != inputs.end(); ++i) delete *(i->second);
}

void DatapathGElement::addInPort(DatapathGElementInput* i){
	inputs.insert(i);
}

void DatapathGElement::registerDestPort(DatapathGElementInput* in,const char* o){
	DatapathGElementOutput* out;
	char outname= *o;
	if(outputs.find(outname)==outputs.end()) {
		out = new DatapathGElementOutput(o,this);
		outputs.insert(std::make_pair(outname,out));
	}else{
		out= outputs.at(outname);
	}
	out->registerInput(in);
	in->registerOutput(out);
}

void DatapathGElement::connect(DatapathGElementInput* i,DatapathGElement* s,const char* o){
	this->addInPort(i);
	s->registerDestPort(i,o);
}

void DatapathGElement::levelize(int lvl,std::map<int, std::list<DatapathGElement*> >& linfo){
	if(level != -1) return;

	level = lvl++; //current level
	if(linfo.find(level) == linfo.end()){
		std::list<DatapathGElement*>* llist = new std::list<DatapathGElement*>();
		llist->push_back(this);
		linfo.insert(std::make_pair(level,*llist));
	}else{
		std::list<DatapathGElement*>& llist = linfo.at(level);
		llist.push_back(this);
	}
//	printf("fu %s set to level %d \n",name,level);
	for(std::map<char,DatapathGElementOutput*>::const_iterator it = outputs.begin();it != outputs.end();++it){
		DatapathGElementOutput* e = it->second;
		e->levelize(lvl,linfo);
	}
}

int DatapathGElement::compute(const int x, const int y,const int w,const int h,TTF_Font *font){
	int text_w, text_h;

	rect.x = x;
	rect.y = y;
	rect.h = h;
	rect.w = w;

	TTF_SizeText(font,text,&text_w, &text_h);
	tx_pos.x = x + ((w - text_w)/2) ;
	tx_pos.y = y + ((h - text_h)/2) ;
	tx_pos.h = text_h;
	tx_pos.w = text_w;

	int delta = w / (inputs.size() + 1);
	int i = 0;
	for(auto in = inputs.begin(); in != inputs.end(); ++in,++i){
		(*in)->compute(x + delta*(i+1), y, w, h);
	}
	return i ;
}

int  DatapathGElement::getArrowPositions(SDL_Arrow* arrow ,int idx){
	int i = 0;
	for(auto in = inputs.begin(); in != inputs.end(); ++in,++i){
		arrow[idx + i] = (*in)->getArrowPosition();
		//printf("arrow[%d] x %d y %d \n",arrow[idx+i],arrow[idx + i].p.x,arrow[idx + i].p.y);
	}
	return i;
}

