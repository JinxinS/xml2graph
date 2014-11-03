/*
 * DatapathGElementOutput.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#include "DatapathGElementOutput.h"
#include "DatapathGElementInput.h"

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

void DatapathGElementOutput::levelize(int lvl,std::map<int, std::list<DatapathGElement*> >& linfo){
	for(std::set<DatapathGElementInput*>::const_iterator it = destationInputs.begin(); it!=destationInputs.end();++it){
		(*it)->levelize(lvl,linfo);
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
