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

int DatapathGElementOutput::rand_number(int from, int to){
	//srand ((unsigned)time(NULL));
	return (2*rand())%(to -from) +from;
}

int DatapathGElementOutput::adjustL1(SDL_Line* l1,SDL_Line*l, std::set<int>& hvals){

	int diff = 0;

	do{
		diff+= 3;
	}while(hvals.find((l1->end.y - diff)) != hvals.end());


	if((l1->end.y - diff) >  l1->start.y) {
		l1->end.y -= diff;
		l1->next->start.y = l1->end.y;
		l1->next->end.y   = l1->end.y;
		l1->next->next->start.y = l1->end.y;
		hvals.insert(l1->end.y);
		return 1;
	}
	//	else if((l->start.y + diff) < l->end.y ) {
	//		l->start.y  += diff;
	//		l->prev->end.y	 =  l->start.y;
	//		l->prev->start.y =  l->start.y;
	//		l->prev->end.y 	 =  l->start.y;
	//		l->prev->prev->end.y =  l->start.y;
	//		return 0;
	//	}
	else{
		printf("adjustL1 failed !\n");
		exit(0);
	}

	//return 0 means the lines already existed is adjusted
	//return 1 means the current line is adjusted
}

int DatapathGElementOutput::adjustL2(SDL_Line* l2,SDL_Line*l, std::set<int>& hvals){

	int diff = 0;

	do{
		diff+= 3;
	}while(hvals.find((l2->end.y - diff)) != hvals.end());

	if((l2->end.y - diff) >  l2->prev->start.y) {
		l2->end.y   -= diff;
		l2->start.y -= diff;
		l2->prev->end.y -= diff;
		l2->next->start.y -=diff;
		hvals.insert(l2->end.y);
		return 1;
	}
	else{
		printf("adjustL2 failed !\n");
		exit(0);
	}

	//return 0 means the lines already existed is adjusted
	//return 1 means the current line is adjusted
}

