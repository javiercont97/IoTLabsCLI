#include "iotlabsproject.h"

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QtXml>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>


#include <QDebug>

IoTLabsProject::IoTLabsProject(QString projectFile, QObject *parent) : QObject(parent) {
    this->projectFile = projectFile;
    errorString = "";
}


bool IoTLabsProject::init() {
    return createProjectFile() && createAppFile();
}

bool IoTLabsProject::createProjectFile() {
    QFile configFile("config-iotlabs.json");

    if(configFile.exists()) {
        errorString = "Project already exists";
        return false;
    }

    if(!configFile.open(QIODevice::WriteOnly)) {
        errorString = configFile.errorString();
        return false;
    } else {

        QJsonDocument projectFileContent;
        QJsonObject rootObj;

        rootObj.insert("main", projectFile);

        QJsonArray resources;
        rootObj.insert("resources", resources);

        QJsonArray components;
        rootObj.insert("components", components);

        projectFileContent.setObject(rootObj);

        configFile.write(projectFileContent.toJson());

        configFile.close();
        return true;
    }
}

bool IoTLabsProject::createAppFile() {
    QFile appFile(projectFile);

    if(appFile.exists()) {
        errorString = "Project already exists";
        return false;
    }

    if(!appFile.open(QIODevice::WriteOnly)) {
        errorString = appFile.errorString();
        return false;
    } else {
        appFile.write("import QtQuick 2.12\nimport QtQuick.Controls 2.12\nimport QtQuick.Layouts 1.15\nimport Controls 1.0\nimport IMT.IoTLabsWS 1.0\n// Styling imports\nimport QtQuick.Controls.Material 2.12\n\n\nItem {\n\tid: rootItem\n\t\n\t\n}");

        appFile.close();
        return true;
    }
}

void IoTLabsProject::buildProject(QString outFile) {
    QString outputPackage = QDir::current().filePath(outFile);

    QFile configFile("config-iotlabs.json");

    if(configFile.open(QIODevice::ReadOnly)){
        QJsonDocument config = QJsonDocument::fromJson(configFile.readAll());

        if(config.object().value("main").toString() != "userApp.qml") {
            QFile::copy(config.object().value("main").toString(), "userApp.qml");
        }

        QDomDocument doc;
        QDomElement root = doc.createElement("RCC");
        doc.appendChild(root);

        QDomElement resources = doc.createElement("qresource");
        resources.setAttribute("prefix", "/");
        root.appendChild(resources);

        QStringList platformSpecific;
        platformSpecific << "main.qml" << "userApp.qml" << "Controls/qmldir" << "Controls/plugins.qmltypes" << "Controls/Gauge.qml";
        foreach(QString platformFile, platformSpecific) {
            QDomElement platformTags = doc.createElement("file");
            resources.appendChild(platformTags);
            QDomText t = doc.createTextNode(platformFile);
            platformTags.appendChild(t);
        }

        QStringList args = QStringList() << "-a" << "-c" << "-f" << outputPackage << "userApp.qml";
        foreach(QJsonValue resource, config.object().value("resources").toArray()) {
            args << resource.toString();

            QDomElement userTags = doc.createElement("file");
            resources.appendChild(userTags);
            QDomText t = doc.createTextNode(resource.toString());
            userTags.appendChild(t);
        }

        foreach(QJsonValue component, config.object().value("components").toArray()) {
            args << component.toString();

            QDomElement userTags = doc.createElement("file");
            resources.appendChild(userTags);
            QDomText t = doc.createTextNode(component.toString());
            userTags.appendChild(t);
        }

        QFile qrcFile("qml.qrc");
        if(qrcFile.open(QIODevice::WriteOnly)){
            qrcFile.write( doc.toByteArray() );
            qrcFile.close();

            args << "qml.qrc";
            QProcess process;;
            process.start("tar.exe", args);
            process.waitForFinished();

            if(config.object().value("main").toString() != "userApp.qml") {
                QFile::remove("userApp.qml");
            }

            QFile::remove("qml.qrc");

            configFile.close();

            qDebug() << "Job is done";
        } else {
            qDebug() << qrcFile.errorString();
        }
    } else {
        qDebug() << configFile.errorString();
    }
}

void IoTLabsProject::addComponent(QString component) {
    QFile configFile("config-iotlabs.json");

    if(configFile.open(QIODevice::ReadOnly)){
        QJsonDocument config = QJsonDocument::fromJson(configFile.readAll());
        configFile.close();

        QJsonDocument outConfig;
        QJsonObject root;
        root.insert("resources", config.object().value("resources"));
        root.insert("main", config.object().value("main"));

        QJsonArray components = config.object().value("components").toArray();
        components.append(component);
        root.insert("components", components);

        outConfig.setObject(root);
        if(configFile.open(QIODevice::WriteOnly)) {
            configFile.write(outConfig.toJson());
            configFile.close();
        } else {
            qDebug() << configFile.errorString();
        }
    } else {
        qDebug() << configFile.errorString();
    }
}

void IoTLabsProject::addResource(QString resource) {
    QFile configFile("config-iotlabs.json");

    if(configFile.open(QIODevice::ReadOnly)){
        QJsonDocument config = QJsonDocument::fromJson(configFile.readAll());
        configFile.close();

        QJsonDocument outConfig;
        QJsonObject root;
        root.insert("components", config.object().value("components"));
        root.insert("main", config.object().value("main"));

        QJsonArray resources = config.object().value("resources").toArray();
        resources.append(resource);
        root.insert("resources", resources);

        outConfig.setObject(root);
        if(configFile.open(QIODevice::WriteOnly)) {
            configFile.write(outConfig.toJson());
            configFile.close();
        } else {
            qDebug() << configFile.errorString();
        }
    } else {
        qDebug() << configFile.errorString();
    }
}

