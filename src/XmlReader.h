/*
 * XmlReader.h
 *
 *  Created on: Oct 24, 2014
 *      Author: songjinxin
 */

#ifndef XMLREADER_H_
#define XMLREADER_H_
#include <string>
#include "XMLParser.h"
class XmlReader {
private:
	static XMLNode xmlMainNode;
public:
	bool read(const char*);
	XmlReader();
	virtual ~XmlReader();
	XmlReader(const XmlReader&);
	XmlReader& operator=(const XmlReader&);
};

#endif /* XMLREADER_H_ */
