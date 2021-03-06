#include "errorHandler.h"
#include <sstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include <locale.h>

using namespace xercesc;

void errorHandler::setDatabase(database *database)
{
	db = database;
}
std::string convertStringErrorMessage(const Error &error)
{
	std::ostringstream os;
	
	os << " -> line :" << error.errorline << " column :" << error.errorcolumn
                  << (error.severity == 1
                      ? " error: "
                      : " warning: ") << error.message;
	return os.str();
}

//! write message to log file
//! \param catagory error category
//! \param fileName name of the current Mets
//! \param message the string to write in the log
void errorHandler::writeToLog(errorType category,const std::string &fileName, const std::string &mess)
{	
	ErrorTypeMets error =  db->getErrorTypeWithId(category);
	char dateStr [20];
	char timeStr [9];
	_strdate_s(dateStr);
	std::string date =dateStr;	
	_strtime_s( timeStr );
	date = date + " " + timeStr;
	std::stringstream message;
	message << date << " ERROR: " << category << " " << error.error << ": " << mess ;
	//std::string message = date + " ERROR: "  + error.error + ": " + mess;
		
	FILE *fp;
	fopen_s(&fp,logFilePath.c_str(), "a");
	if (fp!=NULL)
	{
		fprintf(fp,"%s\n", message.str().c_str());
		fclose(fp);
	}
	else
	{
		std::cout << "could not write to log ("<< logFilePath.c_str() <<")--> Programm will continue " << std::endl; 	
	}
}
//! write to log where the parser currently is
void errorHandler::begin(const std::string &info)
{
	FILE *fp;
	fopen_s(&fp,logFilePath.c_str(), "a");
	if (fp!=NULL)
	{
		fprintf(fp,"%s\n", info.c_str());
		fclose(fp);
	}
	else
	{
		std::cout << "could not write to log ("<< logFilePath.c_str() <<")--> Programm will continue :" << info << std::endl; 	
	}
}
//! set path of log
void errorHandler::setlogFilePath(const std::string &pathLog)
{
	logFilePath = pathLog;
}

//! function called when a date error found
void errorHandler::getDateError(errorType category,std::string dateBegin,std::string dateEnd,std::string issues,std::string comment)
{
	writeToLog(category,"",comment);		
	db->insertDateError(category,dateBegin,dateEnd,issues,comment);
}

//! get Error from the verifiers
void errorHandler::getError(errorType category,const std::string &relatedType,const std::string &file_part, const std::string &message,const std::string &fName, std::string id)
{		
	writeToLog(category,fName,message);		
	Error e;
	e.errorcolumn =0;
	e.errorline =0;
	e.message = message;
	db->insertMetsErrorWithId(category,relatedType,file_part,e,id);
}	

void errorHandler::warning (const xercesc::SAXParseException& e){
  handle (e, s_warning);
}

void errorHandler::error (const xercesc::SAXParseException& e){
  handle (e, s_error);
  //throw new std::exception("Error");
}

void errorHandler::fatalError (const xercesc::SAXParseException& e){
  handle (e, s_fatal);
  throw new std::exception("Fatal");
}

void errorHandler::handle (const xercesc::SAXParseException& e, severity s){

	stringstream ss ;
  const XMLCh* xid (e.getPublicId ());

  if (xid == 0)
    xid = e.getSystemId ();

  char* id (XMLString::transcode (xid));
  char* msg (XMLString::transcode (e.getMessage ()));

  ss << id << ":" << e.getLineNumber () << ":" << e.getColumnNumber ()
	  << " " << (s == s_warning ? "warning: " : ( s == s_error ? "error: " : "fatal:") ) << msg ;

  errorType category = cat_schema_err; // todo define in better way

  char* systemId ( XMLString::transcode ( e.getSystemId()));

  string file_part= systemId;
  writeToLog(category,file_part,ss.str());
  std::cerr << "error" << ss.str() << std::endl;
  Error ee;
  ee.errorcolumn = e.getColumnNumber ();
  ee.errorline =e.getLineNumber ();
  ee.message = ss.str();
  //string relatedType ="XML"; // todo define correcltly 

  db->insertMetsErrorWithId(category,_relatedType,_file_part,ee,ee.message);
 

  XMLString::release (&id);
  XMLString::release (&msg);
  XMLString::release (&systemId);
}
