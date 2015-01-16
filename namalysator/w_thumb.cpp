#include "w_thumb.h"
#include <sstream>
#include <iomanip>


void w_thumb::cancel(){
	canceled = true;
}

void w_thumb::perform(){
	
	QImage image;
	QImage imagebis;
	if ( canceled ) { 
		timer -> stop();
		return;
	}

	stepi++;
	int i = stepi;

	if ( stepi > nbPage ){
		timer -> stop();
		return;
	}

	
	emit nextValue(i);
	std::stringstream ss;
	
    int jj=i%56;

	ss<< "C:\\SmallTestBook\\243149\\tif\\" << std::setfill('0')<<std::setw(5)<< jj << ".tif";
	//ss<< "E:\\tif\\" << std::setfill('0')<<std::setw(5)<< jj << ".tif";
	emit newText(ss.str().c_str());
	image.load(ss.str().c_str());
	;
    ss<<" -> " << stepi;
		
	QPushButton* _but=allButton[stepi - 1];
	_but ->setToolTip(ss.str().c_str());
	_but ->setIcon(QPixmap::fromImage(image.scaled(200,200,Qt::KeepAspectRatio,
		           Qt::SmoothTransformation /*Qt::FastTransformation*/),
				   Qt::DiffuseAlphaDither|Qt::ColorOnly));
	_but ->setIconSize(QSize(200,200));

	 QPixmapCache::clear();
	
		
		
		
	
}

void w_thumb::load(){
	canceled = false;
	stepi = 0;
	dlg = new QProgressDialog();
	dlg->setWindowTitle("Loading thumbnail");
	dlg->setRange(0, nbPage); // number max of page

	timer=new QTimer ();
 
	QObject::connect(this, SIGNAL(nextValue(int)), dlg, SLOT(setValue(int)));
	QObject::connect(this, SIGNAL(newText(QString)), dlg, SLOT(setLabelText(QString)));
	QObject::connect(dlg, SIGNAL(canceled()), this, SLOT(cancel()));
	connect(timer, SIGNAL(timeout()), this, SLOT(perform()));

	dlg->show();
	timer->start();
	//dlg.exec (); // local loop of event 
}

void w_thumb::init(){
	count=0;

	_mainLayout = new QVBoxLayout();
	this->setLayout(_mainLayout);
	_sbar = new QScrollArea();
	_sbar->setMinimumSize(QSize(420,420));
	_sbar->setMaximumSize(QSize(19000,19000));
	_sbar->resize(this->frameSize());	
	_sbar->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
	_sbar->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	_sbar->setWidgetResizable(true);
	_mainLayout->addWidget(_sbar);

	internal = new QWidget();
    
	

	//internal->setMinimumWidth(_sbar->);
	//internal->setMinimumHeight(200);
	
	internal->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

	createButton();
	_sbar->setWidget(internal);
	//internal->setLayout(_qvLayout);
	internal->setLayout(currentLayout);
	show();
	//load();
}

void w_thumb::createButton(){
	//_qvLayout = new QVBoxLayout();
	//_qvLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
	//_qvLayout->setSpacing(1);
	
	currentLayout = new FlowLayout();
	//_qvLayout->addLayout(currentLayout);
	
	//curr = &_qvLayout;

	
	QPushButton* _but;
	for (int i = 0;i<nbPage;i++){
		_but = new QPushButton();
		_but ->setMinimumSize(200,200);
		_but ->setMaximumSize(370,370);
		_but ->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
		addInternal(_but);
		allButton.push_back(_but);
	};
	
}

void w_thumb::addInternal(QWidget * wid){
	currentLayout->addWidget(wid);
	count ++;
	/*
	if ( count == 6 ) {
		count = 0;
		currentLayout = new QHBoxLayout();
		_qvLayout->addLayout(currentLayout);
	}
	*/
}

w_thumb::w_thumb(int nb,QWidget *parent):QWidget(parent),nbPage(nb){
	init();
}

w_thumb::~w_thumb(){
}