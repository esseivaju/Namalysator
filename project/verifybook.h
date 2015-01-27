#ifndef _VERIFY_BOOK_H_
#define _VERIFY_BOOK_H_

#include "metsparser.h"
#include "sql.h"
#include <string>
#include "inventory.h"
#include "errorhandler.h"

class verifyBook {
private :
	database *db;
	inventory invent;
	errorHandler *hError;


	void init(metsparserContext&);
public :
	verifyBook();
	virtual void check(std::string,metsparserContext&);

};

#endif