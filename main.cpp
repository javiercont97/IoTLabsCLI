#include <QCoreApplication>
#include <QCommandLineParser>
#include <QQmlContext>
#include <QQmlEngine>
#include <QDir>
#include <QApplication>
#include <QFile>

#include "iotlabs/iotlabsproject.h"
#include "runtime/wstt/wstt.h"
#include "runtime/quickpreview.h"

#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QQuickStyle>

void ShowArt() {
    qDebug() << "\n";
    qDebug() << "                @@@                         @@@                        ";
    qDebug() << "               @@  @@                     @@  @&                       ";
    qDebug() << "              #@     @%                 @@     @                       ";
    qDebug() << "              @@ @    ,@.             &@    @ @@@                      ";
    qDebug() << "             @@    @    @@          ,@(    @    @@                     ";
    qDebug() << "            &@      @     @@.......@@     @      @.                    ";
    qDebug() << "            .@       @                  @%      /@                     ";
    qDebug() << "             @@   .* @                  @  @    @#                     ";
    qDebug() << "              @@@    @                  @    @ @@                      ";
    qDebug() << "             #@      @                  %&     @@                      ";
    qDebug() << "            #@@   @@@%                   @@@(  @@@                     ";
    qDebug() << "           #@  @    @/////@@      @*&/////@    . @@                    ";
    qDebug() << "          #@   @            @    @            @   @@                   ";
    qDebug() << "         #@    ,             @   @            @    @@                  ";
    qDebug() << "        #@      @           @    ,%          *      @@                 ";
    qDebug() << "       /@       @@*        @      @         @@       @@                ";
    qDebug() << "         %@@@          @@   %@..,@    @,         @@@@                  ";
    qDebug() << "          (@    @.       @     @    @       @@    .@                   ";
    qDebug() << "           @        @     & %@. @@ @    @/        @@                   ";
    qDebug() << "           @#         &#              @           @@                   ";
    qDebug() << "           @@            @          @            @@*                   ";
    qDebug() << "             #@@           @     .@           %@@                      ";
    qDebug() << "                @@@          ,@@(          .@@                         ";
    qDebug() << "                   @@@         &         @@.                           ";
    qDebug() << "                      @@.      @      @@*                              ";
    qDebug() << "                        /@@    @   @@#                                 ";
    qDebug() << "                           @@@,@@@&                                    ";
    qDebug() << "                              @&                                       ";

    qDebug() << "     .___     ___________.____          ___.                           ";
    qDebug() << "     |   | ___\\__    ___/|    |   _____ \\_ |__   ______              ";
    qDebug() << "     |   |/  _ \\|    |   |    |   \\__  \\ | __ \\ /  ___/            ";
    qDebug() << "     |   (  <_> )    |   |    |___ / __ \\| \\_\\ \\___ \\             ";
    qDebug() << "     |___|\\____/|____|   |_______ (____  /___  /____  >               ";
    qDebug() << "                                 \\/    \\/    \\/     \\/             \n";
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    qmlRegisterType<WSTT>("IMT.IoTLabsWS", 1, 0, "IoTLabsWS");

    a.setApplicationName("IoT Labs CLI");
    a.setApplicationVersion("1.0");

    QQuickStyle::setStyle("Material");

    QCommandLineParser parser;

    parser.setApplicationDescription("IoT Labs Command Line Interface");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("command", QCoreApplication::translate("main", "Command to run: init, build, run, help"));
    parser.addPositionalArgument("project_file", QCoreApplication::translate("main", "Project file to use"));

    parser.process(a);

    QStringList arguments = parser.positionalArguments();

    QString command = "";

    if (arguments.length()==0) {
        ShowArt();
        parser.showHelp();
        return 0;
    } else {
        command = arguments.at(0);
    }

    if (command == "help") {
        // Provides help about any command
        ShowArt();
        parser.showHelp();
    } else if(command == "init"){
        // Initialices this folder as an IoTLabs project
        QString projectFile;
        if(arguments.length() == 2){
            projectFile = arguments.at(1).split(".")[0] + ".qml";
        } else {
            projectFile = "userApp.qml";
        }

        IoTLabsProject project(projectFile);

        if(project.init()) {
            return 0;
        } else {
            qDebug() << project.errorString;
            return 1;
        }
    } else if(command == "build") {
        // Creates project's ZIP file which is ready to upload
        QString outFile;
        if(arguments.length() == 2) {
            outFile = arguments.at(1).split(".")[0] + ".zip";
        } else {
            outFile = "build.zip";
        }

        IoTLabsProject project;
        project.buildProject(outFile);
        return 0;
    } else if(command == "run") {
        // Runs current project inside CLI's runtime
        QFile configFile("config-iotlabs.json");

        if(configFile.open(QIODevice::ReadOnly)){
            QJsonDocument config = QJsonDocument::fromJson(configFile.readAll());
            configFile.close();

            QuickPreview *preview = new QuickPreview();
            preview->setWindowTitle( config.object().value("main").toString() );
            preview->setSource("file:///" + QDir::current().filePath( config.object().value("main").toString()));
            preview->show();

            qDebug() << "\n\n==========================================================";
            qDebug() << "                         Starting                         ";
            qDebug() << "==========================================================\n\n";
        } else {
            qDebug() << configFile.errorString();
            return 1;
        }
    } else if(command == "watch") {
        // Runs current project inside CLI's runtime and watches for changes
        ShowArt();
        parser.showHelp();
    } else if(command == "add-component") {
        // Add a custom component: .qml file to be reused across de project
        if(arguments.length() == 2){
            IoTLabsProject project;
            project.addComponent( arguments.at(1) );
            qDebug() << "Component added";
            return 0;
        } else {
            qDebug() << "No component was given";
            return 1;
        }
    } else if(command == "add-resource") {
        // Add a resource: Images, Sounds, assets
        if(arguments.length() == 2){
            IoTLabsProject project;
            project.addResource( arguments.at(1) );
            qDebug() << "Resource added";
            return 0;
        } else {
            qDebug() << "No resource was given";
            return 1;
        }
    } else {
        ShowArt();
        parser.showHelp();
    }

    return a.exec();
}
