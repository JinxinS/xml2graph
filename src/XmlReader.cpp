/*
 * XmlReader.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: songjinxin
 */

#include "XmlReader.h"
#include "DatapathGraphInfo.h"
#include "DatapathGElement.h"
#include "DatapathGElementInput.h"
#include <string.h>
XMLCSTR XmlReader::tag  			= "LogicalSideWorks";
XMLCSTR XmlReader::attr_siwname 	= "siw_name";
XMLCSTR XmlReader::attr_name		= "name";
XMLCSTR XmlReader::attr_simid		= "simid";
XMLCSTR XmlReader::attr_file		= "file";
XMLCSTR XmlReader::attr_line		= "line";
XMLCSTR XmlReader::tag_fuplace		= "FUPlace";
XMLCSTR XmlReader::attr_label		= "place_label";
XMLCSTR XmlReader::tag_function	= "FUFunction";
XMLCSTR XmlReader::attr_tname		= "type_name";
XMLCSTR XmlReader::attr_funame		= "fu_name";
XMLCSTR XmlReader::attr_funcname	= "func_name";
XMLCSTR XmlReader::tag_dbginfo		= "DebugInfo";
XMLCSTR XmlReader::tag_funcarg		= "FunctionArg";
XMLCSTR XmlReader::attr_value		= "value";
XMLCSTR XmlReader::tag_state		= "State";
XMLCSTR XmlReader::attr_id			= "id";
XMLCSTR XmlReader::tag_fumem		= "FUMem";
XMLCSTR XmlReader::attr_nwords		= "nwords";
XMLCSTR XmlReader::attr_a_width	= "a_width";
XMLCSTR XmlReader::attr_d_width	= "d_width";
XMLNode XmlReader::xmlMainNode;

XmlReader::XmlReader()
:graphInfo()
{
	// TODO Auto-generated constructor stub
}

XmlReader::~XmlReader() {
	delete graphInfo;
}

DatapathGraphInfo* XmlReader::read(const char* fname){
	printf("reading input file %s ...\n",fname);
	xmlMainNode = XMLNode::openFileHelper(fname,tag );
	instantiate();
	route();
	printf("done!\n");
	return graphInfo;
}


void XmlReader::instantiate(){
	XMLCSTR conf_name = xmlMainNode.getAttribute(attr_name);

	graphInfo = new DatapathGraphInfo(conf_name);

	/*ADD FUFunction*/
	for (int i = 0; i < xmlMainNode.nChildNode(tag_function); i++)
	{
		XMLNode xNode = xmlMainNode.getChildNode(tag_function,i);

		XMLCSTR f_fu_name   = xNode.getAttribute(attr_funame);
		XMLCSTR f_func_name = xNode.getAttribute(attr_funcname);

		/*create new graph element add to graph*/
		graphInfo->addElement(f_fu_name,new DatapathGElement(f_fu_name,f_func_name));

	}

	/*ADD FUMem*/
	for (int i = 0; i < xmlMainNode.nChildNode(tag_fumem); i++)
	{
		XMLNode xNode = xmlMainNode.getChildNode(tag_fumem,i);

		XMLCSTR  f_fu_name = xNode.getAttribute(attr_funame);
		XMLCSTR  f_func_name = "Memory";

		/*create new graph element add to graph*/
		graphInfo->addElement(f_fu_name,new DatapathGElement(f_fu_name,f_func_name));
	}

}

void XmlReader::route(){
	/*ADD FUFunction*/
	for (int i = 0; i < xmlMainNode.nChildNode(tag_function); i++)
	{
		XMLNode xNode = xmlMainNode.getChildNode(tag_function,i);
		XMLCSTR f_fu_name   = xNode.getAttribute(attr_funame);

		for(int j = 0; j < xNode.nChildNode(tag_funcarg); j++)
		{
			XMLNode ArgNode = xNode.getChildNode(tag_funcarg,j);
			XMLCSTR arg_name  = ArgNode.getAttribute(attr_name);
			XMLCSTR arg_value = ArgNode.getAttribute(attr_value);
			/*add input port */
			graphInfo->addPortToElement(f_fu_name,arg_name,arg_value);
		}
	}

	/*ADD FUMem*/
	for (int i = 0; i < xmlMainNode.nChildNode(tag_fumem); i++)
	{
		XMLNode xNode = xmlMainNode.getChildNode(tag_fumem,i);
		XMLCSTR  f_fu_name = xNode.getAttribute(attr_funame);

		for(int j = 0; j < xNode.nChildNode(tag_funcarg); j++)
		{

			XMLNode ArgNode = xNode.getChildNode(tag_funcarg,j);
			XMLCSTR arg_name  = ArgNode.getAttribute(attr_name);
			XMLCSTR arg_value = ArgNode.getAttribute(attr_value);

			/*add input port */
			graphInfo->addPortToElement(f_fu_name,arg_name,arg_value);
		}
	}
}
