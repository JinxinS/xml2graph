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
offset(4),
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
	//offset += col;
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

int DatapathGElementOutput::adjustL1(SDL_Line* l1,SDL_Line*l, int upper_bound, int lower_bound){

	int diff = l->start.y - l1->end.y + 4 ;

	if((l1->end.y - diff) >  upper_bound) {
		l1->end.y -= diff;
		l1->next->start.y = l1->end.y;
		l1->next->end.y   = l1->end.y;
		l1->next->next->start.y = l1->end.y;
		return 1;
	}
	else if((l->start.y + diff) < l->end.y ) {
		l->start.y  += diff;
		l->prev->end.y	 =  l->start.y;
		l->prev->start.y =  l->start.y;
		l->prev->end.y 	 =  l->start.y;
		l->prev->prev->end.y =  l->start.y;
		return 0;
	}else{
		printf("adjust failed !\n");
		exit(0);
	}

	//return 0 means the lines already existed is adjusted
	//return 1 means the current line is adjusted
}

int DatapathGElementOutput::adjustL2(SDL_Line* l2,SDL_Line*l, std::set<int>& hvals){

	int diff = 0;

	do{
		diff+=1;
	}while(hvals.find((l2->end.y - diff)) != hvals.end());

	if((l2->end.y - diff) >  l2->prev->start.y) {
		l2->end.y   -= diff;
		l2->start.y -= diff;
		l2->prev->end.y -= diff;
		l2->next->start.y -=diff;
		hvals.insert(l2->end.y);
		return 1;
	}
//	else if((l->end.y + diff) < l->next->end.y ) {
//		l->end.y  += diff;
//		l->start.y += diff;
//		l->prev->end.y += diff;
//		l->next->start.y +=diff;
//		hvals.insert(l->end.y);
//		return 0;
//	}
	else{
		printf("adjust failed !\n");
		exit(0);
	}

	//return 0 means the lines already existed is adjusted
	//return 1 means the current line is adjusted
}

bool DatapathGElementOutput::isOverLap(SDL_Line* l1,SDL_Line*l2){
	if(l1->start.x == l2->start.x){
		return ((l1->end.y >= l2->start.y)&&(l1->start.y < l2->end.y));
	}
	if(l1->start.y == l2->start.y){
		return ((l1->end.x >= l2->start.x)&&(l1->start.x < l2->end.x) && ((l1->start.x != l2->start.x)&&(l1->end.x != l2->end.x)));
	}
	return false;
}

void DatapathGElementOutput::adjustRoutes(int offset,int idx){
	this->offset = offset;

	if(idx ==3){
		for(auto r = routes.begin(); r != routes.end(); ++r ){
			SDL_Line* l1 = *r;
			SDL_Line* l2 = l1->next;
			SDL_Line* l3 = l2->next;
			l1->end.y = offset;
			l2->start.y = offset;
			l2->end.y = offset;
			l3->start.y = offset;
		}
	}
	else if(idx ==2){
		for(auto r = routes.begin(); r != routes.end(); ++r ){
			SDL_Line* l1 = *r;
			SDL_Line* l2 = l1->next;
			SDL_Line* l3 = l2->next;
			l1->end.y = offset;
			l2->start.y = offset;
			l2->end.y = offset;
			l3->start.y = offset;
		}
	}


}

