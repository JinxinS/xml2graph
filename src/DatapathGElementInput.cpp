/*
 * DatapathGElementInput.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#include "DatapathGElementInput.h"
#include "DatapathGElement.h"


DatapathGElementInput::DatapathGElementInput(const char* n,DatapathGElement* p)
:name(n),
 sourceOutput(),
 parent(p),
 pos()
{
	// TODO Auto-generated constructor stub

}

DatapathGElementInput::~DatapathGElementInput() {
	// TODO Auto-generated destructor stub
}


void DatapathGElementInput::registerOutput(DatapathGElementOutput* o){
	sourceOutput = o;
}

void DatapathGElementInput::levelize(int lvl,std::map<int, std::list<DatapathGElement*> >& linfo){
	parent->levelize(lvl,linfo);
}

void DatapathGElementInput::compute(const int x, const int y,const int w,const int h,TTF_Font *font){
	int text_w,text_h;
	TTF_SizeText(font,name,&text_w, &text_h);


	pos.p.x = x - w;
	pos.p.y = y - h;
	pos.angle = 90;
	pos.textp.x = x - (text_w/2);
	pos.textp.y=  y ;
}
