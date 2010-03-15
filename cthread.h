#ifndef CTHREAD_H
#define CTHREAD_H

#include <QThread>
#include <QString>
#include <QFile>
#include <QWidget>
#include <QFileInfo>
#include <QDir>
#include <QMutex>
#include <QSemaphore>
#include <vector>

struct device{
	QString path;
	int use;
};
  
class CThread : public QThread
{
public:
    CThread();
    //struct device * devices;
    std::vector<struct device> *devices;    
    virtual void run();
    void setSource(QString);
    void setDestination(QString);
    void setType(int);
    void setDevices(void *);
    void setDevicesMutex(void *);
    void copyFile(QString,QString);
    void moveFile(QString,QString);
    void deleteFile(QString);
    void unlock(void);
    int getProgres(void);
    QString actualySource;
    QString actualyDest;
    int progres;
    QSemaphore mutex;
    QMutex mutexik;
    QMutex *devicesMutex;
    ~CThread();
     
private:
    QString source;
    QString dest;
    int type;

};

#endif
