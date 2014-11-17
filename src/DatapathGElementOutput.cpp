/*
 * DatapathGElementOutput.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#include "DatapathGElementOutput.h"
#include "DatapathGElementInput.h"
#include "DatapathGElement.h"
#include "SDLLine.h"
#include "SDLRoute.h"

DatapathGElementOutput::DatapathGElementOutput(const char* n,DatapathGElement* p):
name(n),
destationInputs(),
parent(p),
pos(),
xoffset(0),
yoffset(0),
origin(),
routes()
{
	origin =  (char*)malloc(strlen(name)+strlen(parent->getName()) + 2);
	strcpy(origin,parent->getName());
	strcat(origin,".");
	strcat(origin,name);
}

DatapathGElementOutput::~DatapathGElementOutput() {
	free(origin);
}

void DatapathGElementOutput::registerInput(DatapathGElementInput* in){
	destationInputs.insert(in);
}

void DatapathGElementOutput::levelize(int row,int col,std::map<int, std::list<DatapathGElement*> >& linfo){
	for(std::set<DatapathGElementInput*>::const_iterator it = destationInputs.begin(); it!=destationInputs.end();++it){
		(*it)->levelize(row,col,linfo);
	}
}

void DatapathGElementOutput::compute(const int x, const int y,const int xoffset_, const int yoffset_,TTF_Font *font){
	int text_w,text_h;
	TTF_SizeText(font,name,&text_w, &text_h);
	pos.p.x = x;
	pos.p.y = y;
	pos.textp.x = x - (text_w/2);
	pos.textp.y=  y - (text_h);
	yoffset = yoffset_;
	xoffset = xoffset_;
}

int DatapathGElementOutput::adjustL1(SDL_Line* l1, std::set<SDL_Line*>& vlines){

	int diff = 0;
	bool overlap = false;

	do{

		l1->end.y -= diff;
		for(auto it = vlines.begin() ; it != vlines.end(); ++it){
			SDL_Line* l_ = *it;
			overlap = isOverLap(l1,l_);
			if(overlap) break;
		}
		diff+= 3;
	}while((overlap) && (l1->end.y > l1->start.y));


	if(!overlap) {
		l1->next->start.y = l1->end.y;
		l1->next->end.y   = l1->end.y;
		l1->next->next->start.y = l1->end.y;
		return 1;
	}
	else{
		printf("adjustL1 failed !\n");
		exit(0);
	}

	//return 0 means the lines already existed is adjusted
	//return 1 means the current line is adjusted
}

int DatapathGElementOutput::adjustL2(SDL_Line* l2, std::set<SDL_Line*>& hvals){

	int diff = 0;
	bool overlap = false;

	do{
		l2->end.y   -= diff;
		l2->start.y -= diff;

		for(auto it = hvals.begin() ; it != hvals.end(); ++it){
			SDL_Line* l_ = *it;
			overlap = isOverLap(l2,l_);
			if(overlap) break;
		}
		diff+= 3;
	}while(overlap&&(l2->start.y > l2->prev->start.y));

	if(!overlap) {
		l2->prev->end.y = l2->start.y;
		l2->next->start.y =l2->start.y;
		return 1;
	}
	else{
		printf("adjustL2 failed !\n");
		//	exit(0);
	}

	//return 0 means the lines already existed is adjusted
	//return 1 means the current line is adjusted
}

int DatapathGElementOutput::adjustL3(SDL_Line* l3, std::set<SDL_Line*>& vlines, bool left){

	int diff = 0;
	bool overlap = false;
	int xtmp = l3->start.x;
	do{
		l3->start.x = xtmp;
		l3->end.x = xtmp;

		if(left){
			l3->start.x -= diff;
			l3->end.x   -= diff;
		}
		else{
			l3->start.x += diff;
			l3->end.x   += diff;
		}

		for(auto it = vlines.begin() ; it != vlines.end(); ++it){
			SDL_Line* l_ = *it;
			overlap = isOverLap(l3,l_);
			if(overlap) break;
		}
		diff+= 6;
	}while(overlap);

	if(!overlap) {
		l3->next->start.x =l3->start.x ;
		l3->prev->end.x  = l3->start.x ;
		return 1;
	}
	else{
		printf("adjustL3 failed !\n");
		exit(0);
	}

	//return 0 means the lines already existed is adjusted
	//return 1 means the current line is adjusted
}

int DatapathGElementOutput::adjustL4(SDL_Line* l4, const int lowerbound, std::set<SDL_Line*>& hlines){

	int diff = 0;
	bool overlap = true;
	int ytmp = l4->start.y;

	do{
		l4->start.y =ytmp;
		l4->end.y   =ytmp;
		l4->start.y +=diff;
		l4->end.y   +=diff;
		for(auto it = hlines.begin();it!=hlines.end() ;++it){
			SDL_Line* l = *it;
			overlap = isOverLap(l4,l);
			if(overlap) break;
		}

		l4->start.y =ytmp;
		l4->end.y   =ytmp;
		l4->start.y -=diff;
		l4->end.y   -=diff;
		for(auto it = hlines.begin();it!=hlines.end() ;++it){
			SDL_Line* l = *it;
			overlap = isOverLap(l4,l);
			if(overlap) break;
		}
		diff+= 3;
	}while(overlap && (l4->start.y < lowerbound));

	if(!overlap){
		l4->next->start.y =	l4->end.y ;
		l4->prev->end.y   = l4->start.y;
		return 1;
	}
	else{
		{
			printf("adjustL4 failed !\n");
			exit(0);
		}
	}

	//return 0 means the lines already existed is adjusted
	//return 1 means the current line is adjusted
}

bool DatapathGElementOutput::isOverLap(SDL_Line* l1,SDL_Line*l2){
	if(l1->start.x == l2->start.x){
		return ((l1->end.y >= l2->start.y)&&(l1->start.y < l2->end.y));
	}
	if(l1->start.y == l2->start.y){
		int l1minx = std::min(l1->start.x,l1->end.x);
		int l1maxx = std::max(l1->start.x,l1->end.x);
		int l2minx = std::min(l2->start.x,l2->end.x);
		int l2maxx = std::max(l2->start.x,l2->end.x);
		return ((l1maxx >= l2minx)&&(l1minx <= l2maxx ));

	}
	return false;
}

void DatapathGElementOutput::route(const SDL_Rect& outlineR1,const SDL_Output& origin,const SDL_Rect& outlineR2, const SDL_Arrow& destination,std::set<SDL_Line*>& lines){
	//	SDL_LNode* route_start =  new SDL_LNode(origin.p,this->parent->getName());
	//	SDL_LNode* route_end   =  new SDL_LNode(destination.cp);
	const int ox = origin.p.x;
	const int oy = origin.p.y;
	const int dx = destination.cp.x;
	const int dy = outlineR2.y;

	const int oleft_bound = outlineR1.x;
	const int oright_bound = outlineR1.x + outlineR1.w;
	//	const int oupper_bound  = outlineR1.y;
	const int olower_bound  = outlineR1.y + outlineR1.h;

	//	const int dleft_bound = outlineR2.x;
	//	const int dright_bound = outlineR2.x + outlineR2.w;
	const int dupper_bound  = outlineR2.y;
	//	const int dlower_bound  = outlineR2.y + outlineR2.h;

	//	const int free_row_space = outlineR2.w/hsratio ;
	const int free_col_space = outlineR2.h/hsratio ;

	std::set<SDL_Line*> hlines;
	std::set<SDL_Line*> vlines;

	for(auto it = lines.begin();it != lines.end();++it){
		SDL_Line* l = (*it);
		if((l->idx == 2)||(l->idx == 4)) {
			hlines.insert(l);
		}
		if(l->idx == 3) {
			vlines.insert(l);
		}
	}

	if(oy < dy){
		if( (dupper_bound - olower_bound) >  free_col_space){
			bool adjustLR = (ox > dx); //true on left adjust false on right adjust
			int c1y = oy + yoffset;
			int c2x = adjustLR ? (oleft_bound- xoffset ) :  (oright_bound + xoffset );
			int c3y = dy - (free_col_space/2) + 3;
			SDL_Point c1 = { ox,  c1y};
			SDL_Point c2 = { c2x, c1y};
			SDL_Point c3 = { c2x, c3y};
			SDL_Point c4 = { dx,  c3y};
			SDL_Line* l1= new SDL_Line(origin.p, c1, 1, NULL, this);
			SDL_Line* l2= new SDL_Line(c1, c2, 2, l1, this);
			SDL_Line* l3= new SDL_Line(c2, c3, 3, l2, this);
			SDL_Line* l4= new SDL_Line(c3, c4, 4, l3, this);
			SDL_Line* l5= new SDL_Line(c4, destination.cp, 5, l4, this);
			l1->next = l2;
			l2->next = l3;
			l3->next = l4;
			l4->next = l5;
			int ret = adjustL1(l1,vlines);
			if(ret == 1)yoffset = l1->end.y - l1->start.y;
			ret = adjustL2(l2,hlines);
			if(ret == 1)yoffset = l2->start.y - l1->start.y ;
			ret = adjustL3(l3,vlines,adjustLR);
			ret = adjustL4(l4,dupper_bound,hlines);

			lines.insert(l1);
			lines.insert(l2);
			lines.insert(l3);
			lines.insert(l4);
			lines.insert(l5);

		}else{
			int c1y = oy + yoffset;

			SDL_Point c1 = { ox, c1y};
			SDL_Point c2 = { dx, c1y };
			SDL_Line* l1= new SDL_Line(origin.p, c1,1,NULL,this);
			SDL_Line* l2= new SDL_Line(c1, c2, 2,l1,this);
			SDL_Line* l3= new SDL_Line(c2, destination.cp, 3,l2,this);
			l1->next = l2;
			l2->next = l3;

			int ret = adjustL1(l1,vlines);
			if(ret == 1)yoffset = l1->end.y - l1->start.y;

			ret = adjustL2(l2,hlines);
			if(ret == 1)yoffset = l2->start.y - l1->start.y ;
			lines.insert(l1);
			lines.insert(l2);
			lines.insert(l3);
		}
	}
	else if(oy > dy){						//feedback
		bool adjustLR = (ox > dx); //true on left adjust false on right adjust
		int c2x = adjustLR ? (oleft_bound- xoffset ) :  (oright_bound + xoffset );
		int c3y = dy - (free_col_space/2) + 3;
		SDL_Point c1 = {ox, oy + 8 };
		SDL_Point c2 = {c2x, oy + 8};
		SDL_Point c3 = {c2x, c3y };
		SDL_Point c4 = {dx, c3y };
		SDL_Line* l1= new SDL_Line(origin.p, c1, 1, NULL, this);
		SDL_Line* l2= new SDL_Line(c1, c2, 2, l1, this);
		SDL_Line* l3= new SDL_Line(c2, c3, 3, l2, this);
		SDL_Line* l4= new SDL_Line(c3, c4, 4, l3, this);
		SDL_Line* l5= new SDL_Line(c4, destination.cp, 5, l4, this);
		l1->next = l2;
		l2->next = l3;
		l3->next = l4;
		l4->next = l5;

		int ret = adjustL1(l1,vlines);
		if(ret == 1)yoffset = l1->end.y - l1->start.y;
		ret = adjustL2(l2,hlines);
		if(ret == 1)yoffset = l2->start.y - l1->start.y ;
		ret = adjustL3(l3,vlines,adjustLR);
		ret = adjustL4(l4,dupper_bound,hlines);

		lines.insert(l1);
		lines.insert(l2);
		lines.insert(l3);
		lines.insert(l4);
		lines.insert(l5);
	}
}

void DatapathGElementOutput::computeConnections(std::set<SDL_Line*>& lines){

	if ((this->pos.p.x==0)&&(this->pos.p.y == 0)) return;
	const SDL_Rect&  outlineR1 = parent->getOutlineRect();

	for(std::set<DatapathGElementInput*>::const_iterator it = destationInputs.begin(); it!=destationInputs.end();++it){
		const SDL_Rect&  outlineR2 = (*it)->getParent()->getOutlineRect();
		//	if(!strcmp(this->getParent()->getName(),"tu_read"))
//		printf("routing %s.%s  , %s.%s   \n",this->getParent()->getName(),this->getText(),
//				(*it)->getParent()->getName(),(*it)->getText());
		route(outlineR1,this->pos,outlineR2,(*it)->getArrowPosition(),lines);
		//	if(!strcmp(this->getParent()->getName(),"tu_read")) printf("------------------------------------------------\n");
	}
}
