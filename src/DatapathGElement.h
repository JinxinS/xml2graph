/*
 * DatapathGElement.h
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#ifndef DATAPATHGELEMENT_H_
#define DATAPATHGELEMENT_H_
#include <string>
#include <list>
#include <map>
#include <set>
class DatapathGElementInput;
class DatapathGElementOutput;


class DatapathGElement {
	const char*  name;
	const char*  function;
	std::set<DatapathGElementInput*> inputs;
	std::map<char,DatapathGElementOutput*> 	outputs;
	int level;

private:
	void addInPort(const char*);
	void registerDestPort(DatapathGElementInput*i,const char*);
public:
	DatapathGElement(const DatapathGElement&);
	DatapathGElement& operator=(const DatapathGElement&);
	DatapathGElement(const char* n,const char* f);
	virtual ~DatapathGElement();
	void connect(const char* i,DatapathGElement* s,const char*o );
	void levelize(int);
};

#endif /* DATAPATHGELEMENT_H_ */
