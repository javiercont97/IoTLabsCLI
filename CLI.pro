QT       += gui core
QT       += quickwidgets     ## qt quick support
QT       += quickcontrols2   ## material design support
QT       += network          ## http request support
QT       += websockets       ## websockets request support
QT       += mqtt             ## mqtt support
QT       += xml              ## xml writing support


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console
# CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        iotlabs/iotlabsproject.cpp \
        main.cpp \
        runtime/quickpreview.cpp \
        runtime/wstt/wstt.cpp

TRANSLATIONS += \
    CLI_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    iotlabs/iotlabsproject.h \
    runtime/quickpreview.h \
    runtime/wstt/wstt.h


FORMS += \
    runtime/quickpreview.ui

QML_IMPORT_PATH += \
    Controls/

RESOURCES += \
    assets.qrc
