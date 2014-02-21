#ifndef VERIFYCOVERAGEPERCENTAGEALTOBLOCKS_H_
#define VERIFYCOVERAGEPERCENTAGEALTOBLOCKS_H_

#include <string>
#include "datafactory.h"
#include "errorhandler.h"

//! Verify if the sum of the areas of the ALTO blocks divided by the total page size is > 75%
class verifycoveragepercentagealtoblocks
{
private :
	std::set<std::pair<std::string, std::string>> block;	

public:
	verifycoveragepercentagealtoblocks(datafactory *df,ErrorHandler *h,std::string &fileName);
};


#endif	//VERIFYCOVERAGEPERCENTAGEALTOBLOCKS_H_