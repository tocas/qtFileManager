#include "cthread.h"
#include <iostream>


CThread::CThread() : QThread() {
	mutex.release(1);
}
CThread::~CThread(){}

void CThread::setSource(QString a){ source = a; }
void CThread::setDestination(QString b){dest = b; }
void CThread::setType(int a){type = a; }
void CThread::setDevices(void * k){devices = (std::vector<struct device> *)k; }
void CThread::setDevicesMutex(void * k){devicesMutex = (QMutex *)k; }

// Types n.
// 1 - copy
// 2 - move
// 3 - delete

void CThread::run(){ 
  std::cout << "Kopirovani" << std::endl;
  std::cout << qPrintable(source) << std::endl;
  std::cout << qPrintable(dest) << std::endl;
  mutexik.lock();
  switch (type){
	  case 1:  copyFile(source,dest); break;
	  case 2:  moveFile(source,dest); break;
	  case 3:  deleteFile(source); break;
  }
  unlock();
  std::cout << "Kopirovani dokonceno" << std::endl;
}

void CThread::copyFile(QString a, QString b){
	actualySource = a;
	actualyDest = b;
  QFile file(a);
	if(QFileInfo(file).isDir()){
    QString tmp = a.right(a.length() - a.lastIndexOf("/")-1); // Vykousne nazev slozky, ktera se ma vytvorit
    QString tmp2= b.left(b.lastIndexOf("/")); // Zjisti rodice v destination
	  
    QDir dir(tmp2); 
	  dir.mkdir(tmp); // V rodicovske slozce vytvori novou slozku
	  
    QStringList childs = QDir(a).entryList();
    for(int i = 0; i < childs.size(); i++){
      if(childs.at(i) == ".." || childs.at(i) == ".") continue;
      std::cout << "Kopiruji " << qPrintable(childs.at(i)) << std::endl;
      copyFile(a + "/" + childs.at(i),tmp2 + "/" + tmp + "/" + childs.at(i));
    }
  }else{
// 		QFile soubor(a);
// 		soubor.copy(b);
	  QFile oldFile(a);
	  QFile newFile(b);
	  oldFile.open( QIODevice::ReadOnly );
	  newFile.open( QIODevice::WriteOnly );
	  uint BUFFER_SIZE = 16000;
 	  double completed = 0;
	  char* buffer = new char[BUFFER_SIZE];
	  while(!oldFile.atEnd())
	  {
		  if(mutex.available() == 0) mutexik.lock();
		  qint64 len = oldFile.read( buffer, BUFFER_SIZE );
 		  completed +=len;
		  progres = completed * 100 / oldFile.size();
		  newFile.write( buffer, len );
	  }
	  
	}
}

void CThread::moveFile(QString a, QString b){
	actualySource = a;
	actualyDest = b;
	progres = 99;
  QFile file(a);
	if(QFileInfo(file).isDir()){
    QString tmp = a.right(a.length() - a.lastIndexOf("/")-1); // Vykousne nazev slozky, ktera se ma vytvorit
    QString tmp2= b.left(b.lastIndexOf("/")); // Zjisti rodice v destination
	  
    QDir dir(tmp2); 
	  dir.mkdir(tmp); // V rodicovske slozce vytvori novou slozku
	  
    QStringList childs = QDir(a).entryList();
    
    for(int i = 0; i < childs.size(); i++){
      if(childs.at(i) == ".." || childs.at(i) == ".") continue;
      std::cout << "Presouvam " << qPrintable(childs.at(i)) << std::endl;
      moveFile(a + "/" + childs.at(i),tmp2 + "/" + tmp + "/" + childs.at(i));
    }
    QDir dirSource(a);
    QString dirName = dirSource.dirName();
    dirSource.cdUp();
    dirSource.rmpath(dirName);
  }else{
		QFile soubor(a);
		soubor.rename(b);
	}
}

void CThread::deleteFile(QString file){
	actualySource = file;
	actualyDest = "";
	progres = 50;
  QFile s(file);
  if(QFileInfo(s).isDir()){
    QDir dirSource(file);
    QStringList childs = dirSource.entryList();
    for(int i = 0; i < childs.size(); i++){
      if(childs.at(i) == ".." || childs.at(i) == ".") continue;
      deleteFile(file + "/" + childs.at(i));
    }
    QString dirName = dirSource.dirName();
    dirSource.cdUp();
    dirSource.rmpath(dirName);
  }else{
    std::cout << "Mazam " << qPrintable(file) << std::endl;
    QFile::remove(file);
  }
}

int CThread::getProgres(){
   QFile file(dest);
   QFileInfo infoFile(file);
   return infoFile.size();
}

void CThread::unlock(){
  if(type == 1 || type == 2){
    devicesMutex->lock();
    int size = devices->size();
    int sa=0,sb=0,da=0,db=0;
    for(int i = 0; i < size; i++){
      if(source.startsWith(devices->at(i).path) && devices->at(i).path.size() > sa) {
        sa = devices->at(i).path.size();
        sb = i;
      }
      if(dest.startsWith(devices->at(i).path) && devices->at(i).path.size() > da) {
        da = devices->at(i).path.size();
        db = i;
      }
    }
    devices->at(sb).use = 0;
    devices->at(db).use = 0;
    devicesMutex->unlock();
  }
	std::cout << "Konci kopirovaci vlakno" << std::endl;
}
