#ifndef ALTOPARSER_H_
#define ALTOPARSER_H_

#include "../common_files/xmlparser.h"
#include "../common_files/struct.h"
#include "../common_files/utilities.h"
// #include "schema_validatoralto.h"
#include "../common_files/xmltostr.h"
#include "dataitem.h"
#include "datafactory.h"
#include "errorhandler.h"
#include "datafactory.h"
#include "stateparser.h"


class altoparserContext : public StateParserContext{
	
public :
	/*
	enum state 
	{	
		state_header,
		state_alto,
		state_block		
	};
	*/

	//state currentstate;
	//std::string content;	
	std::string alto;
	datafactory *dfalto;
	//std::string xml;
	std::string altoFile;
	altoblock altoBlock;
	//std::set<std::string> to_extract;
	errorHandler *hError;
	bool /*headerIn, measurement_unit,*/is_mm10/*,convert_to_pixels*/;	
	int  dpi;
};

class StateParserAltoRootState:public StateParserState{
public:
	StateParserAltoRootState(){};
	virtual ~StateParserAltoRootState(){};

	virtual StateParserState* getNext(const char* const name);
};

//! parse ALTO files and store in a datafactory
class altoparser: public StateParserCH/*, public xmlparser_with_stack<altoparser>*/
{
		
private :
	altoparserContext ctx;
	StateParserAltoRootState root;

public:	
	altoparser(const std::string &altoFile,const std::string &altoId,errorHandler *h,datafactory *df);
	virtual void XMLstartElement(const char *name, const char **atts);
	virtual void XMLendElement(const char *name);
	virtual void XMLcharacterData(const char *s, int len);	
	//virtual void getElement(const char *name, const char **atts);
};

#endif	//ALTOPARSER_H_