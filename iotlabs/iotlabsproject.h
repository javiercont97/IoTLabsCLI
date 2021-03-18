#ifndef IOTLABSPROJECT_H
#define IOTLABSPROJECT_H

#include <QObject>

class IoTLabsProject : public QObject {
    Q_OBJECT
public:
    explicit IoTLabsProject(QString projectFile = "", QObject *parent = nullptr);

    bool init();
    bool createProjectFile();
    bool createAppFile();
    QString errorString;
    void buildProject(QString projectFile);

    void addComponent(QString component);
    void addResource(QString resource);

signals:


private:
    QString projectFile;

};

#endif // IOTLABSPROJECT_H
