#include "bookModel.h"
#include <iostream>

bookModel::bookModel(dbrequest & _db,QObject *parent):db(_db),QAbstractTableModel(parent){
}
bookModel::~bookModel(){
}

void bookModel::init(bool sampling){
	allMets = db.getAllMets(BatchDetail::getBatchDetail().idTestSet,sampling);
	emit endResetModel();
}

void bookModelInventory::init(){
	allMets = db.getAllBooks(BatchDetail::getBatchDetail().idTestSet);
}

QModelIndex bookModel::parent(const QModelIndex &child) const {
								 return QModelIndex();
		
}

int bookModel::rowCount(const QModelIndex &parent ) const{
	return allMets.size();
}

int bookModel::columnCount(const QModelIndex &parent ) const{
	return allMets.size()!=0 ? allMets[0].size() : 0;
}

int bookModel::idMets ( const QModelIndex &index ){
	QString tmp = allMets[index.row()][0].value<QString>();
	return tmp.toInt(); 
}

QVariant bookModel::data(const QModelIndex &index, int role ) const{


	if ( role == Qt::CheckStateRole ) return QVariant();

	if(role==Qt::DecorationRole && index.column()==3 ){
		QVariant ret = allMets[index.row()][index.column()];
		return ret;
	}

	if ( role == Qt::DisplayRole && index.column()!=3 ) {
		QVariant ret = allMets[index.row()][index.column()];
		return ret;
	}

	return QVariant();
}

QVariant bookModelInventory::data(const QModelIndex &index, int role ) const{

	if ( role == Qt::CheckStateRole ) return QVariant();

		if ( role == Qt::DisplayRole ) {
		QVariant ret = allMets[index.row()][index.column()];
		return ret;
	}

	return QVariant();
}

QVariant bookModel::headerData ( int section, Qt::Orientation orientation, int role ) const{
	//static char* table[]={"ID","PATH","METS", " ","Page","BIBREC_245a","BIBREC_245b","BIBREC_100a-1","BIBREC_100a-2","BIBREC_008-35-37","BIBREC_260b","BIBREC_260c","ITEMbarCode","BIBREC_SYS_NUM"};
	static char* table[]={"ID","PATH","METS", " ","BIBREC_245a",
		                  "BIBREC_100a","BIBREC_260b","ITEMbarCode","BIBREC_SYS_NUM","CHECKED","R","N","D"};

	QVariant ret=QVariant();
	if ( role == Qt::DisplayRole && section < 11 && section >= 0 ) {
		char * tmp = table [section];
		QString qs ( tmp );
		ret = QVariant (qs);
	}
	return ret;
}


QVariant bookModelInventory::headerData ( int section, Qt::Orientation orientation, int role  ) const{
	static char* table[]={"CHECKED","BATCH","language", "BIBREC_SYS_NUM","ITEMbarCode","BIBREC_245a",
		                  "BIBREC_100a","BIBREC_260b","BIBREC_008_7_10","N","D"};

	QVariant ret=QVariant();
	if ( role == Qt::DisplayRole && section < 9 && section >= 0 ) {
		char * tmp = table [section];
		QString qs ( tmp );
		ret = QVariant (qs);
	}
	return ret;
}

bookModelE::bookModelE(dbrequest & _db,QObject *parent):bookModel(_db,parent){
}

bookModelE::~bookModelE(){
}

QModelIndex bookModelE::parent(const QModelIndex &child)const{
	return bookModel::parent (child);
}
int bookModelE::columnCount(const QModelIndex &parent)const {	
	return bookModel::columnCount (parent)+1;
}
QVariant bookModelE::data(const QModelIndex &index, int role)const{
	if ( index.column() == 0 ) return QVariant();
	return bookModel::data (index.sibling(index.row(),index.column()-1),role);
}

QVariant bookModelE::headerData ( int section, Qt::Orientation orientation, int role  )const{

	if ( section > 0  ) return bookModel::headerData(section - 1 , orientation, role ); 
    return QVariant("suivi");
}

Qt::ItemFlags bookModelE::flags(const QModelIndex &index) const {

	Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
	if ( index.isValid() ) {
		if (index.column()==0)
		return Qt::ItemIsEditable | defaultFlags;
	}
	return defaultFlags;

}

bool bookModelE::setData(const QModelIndex &index, const QVariant &value, int /* role */)
{
    if (index.column() != 0)
        return false;

    
    return true;
}