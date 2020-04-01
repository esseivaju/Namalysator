#ifndef ENTITYCOUNT
#define ENTITYCOUNT

#include <QtWidgets>

#include "dbrequest.h"
#include "entitycountModel.h"


class w_entitycount : public QWidget
{
    Q_OBJECT

public:
	w_entitycount(QWidget *parent = 0);
	~w_entitycount();

	void init(QMainWindow* _qmain);
	void setBatchDetail();


private:

	entityCountModel* entityModel;
	QMainWindow* qmain;
	QMenu* menu;
	dbrequest db;
private slots :	
	void exportToFile();
};

#endif