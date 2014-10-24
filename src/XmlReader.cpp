/*
 * XmlReader.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: songjinxin
 */

#include "XmlReader.h"

XMLNode XmlReader::xmlMainNode;
XmlReader::XmlReader(){
	// TODO Auto-generated constructor stub
}

XmlReader::~XmlReader() {
	// TODO Auto-generated destructor stub
}

bool XmlReader::read(const char* fname){
	printf("reading input file %s ...\n",fname);
	xmlMainNode = XMLNode::openFileHelper(fname,"" );
	printf("done!\n");
	return true;
}
