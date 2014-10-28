/*
 * DatapathGElementInput.h
 *
 *  Created on: Oct 27, 2014
 *      Author: songjinxin
 */

#ifndef DATAPATHGELEMENTINPUT_H_
#define DATAPATHGELEMENTINPUT_H_
#include <string>
class DatapathGElement;
class DatapathGElementOutput;
class DatapathGElementInput {
	const char* name;
	DatapathGElementOutput * sourceOutput;
	DatapathGElement* parent;
public:
	DatapathGElementInput(const char* n, DatapathGElement* p);
	virtual ~DatapathGElementInput();
	void registerOutput(DatapathGElementOutput*);
	void levelize(int lvl);
	DatapathGElementInput(const DatapathGElementInput&);
	DatapathGElementInput& operator=(const DatapathGElementInput&);
};

#endif /* DATAPATHGELEMENTINPUT_H_ */
