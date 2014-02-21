#ifndef DATAAGGREGATOR_H_
#define DATAAGGREGATOR_H_

#include "datafactory.h"
#include "../common_files/struct.h"
#include <set>

//! create structs for future verification
class dataaggregator{

private:
	datafactory *dfTemp;
	Item *currentItem;
	Item *rootItem;
	std::vector<Item*> vectItem;
	std::vector<typeBlock> vectTypeBlock;
	DivType divType;
	std::string fileName;
	std::set<std::string> to_extract;
public:
	dataaggregator(datafactory *df,std::string fname);
	
private:	
	void transform();
	void findArticle(Item *item);
	void createArticleFactory();
	void findBlockToArticle(Item *item);
	void findDiv(Item *item);
	void createFactoryDivType();
	void deleteCurrentItem();
	void createGroupId();
	void createIssue();
};

#endif // DATAAGGREGATOR_H_