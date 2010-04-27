 #ifndef CONTROLER_H
 #define CONTROLER_H

 #include <QString>
 #include <QStringList>
 #include <QDir>
 #include <vector>
 #include <QMutex>
 #include <QDirModel>



 //#include "cthread.h"
/*
struct device{
	QString path;
	int use;
};
*/
 class Controler{
 public:
     Controler();
     static int vetsi(QString  a, QString  b);
     
     QStringList  cGetFiles(QDir path);
     QString  cShowDiskUsage(void);	

     bool  cCopy(QString source, QString destination);
     bool  cMove  (QString source, QString destination);
     int  cRename(QString source, QString destination);
     int  cMkDir (QString path,QString name);
     void  cDelete(QString file);
     void cChnangePath(int site, QString path);

     
     std::vector<struct device> *useDev;
     QMutex * devicesMutex;
     QDirModel *leftModel;
     QDirModel *rightModel;
 protected:
     
 private:
	
 };
 #endif
