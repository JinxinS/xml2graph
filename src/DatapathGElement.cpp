/*
 * DatapathGElement.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#include "DatapathGElement.h"
#include "DatapathGElementOutput.h"
#include "DatapathGElementInput.h"

DatapathGElement::DatapathGElement(const char* n,const char* f):
	name(n),
	function(f),
	inputs(),
	outputs(),
	level(-1)
{}

DatapathGElement::~DatapathGElement() {
	//for(auto i = inputs.begin(); i != inputs.end(); ++i) delete *(i->second);
}

void DatapathGElement::addInPort(const char* i){
	inputs.insert(new DatapathGElementInput(i,this));
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

void DatapathGElement::connect(const char* i,DatapathGElement* s,const char* o){
	DatapathGElementInput* in = new DatapathGElementInput(i,this);
	s->registerDestPort(in,o);
}

void DatapathGElement::levelize(int lvl){
	level = lvl ; //current level
	printf("fu %s set to level %d \n",name,lvl);

	for(std::map<char,DatapathGElementOutput*>::const_iterator it = outputs.begin();it != outputs.end();++it){
		DatapathGElementOutput* e = it->second;
		e->levelize(lvl);
	}
}
