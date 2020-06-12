#include "w_errors.h"
#include "ui_w_errors.h"
#include "qwidgettaberror.h"
#include "w_summaryerrors.h"
#include "dateerrortab.h"
#include <QFileDialog>
#include "exportdata.h"

w_errors::w_errors(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::w_errors),dialogTab(new DialogImage(this))
{
    m_ui->setupUi(this);  
   
    connect(m_ui->btnExport, SIGNAL(clicked()),this, SLOT(exportExcel()));
	connect(m_ui->btnExportCSV, SIGNAL(clicked()), this, SLOT(exportCSV()));
}

w_errors::~w_errors()
{
    delete m_ui;
	
	dialogTab->close();
	
	delete dialogTab;

	for (map < int,std::vector<MetsError>*>::iterator it = brutoModel.begin(); it != brutoModel.end(); ++it)
	{
		delete it->second;
	}
	brutoModel.clear();

}

void w_errors::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//! set detail of the batch
void w_errors::setBatchDetail()
{ 	
	getTaberrors();
}

void w_errors::getTaberrors()
{
	w_summaryerrors *w_s = new w_summaryerrors();
	dbrequest db;
	db.setDataBaseName(BatchDetail::getBatchDetail().database);
	w_s->setBatchDetail();
	m_ui->scrollAreaErrorTable->setWidget(w_s);	
	std::map<int,ErrorCategory> vCategory = db.getErrorCategory(); 
	
	db.getAllvError(brutoModel, BatchDetail::getBatchDetail().idTestSet);
	for (map < int,std::vector<MetsError>*>::iterator it = brutoModel.begin(); it != brutoModel.end(); ++it)
	{
		it->first;
		tabErrors *e = new tabErrors(it->first,it->second,BatchDetail::getBatchDetail(),dialogTab);	
		if (e->getSizeVError() !=0)
		{			
			m_ui->tabWidget->insertTab(it->first+1,e,vCategory[it->first].name.c_str());		
		}
	}
	dateerrortab *d = new dateerrortab(6);		
	if( d->getSizeVError() !=0){
		m_ui->tabWidget->addTab(d,"DATES");
	}
}

void w_errors::exportCSV() {
	QString dirName = QFileDialog::getExistingDirectory(this, tr("Select Directory"));
	
	if (!dirName.isEmpty()) {
		exportdata* data = new exportdata();
		data->exportErrorsCSV(BatchDetail::getBatchDetail(), dirName.toStdString());
		delete data;
	}
}

void w_errors::exportExcel()
{
	QString fileName = QFileDialog::getSaveFileName(this,tr("Open File"),"",tr("file (*.ods)"));
    
     if (!fileName.isEmpty()) 
     {			
		 exportdata *data = new exportdata();
		 data->exportErrors(BatchDetail::getBatchDetail(),fileName.toStdString());
		 delete data;
	}  	
}