void DatapathGElementOutput::route(const SDL_Rect& outlineR1,const SDL_Output& origin,const SDL_Rect& outlineR2, const SDL_Arrow& destination,std::set<SDL_Line*>& lines){
	//	SDL_LNode* route_start =  new SDL_LNode(origin.p,this->parent->getName());
	//	SDL_LNode* route_end   =  new SDL_LNode(destination.cp);
	const int ox = origin.p.x;
	const int oy = origin.p.y;
	const int dx = destination.cp.x;
	const int dy = destination.cp.y;

//	const int oleft_bound = outlineR1.x;
//	const int oright_bound = outlineR1.x + outlineR1.w;
//	const int oupper_bound  = outlineR1.y;
	const int olower_bound  = outlineR1.y + outlineR1.h;

//	const int dleft_bound = outlineR2.x;
//	const int dright_bound = outlineR2.x + outlineR2.w;
	const int dupper_bound  = outlineR2.y;
//	const int dlower_bound  = outlineR2.y + outlineR2.h;

//	const int free_row_space = outlineR2.w/hsratio ;
	const int free_col_space = outlineR2.h/hsratio ;


	if(oy < dy){
		if( (dupper_bound - olower_bound) >  free_col_space){
			//			int cy = oy + offset;
			//			int c2x = (ox > dx) ? (oboundary_xlft- offset ) :  (oboundary_xrht + offset );
			//			int c3y = dy - 8;
			//
			//			SDL_Point c1 = { ox, cy};
			//			SDL_Point c2 = { c2x, cy };
			//			SDL_Point c3 = { c2x, c3y};
			//			SDL_Point c4 = { dx, c3y };
			//			SDL_Line* l1= new SDL_Line(origin.p, c1);
			//			SDL_Line* l2= new SDL_Line(c1, c2);
			//			SDL_Line* l3= new SDL_Line(c2, c3);
			//			SDL_Line* l4= new SDL_Line(c3, c4);
			//			SDL_Line* l5= new SDL_Line(c4, destination.cp);
			//
			//			lines.insert(l1);
			//			lines.insert(l2);
			//			lines.insert(l3);
			//			lines.insert(l4);
			//			lines.insert(l5);

		}else{
			int cy = oy + offset;
			//printf("%s.%s -> %d \n",this->getParent()->getName(),this->getText(),cy);
			SDL_Point c1 = { ox, cy};
			SDL_Point c2 = { dx, cy };
			SDL_Line* l1= new SDL_Line(origin.p, c1,1,NULL,this);
			SDL_Line* l2= new SDL_Line(c1, c2, 2,l1,this);
			SDL_Line* l3= new SDL_Line(c2, destination.cp, 3,l2,this);
			l1->next = l2;
			l2->next = l3;
			std::set<int> horizonvals;
			for(auto it = lines.begin();it != lines.end();++it){
				SDL_Line* l = (*it);
				if(l->idx == 2) horizonvals.insert(l->start.y);
			}

			bool overlap = false;
			do{
				overlap = false;
				for(auto it = lines.begin();it != lines.end();++it){
					SDL_Line* l = (*it);
					if(l->idx == 3){
						overlap =  isOverLap(l1,l);
						if(overlap) printf("vertical overlap![%d %d] l1 (%d %d) -> (%d %d) l2 (%d %d) -> (%d %d)\n",
								olower_bound,
								dupper_bound,
								l1->start.x,l1->start.y,l1->end.x,l1->end.y,
								l->start.x,l->start.y,l->end.x,l->end.y);
						if(overlap){
							int ret = adjustL1(l1,l,olower_bound,dupper_bound);

							if (ret == 0){ //adjust all lines derived from line(l)'s origin
								l->origin->adjustRoutes(l->start.y,l->idx);
							}else if(ret == 1){//correct this output port's offset
								offset = l1->end.y;
							}
						}
					}
					if(l->idx == 2){
						overlap =  isOverLap(l2,l);
						if(overlap) printf("Horizon overlap![%d %d] l2 (%d %d) -> (%d %d) l (%d %d) -> (%d %d)\n",
								olower_bound,
								dupper_bound,
								l2->start.x,l2->start.y,l2->end.x,l2->end.y,
								l->start.x,l->start.y,l->end.x,l->end.y);

						if (overlap){
							int ret = adjustL2(l2,l,horizonvals);

							if (ret == 0){ //adjust all lines derived from line(l)'s origin
								l->origin->adjustRoutes(l->start.y,l->idx);
							}else if(ret == 1){//correct this output port's offset
								offset = l2->start.y;
							}
						}
						if(overlap) printf("adjusted Horizon overlap![%d %d] l2 (%d %d) -> (%d %d) l (%d %d) -> (%d %d)\n",
								olower_bound,
								dupper_bound,
								l2->start.x,l2->start.y,l2->end.x,l2->end.y,
								l->start.x,l->start.y,l->end.x,l->end.y);
					}
				}
			}while(overlap);

			lines.insert(l1);
			lines.insert(l2);
			lines.insert(l3);
			routes.insert(l1);
		}
	}
	//	else if(oy > dy){						//feedback
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

}

void DatapathGElementOutput::computeConnections(std::set<SDL_Line*>& lines){

	if ((this->pos.p.x==0)&&(this->pos.p.y == 0)) return;
	const SDL_Rect&  outlineR1 = parent->getOutlineRect();

	for(std::set<DatapathGElementInput*>::const_iterator it = destationInputs.begin(); it!=destationInputs.end();++it){
		const SDL_Rect&  outlineR2 = (*it)->getParent()->getOutlineRect();
		printf("routing %s.%s  , %s.%s   \n",this->getParent()->getName(),this->getText(),
				(*it)->getParent()->getName(),(*it)->getText());

		route(outlineR1,this->pos,outlineR2,(*it)->getArrowPosition(),lines);

	}
}
