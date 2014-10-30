/*
 * DatapathGraphInfo.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#include "DatapathGraphInfo.h"
#include "DatapathGElement.h"
#include "DatapathGElementInput.h"
#include "LTexture.h"
#include <string.h>
DatapathGraphInfo::DatapathGraphInfo(const std::string& n)
:name(n),
 element_size(),
 disp(),
 graphelements(),
 levelinfo()
{
	// TODO Auto-generated constructor stub

}

DatapathGraphInfo::~DatapathGraphInfo() {
	//	for(std::map<int, std::list<DatapathGElement*> >::const_iterator it = levelinfo.begin();it!=levelinfo.end();++it){
	//		delete &(it->second);
	//	}
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
	ctrl->levelize(0,levelinfo);
}

void DatapathGraphInfo::compute(TTF_Font *gFont){
	float max_n = 0;													//max number of element per row
	float max_l = levelinfo.size();									//max number of rows
	float swratio = 0.5;												// space-width ratio
	float shratio = 0.5;												// space-height ratio
	for(auto it = levelinfo.begin();it!=levelinfo.end();++it){
		if ( max_n < (float)(it->second).size())	max_n = (float)(it->second).size();
	}


	float width  = SCREEN_WIDTH  /(max_n + (max_n+1)*swratio);
	float height = SCREEN_HEIGHT /(max_l + (max_l+1)*shratio);
	element_size = {width,height};
	disp = { width*swratio,height*shratio };
	position p = {disp.x,disp.y};
	for(auto it = levelinfo.begin();it!=levelinfo.end();++it){
		p.x = disp.x;
		for( auto itr = (it->second).begin();itr !=(it->second).end(); ++itr ){
			DatapathGElement* e = (*itr);
			e->compute(p.x,p.y,element_size.width,element_size.height,gFont);
			p.x +=(disp.x+element_size.width);
		}
		p.y +=(disp.y+element_size.height);
	}
}

int DatapathGraphInfo::getOutlineRects(SDL_Rect** rects){
	int i = 0;
	*rects = new SDL_Rect[graphelements.size()];
	for(auto it = levelinfo.begin();it!=levelinfo.end();++it){
		for( auto itr = (it->second).begin();itr !=(it->second).end(); ++itr, ++i ){
			DatapathGElement* e = (*itr);
			(*rects)[i] = (e->getOutlineRect());
		}
	}

	return 1;
}


int DatapathGraphInfo::getTextTexture(LTexture* gTextTexture[],SDL_Rect* gTextPosition,const SDL_Color& color){
	int i = 0;
	for(auto it = levelinfo.begin();it!=levelinfo.end();++it){
		for( auto itr = (it->second).begin();itr !=(it->second).end(); ++itr, ++i ){
			DatapathGElement* e = (*itr);
			gTextTexture[i]->loadFromRenderedText(e->getName(),color);
			gTextPosition[i] = (e->getTextPosition());
		}
	}

	return 1;
}


#include <iostream>
void DatapathGraphInfo::printlevel(){
	for(std::map<int, std::list<DatapathGElement*> >::const_iterator it = levelinfo.begin();it!=levelinfo.end();++it){
		//std::cout<<"level "<<it->first<<std::endl;
		for( std::list<DatapathGElement*>::const_iterator itr = (it->second).begin();itr !=(it->second).end(); ++itr ){
			//printf("  x %f y %f 	",(*itr)->getPos().x,(*itr)->getPos().y);
		}
	}
}

