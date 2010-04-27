 #include <QtGui>
 #include "window.h"
 #include "controler.h"
 #include <QLayout>
 #include <QPushButton>
 #include <QMenuBar>
 #include <QWidget>
 #include <QListView>
 #include <QListWidgetItem>
 #include <QEvent>
 #include <QString>
 #include <sstream>
 #include <iostream>
 #include <QInputDialog>
 #include <QFileInfo>
 #include <QMessageBox>

 QListWidgetItem * item1;

 int listFocus;
  
 Window::Window()
 {
     QWidget *widget = new QWidget;
     setCentralWidget(widget);



     infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                               "invoke a context menu</i>"));
     infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
     //infoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     infoLabel->setAlignment(Qt::AlignCenter);

     bottomButton = new QWidget;
     buttons  = new QHBoxLayout;

     createButtons();

     bottomButton->setLayout(buttons);

     QWidget *panelList = new QWidget;
     QHBoxLayout *lists = new QHBoxLayout;
  
     QWidget *listL = new QWidget;
     QVBoxLayout *lisL = new QVBoxLayout;
     QWidget *listR = new QWidget;
     QVBoxLayout *lisR = new QVBoxLayout;
     
     comboL = new QComboBox;
     comboR = new QComboBox;
     
     viewL  = new QListWidget;
     viewR  = new QListWidget;

     lisL->setMargin(0);
     lisL->addWidget(comboL);
     lisL->addWidget(&treeL);
     lisR->setMargin(0);
     lisR->addWidget(comboR);
     lisR->addWidget(&treeR);
     listL->setLayout(lisL);
     listR->setLayout(lisR);
     
     lists->setMargin(0);
     lists->addWidget(listL);



     lists->addWidget(listR);
     
     panelList->setLayout(lists);
          
     initializeDevices();

     dirL.setPath(QDir::rootPath());
     dirR.setPath(QDir::homePath());
     refreshList(0,dirL);
     refreshList(1,dirR);
     
     lcd = new QLCDNumber(2);
     lcd->setSegmentStyle(QLCDNumber::Filled);
     lcd->display(19);
     
     
     QVBoxLayout *layout = new QVBoxLayout;
     layout->setMargin(0);
     //layout->addWidget(lcd);
     layout->addWidget(infoLabel);
     layout->addWidget(panelList);
     layout->addWidget(bottomButton);
     widget->setLayout(layout);
          
     kontroler = new Controler();
     treeL.setModel(kontroler->leftModel);
     treeL.setAnimated(false);
     treeL.setIndentation(20);
     treeL.setSortingEnabled(true);

     treeR.setModel(kontroler->leftModel);
     treeR.setAnimated(true);
     treeR.setIndentation(20);
     treeR.setSortingEnabled(true);


     createActions();
     createMenus();

     setWindowTitle(tr("File Manager CPM"));
     setMinimumSize(300, 200);
     resize(800, 600);
     

     
     listFocus = 0;
     switchFocus();
 }

 void Window::newFile()
 {
    infoLabel->setText(tr("Invoked <b>File|New</b>"));
 }


 
 void Window::copyF()
 {
	 bool ok = true;
	 QString source,dest;
	 if(listFocus == 0){
		 source = viewL->item(viewL->currentRow())->text();
		 dest = dirR.path() + "/" + source;
		 ok = kontroler->cCopy(dirL.path() + "/" + source,dest);
	 }else{
		 source = viewR->item(viewR->currentRow())->text();
		 dest = dirL.path() + "/" + source;
		 ok = kontroler->cCopy(dirR.path() + "/" + source,dest);
	 }
	 refreshList(0,dirL);
	 refreshList(1,dirR);
	 if(!ok) QMessageBox::warning(this, tr("tc"),
	     tr("Can't copy file!!!"),QMessageBox::Cancel);
 }

 void Window::moveF()
 {
	 bool ok = true;
	 QString source,dest;
	 if(listFocus == 0){
		 source = viewL->item(viewL->currentRow())->text();
		 dest = dirR.path() + "/" + source;
		 ok = kontroler->cMove(dirL.path() + "/" + source,dest);
	 }else{
		 source = viewR->item(viewR->currentRow())->text();
		 dest = dirL.path() + "/" + source;
		 ok = kontroler->cMove(dirR.path() + "/" + source,dest);
	 }
	 refreshList(0,dirL);
	 refreshList(1,dirR);
	 if(!ok) QMessageBox::warning(this, tr("tc"),
	     tr("Can't rename file!!!"),QMessageBox::Cancel);
 }

 void Window::mkdir()
 {
	bool ok = true;
	if(listFocus == 0 ){
    ok = kontroler->cMkDir(dirL.path(),QInputDialog::getText(this, tr("New Dir"),
		  tr("Input the new dir name"), QLineEdit::Normal,"", &ok));
	}else{
    ok = kontroler->cMkDir(dirR.path(),QInputDialog::getText(this, tr("New Dir"),
		  tr("Input the new dir name"), QLineEdit::Normal,"", &ok));
	}
	refreshList(listFocus,dirR);
	if(!ok) QMessageBox::warning(this, tr("tc"), tr("Can't create directory!!!"),QMessageBox::Cancel);
 }

 void Window::deleteF()
 {
  bool ok = true;
	QString source;
	if(listFocus == 0){
    source = viewL->item(viewL->currentRow())->text();
    kontroler->cDelete(dirL.path() + "/" + source);
	}else{
    source = viewR->item(viewR->currentRow())->text();
    kontroler->cDelete(dirR.path() + "/" + source);
  }
  refreshList(listFocus,dirR);
	if(!ok) QMessageBox::warning(this, tr("tc"), tr("Can't delete file!!!"),QMessageBox::Cancel);
 }

 void Window::renameF()
 {
  bool ok = true;
  QString source,dest;
  if(listFocus == 0){
	  source = viewL->item(viewL->currentRow())->text();
	  dest = QInputDialog::getText(this, tr("Rename"),
				       tr("Input the new name of file"), QLineEdit::Normal, source, &ok);
	  ok = kontroler->cRename(dirL.path() + "/" + source,dirL.path() + "/" + dest);
  }else{
    source = viewR->item(viewR->currentRow())->text();
    dest = QInputDialog::getText(this, tr("Rename"),
				    tr("Input the new name of file"), QLineEdit::Normal, source, &ok);
    ok = kontroler->cRename(dirR.path() + "/" + source,dirR.path() + "/" + dest);
  }
  refreshList(listFocus,dirR);
  if(!ok) QMessageBox::warning(this, tr("tc"), tr("Can't rename file!!!"),QMessageBox::Cancel);
 }

 void Window::bold()
 {
     infoLabel->setText(tr("Invoked <b>Edit|Format|Bold</b>"));
 }

 void Window::italic()
 {
     infoLabel->setText(tr("Invoked <b>Edit|Format|Italic</b>"));
 }

 void Window::about()
 {
     infoLabel->setText(tr("Invoked <b>Help|About</b>"));
     QMessageBox::about(this, tr("About Menu"), tr("The <b>Menu</b> example shows how to create menu-bar menus and context menus."));
 }

 void Window::aboutQt()
 {
     infoLabel->setText(tr("Invoked <b>Help|About Qt</b>"));
 }
 
  void Window::dblClickListL()
  {
    if(viewL->currentRow() != 0){
      QFile tmp(dirL.path() + "/" + viewL->item(viewL->currentRow())->text());
      QFileInfo tmpInfo(tmp);
      if(tmpInfo.isDir()){
        dirL.setPath( QDir::cleanPath(tmp.fileName()));
        refreshList(0,dirL);
      }else{ 
        infoLabel->setText(tmpInfo.fileName() + " size: " + QString::number(tmpInfo.size()));
      }
    }
    listFocus = 0;
  }
 
  void Window::dblClickListR()
  {
    if(viewR->currentRow() != 0){
      QFile tmp(dirR.path() + "/" + viewR->item(viewR->currentRow())->text());
      QFileInfo tmpInfo(tmp);
      if(tmpInfo.isDir()){
        dirR.setPath( QDir::cleanPath(tmp.fileName()));
	      refreshList(1,dirR);
      }else{ 
        infoLabel->setText(tmpInfo.fileName() + " size: " + QString::number(tmpInfo.size()));
      }
    }
    listFocus = 1;
 }
 
 void Window::setFocusL() { listFocus = 0;}
 void Window::setFocusR() { listFocus = 1;}

 void Window::setPathL(int a) { 
    kontroler->cChnangePath(0,comboL->itemText(a));
 }
 void Window::setPathR(int a) {
  kontroler->cChnangePath(1,comboR->itemText(a));
 }

 void Window::createButtons()
  {
     tabB     = new QPushButton(tr("Tab"),      this);
     bbRefresh= new QPushButton(tr("Refresh"),  this);
     bbCopy   = new QPushButton(tr("F5 Copy"),  this);
     bbMove   = new QPushButton(tr("F6 Move"),  this);
     bbMkdir  = new QPushButton(tr("F7 Mkdir"), this);
     bbDelete = new QPushButton(tr("F8 Delete"),this);
     bbRename = new QPushButton(tr("F9 Rename"),this);
     bbQuit   = new QPushButton(tr("F10 Quit"), this);
     
     buttons->addWidget(tabB);
     buttons->addWidget(bbRefresh);
     buttons->addWidget(bbCopy);
     buttons->addWidget(bbMove);
     buttons->addWidget(bbMkdir);
     buttons->addWidget(bbDelete);
     buttons->addWidget(bbRename);
     buttons->addWidget(bbQuit);
     
  }

 void Window::createActions()
 {
     newFileAct = new QAction(tr("&New file"), this);
     newFileAct->setShortcut(tr("Ctrl+N"));
     connect(newFileAct, SIGNAL(triggered()), this, SLOT(newFile()));

     newDirectoryAct = new QAction(tr("&New directory"), this);
     connect(newDirectoryAct, SIGNAL(triggered()), this, SLOT(mkdir()));

     exitAct = new QAction(tr("E&xit"), this);
     exitAct->setShortcut(tr("Ctrl+Q"));
     connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

     boldAct = new QAction(tr("&Bold"), this);
     boldAct->setCheckable(true);
     boldAct->setShortcut(tr("Ctrl+B"));
     connect(boldAct, SIGNAL(triggered()), this, SLOT(bold()));

     QFont boldFont = boldAct->font();
     boldFont.setBold(true);
     boldAct->setFont(boldFont);

     italicAct = new QAction(tr("&Italic"), this);
     italicAct->setCheckable(true);
     italicAct->setShortcut(tr("Ctrl+I"));
     connect(italicAct, SIGNAL(triggered()), this, SLOT(italic()));

     QFont italicFont = italicAct->font();
     italicFont.setItalic(true);
     italicAct->setFont(italicFont);

     aboutAct = new QAction(tr("&About"), this);
     connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

     aboutQtAct = new QAction(tr("About &Qt"), this);
     connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
     connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(aboutQt()));

     connect(bbRefresh, SIGNAL(clicked()), this, SLOT(refresh()));
     bbRefresh->setShortcut(tr("F3"));
     connect(bbCopy, SIGNAL(clicked()), this, SLOT(copyF()));
     bbCopy->setShortcut(tr("F5"));
     connect(bbMove, SIGNAL(clicked()), this, SLOT(moveF()));
     bbMove->setShortcut(tr("F6"));
     connect(bbMkdir, SIGNAL(clicked()), this, SLOT(mkdir()));
     bbMkdir->setShortcut(tr("F7"));
     connect(bbDelete, SIGNAL(clicked()), this, SLOT(deleteF()));
     bbDelete->setShortcut(tr("F8"));
     connect(bbRename, SIGNAL(clicked()), this, SLOT(renameF()));
     bbRename->setShortcut(tr("F9"));
     connect(bbQuit, SIGNAL(clicked()), this, SLOT(close()));
     bbQuit->setShortcut(tr("F10"));
     
     connect(tabB, SIGNAL(clicked()), this, SLOT(switchFocus()));
     tabB->setShortcut(tr("Tab"));
     
     connect(viewL, SIGNAL(itemClicked( QListWidgetItem * )), this, SLOT(setFocusL()));
     connect(viewR, SIGNAL(itemClicked( QListWidgetItem * )), this, SLOT(setFocusR()));
     connect(viewL, SIGNAL(itemDoubleClicked( QListWidgetItem * )), this, SLOT(dblClickListL()));
     connect(viewR, SIGNAL(itemDoubleClicked( QListWidgetItem * )), this, SLOT(dblClickListR()));
     connect(comboL, SIGNAL(currentIndexChanged( int )), this, SLOT(setPathL(int)));
     connect(comboR, SIGNAL(currentIndexChanged( int )), this, SLOT(setPathR(int)));
     
 }

 void Window::createMenus()
 {
     fileMenu = menuBar()->addMenu(tr("&File"));
     fileMenu->addAction(newFileAct);
     fileMenu->addAction(newDirectoryAct);
     fileMenu->addSeparator();
     fileMenu->addAction(exitAct);

     editMenu = menuBar()->addMenu(tr("&Edit"));
     editMenu->addSeparator();

     helpMenu = menuBar()->addMenu(tr("&Help"));
     helpMenu->addAction(aboutAct);
     helpMenu->addAction(aboutQtAct);

     formatMenu = editMenu->addMenu(tr("&Format"));
     formatMenu->addAction(boldAct);
     formatMenu->addAction(italicAct);
     //formatMenu->addSeparator();

 }

 void Window::refreshList(int list, QDir path){
  QListWidget * qlw;
  if(list == 0){
    qlw = viewL;
  }else{
    qlw = viewR;
  }
  qlw->clear();
  qlw->addItems(kontroler->cGetFiles(path));
 }
 void Window::switchFocus()
 {
  if(listFocus == 0){
    viewR->setFocus();
  }else{
    viewL->setFocus();
  }
  listFocus = (listFocus + 1)%2;
 }
 
 void Window::initializeDevices(){
     QFileInfoList drivers = QDir::drives();
     for(int i = 0; i < drivers.size(); ++i){
         devices << drivers.at(i).absolutePath();
     }

     #ifdef Q_WS_MAC
         QDir volumes( "/Volumes");
         QFileInfoList s = volumes.entryInfoList();
         for(int i = 0; i < s.size(); ++i){
             if(! devices.contains(s.at(i).absoluteFilePath())) devices << s.at(i).absoluteFilePath();
         }
     #endif

     comboL->addItems(devices);
     comboR->addItems(devices);
 }

 void Window::refresh(){
	 refreshList(1,dirR);
	 refreshList(0,dirL);
 }
