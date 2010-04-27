 
 #include <iostream>
 #include <dirent.h>
 #include <sys/stat.h>
 #include <sys/types.h>
 #include <QFile>
 #include <QProgressDialog>
 #include <QFileInfo>
 #include <QTextStream>
 #include <QDir>
 #include "CProgresDialog.h"
 #include <QMessageBox>
 #include "controler.h"
 
 
 Controler::Controler(){
  devicesMutex = new QMutex();
  leftModel = new QDirModel();
  leftModel->setResolveSymlinks(true);
  leftModel->setReadOnly( false );
  rightModel = new QDirModel();
  rightModel->setResolveSymlinks(true);
  leftModel->setReadOnly( false );
 }
 
 int Controler::vetsi(QString  a, QString  b){
    if(a > b) return 0;
    else return 1;
 }
 
 QStringList Controler::cGetFiles(QDir path)
 {
    QDir directory(path);
    directory.setSorting(QDir::IgnoreCase);
    return directory.entryList();
 }
 

 QString Controler::cShowDiskUsage()
 {
	QString a(system("df -h"));
	return a;
 }
 
 bool Controler::cCopy(QString source, QString destination)
 {
     CProgresDialog *progrDialog = new CProgresDialog();
     progrDialog->setSource(source);
     progrDialog->setDestination(destination);
     progrDialog->type = 1;
     progrDialog->show();
     progrDialog->activateWindow();
 }


 bool Controler::cMove(QString source,QString destination)
 {
	 CProgresDialog *progrDialog = new CProgresDialog();
	 progrDialog->setSource(source);
	 progrDialog->setDestination(destination);
	 progrDialog->type = 2;
	 progrDialog->show();
	 progrDialog->activateWindow();

 }

 void Controler::cDelete(QString source)
 {
	 CProgresDialog *progrDialog = new CProgresDialog();
	 progrDialog->setSource(source);
	 progrDialog->setDestination("");
	 progrDialog->type = 3;
	 progrDialog->show();
	 progrDialog->activateWindow();
 }

 int Controler::cMkDir(QString path, QString name){
	 QDir dir(path);
	 return dir.mkdir(name);
 }

 int Controler::cRename(QString source, QString destination){
        QFile file(source);
        return file.rename(destination);
 } 

 void Controler::cChnangePath(int site, QString path){
     if(site == 0){
        //leftModel->setRootPath(path);
    } else {
        //rightModel->setRootPath(path);
    }
 }


 
 
