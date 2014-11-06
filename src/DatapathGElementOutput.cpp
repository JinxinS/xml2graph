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

int DatapathGElementOutput::rand_number(int from, int to){
	//srand ((unsigned)time(NULL));
	return (2*rand())%(to -from) +from;
}


SDL_LNode* DatapathGElementOutput::route(const SDL_Rect& outlineR1,const SDL_Output& origin,const SDL_Rect& outlineR2, const SDL_Arrow& destination){
	SDL_LNode* route_start =  new SDL_LNode(origin.p);
	SDL_LNode* route_end   =  new SDL_LNode(destination.cp);
	const int ox = origin.p.x;
	const int oy = origin.p.y;
	const int dx = destination.cp.x;
	const int dy = destination.cp.y;

	const int oboundary_xlft = outlineR1.x;
	const int oboundary_xrht = outlineR1.x + outlineR1.w;
	const int oboundary_yup  = outlineR1.y;
	const int oboundary_ydn  = outlineR1.y + outlineR1.h;

	const int dboundary_xlft = outlineR2.x;
	const int dboundary_xrht = outlineR2.x + outlineR2.w;
	const int dboundary_yup  = outlineR2.y;
	const int dboundary_ydn  = outlineR2.y + outlineR2.h;

	const int free_row_space = outlineR2.w/hsratio ;
	const int free_col_space = outlineR2.h/hsratio ;


	if(oy < dy){
		if( (dboundary_yup - oboundary_ydn) >  free_col_space){
		//	route_start->next = route_end;
		}else{
			int cy = rand_number(dy-12,dy-5);
			printf("from %d to %d cy %d \n",dy-12,dy-8,cy);

			SDL_Point c1 = { ox, cy};
			SDL_LNode* corner1 =  new SDL_LNode(c1);
			route_start->next = corner1;
			SDL_Point c2 = { dx, cy };
			SDL_LNode* corner2 =  new SDL_LNode(c2);
			corner1->next = corner2;
			corner2->next = route_end;

		}
	}
	//	else if(oy > dy){
	//		SDL_Point c1 = {ox, (oy + 8) };
	//		SDL_LNode* corner1 =  new SDL_LNode(c1);
	//		route_start->next = corner1;
	//		SDL_Point c2 = {abs(dx- ox), (oy + 8)};
	//		SDL_LNode* corner2 =  new SDL_LNode(c2);
	//		corner1->next = corner2;
	//		SDL_Point c3 = {abs(dx - ox), dy - 4 };
	//		SDL_LNode* corner3 =  new SDL_LNode(c3);
	//		corner2->next = corner3;
	//		SDL_Point c4 = {dx, dy - 4 };
	//		SDL_LNode* corner4 =  new SDL_LNode(c4);
	//		corner3->next = corner4;
	//		corner4->next = route_end;
	//	}
	//	else{
	//		route_start->next = route_end;
	//	}
	return route_start;
}

void DatapathGElementOutput::computeConnections(std::set<SDL_LNode*>& cset){

	if ((this->pos.p.x==0)&&(this->pos.p.y == 0)) return;
	const SDL_Rect&  outlineR1 = parent->getOutlineRect();

	for(std::set<DatapathGElementInput*>::const_iterator it = destationInputs.begin(); it!=destationInputs.end();++it){
		const SDL_Rect&  outlineR2 = (*it)->getParent()->getOutlineRect();

		SDL_LNode* r = route(outlineR1,this->pos,outlineR2,(*it)->getArrowPosition());
		cset.insert(r);
		//		printf("%s.%s x(%d) y(%d), %s.%s x(%d) y(%d) \n",this->getParent()->getName(),this->getText(),
		//				route_start->p.x, route_start->p.y,
		//				(*it)->getParent()->getName(),(*it)->getText(),route_end->p.x,route_end->p.y);
	}
}