int DatapathGElementOutput::adjustL3(SDL_Line* l3,SDL_Line*l, std::set<int>& vvals, bool left){

	int diff = 0;
	int val = l3->start.x;
	do{
		diff+= 6;
		if(left) val -= diff;
		else val += diff;
	}while(vvals.find(val) != vvals.end());

	bool boundarycheck = true;
	if(boundarycheck) {
		l3->start.x = val;
		l3->end.x = val;
		l3->next->start.x=val;
		l3->prev->end.x = val;
		vvals.insert(val);
		return 1;
	}
	else{
		printf("adjustL3 failed !\n");
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
	yoffset = offset;
	//	printf("%s.%s offset set to %d \n",this->getParent()->getName(),this->getText(),offset);

	//if(idx ==3){
	for(auto r = routes.begin(); r != routes.end(); ++r ){
		SDL_Line* l1 = *r;
		SDL_Line* l2 = l1->next;
		SDL_Line* l3 = l2->next;
		l1->end.y = offset;
		l2->start.y = offset;
		l2->end.y = offset;
		l3->start.y = offset;
	}
	//}
	//	else if(idx ==2){
	//		for(auto r = routes.begin(); r != routes.end(); ++r ){
	//			SDL_Line* l1 = *r;
	//			SDL_Line* l2 = l1->next;
	//			SDL_Line* l3 = l2->next;
	//			l1->end.y = offset;
	//			l2->start.y = offset;
	//			l2->end.y = offset;
	//			l3->start.y = offset;
	//		}
	//	}
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

	bool overlap = false;

	std::set<int> horizonvals;
	std::set<int> verticalvals;

	for(auto it = lines.begin();it != lines.end();++it){
		SDL_Line* l = (*it);
		if((l->idx == 2)||(l->idx == 4)) horizonvals.insert(l->start.y);
		if(l->idx == 3) verticalvals.insert(l->start.y);
	}
	printf("yoffset is %d free_col_space/2 is %d \n",yoffset,free_col_space/2);
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

			for(auto it = lines.begin();it != lines.end();++it){
				SDL_Line* l = (*it);
				if(l->idx == 3){
					overlap =  isOverLap(l1,l);
					if(overlap){
						int ret = adjustL1(l1,l,horizonvals);
						yoffset = l1->end.y - l1->start.y;
					}

					overlap = isOverLap(l3,l);
					if(overlap){
						int ret = adjustL3(l3,l,horizonvals,adjustLR);
						yoffset = l1->end.y - l1->start.y;
					}
				}
				else if(l->idx == 5){
					overlap =  isOverLap(l1,l);
					if(overlap){
						int ret = adjustL1(l1,l,horizonvals);
						yoffset = l1->end.y - l1->start.y;
					}
				}


			}

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
			//			printf("initial %s.%s l1 ->(%d %d) (%d %d) ",this->getParent()->getName(),this->getText(),l1->start.x,l1->start.y
			//					,l1->end.x,l1->end.y);
			//			printf(" l2 ->(%d %d) (%d %d) ",l2->start.x,l2->start.y
			//					,l2->end.x,l2->end.y);
			//			printf(" l3 ->(%d %d) (%d %d) \n",l3->start.x,l3->start.y
			//					,l3->end.x,l3->end.y);
			for(auto it = lines.begin();it != lines.end();++it){
				SDL_Line* l = (*it);
				if(l->idx == 3){
					overlap =  isOverLap(l1,l);
					//					if(overlap) printf("vertical overlap![%d %d] l1 (%d %d) -> (%d %d) l2 (%d %d) -> (%d %d)\n",
					//							olower_bound,
					//							dupper_bound,
					//							l1->start.x,l1->start.y,l1->end.x,l1->end.y,
					//							l->start.x,l->start.y,l->end.x,l->end.y);
					if(overlap){
						int ret = adjustL1(l1,l,horizonvals);
						yoffset = l1->end.y - l1->start.y;
						//printf("%s.%s offset set to %d \n",this->getParent()->getName(),this->getText(),offset);
					}
				}
				else if(l->idx == 2){
					overlap =  isOverLap(l2,l);
					//					if(overlap) printf("Horizon overlap![%d %d] l2 (%d %d) -> (%d %d) l (%d %d) -> (%d %d)\n",
					//							olower_bound,
					//							dupper_bound,
					//							l2->start.x,l2->start.y,l2->end.x,l2->end.y,
					//							l->start.x,l->start.y,l->end.x,l->end.y);

					if (overlap){
						int ret = adjustL2(l2,l,horizonvals);
						yoffset = l2->start.y - l1->start.y ;
						//printf("%s.%s offset set to %d \n",this->getParent()->getName(),this->getText(),offset);
					}
					//					if(overlap) printf("adjusted Horizon overlap![%d %d] l2 (%d %d) -> (%d %d) l (%d %d) -> (%d %d)\n",
					//							olower_bound,
					//							dupper_bound,
					//							l2->start.x,l2->start.y,l2->end.x,l2->end.y,
					//							l->start.x,l->start.y,l->end.x,l->end.y);
				}
			}
			//			if(!strcmp(this->getParent()->getName(),"tu_read")) {
			//				printf("final %s.%s l1 ->(%d %d) (%d %d) ",this->getParent()->getName(),this->getText(),l1->start.x,l1->start.y
			//						,l1->end.x,l1->end.y);
			//				printf(" l2 ->(%d %d) (%d %d) ",l2->start.x,l2->start.y
			//						,l2->end.x,l2->end.y);
			//				printf(" l3 ->(%d %d) (%d %d) \n",l3->start.x,l3->start.y
			//						,l3->end.x,l3->end.y);
			//			}
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
		//	if(!strcmp(this->getParent()->getName(),"tu_read"))
		printf("routing %s.%s  , %s.%s   \n",this->getParent()->getName(),this->getText(),
				(*it)->getParent()->getName(),(*it)->getText());
		route(outlineR1,this->pos,outlineR2,(*it)->getArrowPosition(),lines);
		//	if(!strcmp(this->getParent()->getName(),"tu_read")) printf("------------------------------------------------\n");
	}
}
