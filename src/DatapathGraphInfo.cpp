/*
 * DatapathGraphInfo.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#include "DatapathGraphInfo.h"
#include "DatapathGElement.h"
#include "DatapathGElementInput.h"
#include <string.h>
DatapathGraphInfo::DatapathGraphInfo(const std::string& n)
:name(n),
 graphelements()
{
	// TODO Auto-generated constructor stub

}

DatapathGraphInfo::~DatapathGraphInfo() {
	// TODO Auto-generated destructor stub
}

void DatapathGraphInfo::addElement(const char* fname, DatapathGElement* element){
	graphelements.insert(std::make_pair(fname,element));
}

void DatapathGraphInfo::addPortToElement(const char* fname, const char* iname,const char* oname){

	if(strstr(iname,"dly")||(!strcmp(iname,"value"))) return;			//parameter ignored

	DatapathGElement* gElement = graphelements.at(fname);
	const char* sfname =  strtok (const_cast<char*>(oname),".");
	const char* soname =  strtok (NULL,".");
	if(soname!=NULL){
		DatapathGElement* gSourceElement = graphelements.at(sfname);
		gElement->connect(iname,gSourceElement,soname);
	}else{
		 /*input connect to undefiend or constant*/
	}
}

void DatapathGraphInfo::levelize(){
	DatapathGElement* ctrl = graphelements.at("siw_ctrl");
	ctrl->levelize(0);
}
