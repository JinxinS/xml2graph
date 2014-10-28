/*
 * DatapathGraphInfo.h
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#ifndef DATAPATHGRAPHINFO_H_
#define DATAPATHGRAPHINFO_H_
#include <string>
#include <list>
#include <map>

class DatapathGElement;
class DatapathGraphInfo {
	std::string name;
	std::map<std::string, DatapathGElement*> graphelements;
private:

public:
	DatapathGraphInfo(const std::string&);
	virtual ~DatapathGraphInfo();
	void addElement(const char* fname,DatapathGElement* element);
	void addPortToElement(const char* fname, const char* iname,const char* oname);
	void levelize();

};

#endif /* DATAPATHGRAPHINFO_H_ */
