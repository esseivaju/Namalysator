#ifndef W_ERRORS_H
#define W_ERRORS_H

#include <QWidget>
#include "structgui.h"
#include "dialogImage.h"

namespace Ui {
    class w_errors;
}

class w_errors : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(w_errors)
public:
    explicit w_errors(QWidget *parent = 0);
    virtual ~w_errors();
     void setBatchDetail();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::w_errors *m_ui;
	DialogImage* dialogTab;
    //BatchDetail batchdetail;
	map < int,std::vector<MetsError>*> brutoModel;
    void getTaberrors();
private slots:
	void exportExcel();
    void exportCSV();
};

#endif // W_ERRORS_H
