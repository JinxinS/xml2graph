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
class DatapathGraphInfo;
class XmlReader {
private:
	static XMLCSTR tag;
	static XMLCSTR attr_siwname;
	static XMLCSTR attr_name;
	static XMLCSTR attr_simid;
	static XMLCSTR attr_file;
	static XMLCSTR attr_line;
	static XMLCSTR tag_fuplace;
	static XMLCSTR attr_label;
	static XMLCSTR tag_function;
	static XMLCSTR attr_tname;
	static XMLCSTR attr_funame;
	static XMLCSTR attr_funcname;
	static XMLCSTR tag_dbginfo;
	static XMLCSTR tag_funcarg;
	static XMLCSTR attr_value;
	static XMLCSTR tag_state;
	static XMLCSTR attr_id;
	static XMLCSTR tag_fumem;
	static XMLCSTR attr_nwords;
	static XMLCSTR attr_a_width;
	static XMLCSTR attr_d_width;
	static XMLNode xmlMainNode;

	DatapathGraphInfo * graphInfo;
	void instantiate();
	void route();

public:
	DatapathGraphInfo* read(const char*);
	XmlReader();
	virtual ~XmlReader();
	XmlReader(const XmlReader&);
	XmlReader& operator=(const XmlReader&);
};

#endif /* XMLREADER_H_ */
