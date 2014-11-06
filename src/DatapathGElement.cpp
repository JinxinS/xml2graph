/*
 * DatapathGElement.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#include "DatapathGElement.h"
#include "DatapathGElementOutput.h"
#include "DatapathGElementInput.h"
#include "LTexture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

DatapathGElement::DatapathGElement(const char* n,const char* f):
	name(n),
	function(f),
	text(),
	inputs(),
	outputs(),
	rowidx(-1),
	colidx(-1),
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

DatapathGElementOutput* DatapathGElement::getElementOutput(const char* o){
	DatapathGElementOutput* out;
	char outname= *o;
	if(outputs.find(outname)==outputs.end()) {
		out = new DatapathGElementOutput(o,this);
		outputs.insert(std::make_pair(outname,out));
	}else{
		out= outputs.at(outname);
	}
	return out;
}

void DatapathGElement::connect(DatapathGElementInput* i,DatapathGElementOutput* o){
	this->addInPort(i);
	o->registerInput(i);
	i->registerOutput(o);
}

void DatapathGElement::levelize(int row,int col, std::map<int, std::list<DatapathGElement*> >& linfo){
	if(rowidx != -1) return;

	rowidx = row++; //current level
	if(linfo.find(rowidx) == linfo.end()){
		std::list<DatapathGElement*>* llist = new std::list<DatapathGElement*>();
		colidx = llist->size();
		llist->push_back(this);
		linfo.insert(std::make_pair(rowidx,*llist));
	}else{
		std::list<DatapathGElement*>& llist = linfo.at(rowidx);
		colidx = llist.size();
		llist.push_back(this);
	}
//	printf("fu %s set to row %d col %d \n",name,rowidx, colidx);

	for(std::map<char,DatapathGElementOutput*>::const_iterator it = outputs.begin();it != outputs.end();++it){
		DatapathGElementOutput* e = it->second;
		e->levelize(row,col,linfo);
	}
}

void DatapathGElement::compute(const int x, const int y,const int w,const int h,TTF_Font *font,TTF_Font *gInFont,LTexture * arrowTexture){
	int text_w, text_h;
	int arrow_w, arrow_h;
	rect.x = x;
	rect.y = y;
	rect.h = h;
	rect.w = w;

	TTF_SizeText(font,text,&text_w, &text_h);
	tx_pos.x = x + ((w - text_w)/2) ;
	tx_pos.y = y + ((h - text_h)/2) ;
	tx_pos.h = text_h;
	tx_pos.w = text_w;

	arrow_w = arrowTexture->getWidth()/2;
	arrow_h = arrowTexture->getHeight()/1.5;
	//printf("%d %d arrow_w %d arrow_h %d \n",arrowTexture->getWidth(),arrowTexture->getHeight(),arrow_w,arrow_h);
	int delta = w / (inputs.size() + 1);
	int deltatotal = delta ;
	for(auto in = inputs.begin(); in != inputs.end(); ++in){
		(*in)->compute(x + deltatotal, y, arrow_w, arrow_h, gInFont );
		deltatotal+=delta;
	}


	delta = w / (outputs.size() + 1);
	deltatotal = delta ;
	for(auto out = outputs.begin(); out != outputs.end(); ++out){
		(out->second)->compute(x + deltatotal, y + h , gInFont);
		deltatotal+=delta;
	}

}


